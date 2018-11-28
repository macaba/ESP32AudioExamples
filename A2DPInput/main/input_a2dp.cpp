#include "input_a2dp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"

#include "esp_bt.h"
#include "bt_app_core.h"
#include "bt_app_av.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"
#include "esp_avrc_api.h"

// #define BT_AV_TAG               "BT_AV"

// /* event for handler "bt_av_hdl_stack_up */
// enum {
//     BT_APP_EVT_STACK_UP = 0,
// };

// /* A2DP global state */
// enum {
//     APP_AV_STATE_IDLE,
//     APP_AV_STATE_DISCOVERING,
//     APP_AV_STATE_DISCOVERED,
//     APP_AV_STATE_UNCONNECTED,
//     APP_AV_STATE_CONNECTING,
//     APP_AV_STATE_CONNECTED,
//     APP_AV_STATE_DISCONNECTING,
// };

// /* sub states of APP_AV_STATE_CONNECTED */
// enum {
//     APP_AV_MEDIA_STATE_IDLE,
//     APP_AV_MEDIA_STATE_STARTING,
//     APP_AV_MEDIA_STATE_STARTED,
//     APP_AV_MEDIA_STATE_STOPPING,
// };

// #define BT_APP_HEART_BEAT_EVT                (0xff00)

// /// handler for bluetooth stack enabled events
// static void bt_av_hdl_stack_evt(uint16_t event, void *p_param);

// /// callback function for A2DP source
// static void bt_app_a2d_cb(esp_a2d_cb_event_t event, esp_a2d_cb_param_t *param);

// /// callback function for A2DP source audio data stream
// static int32_t bt_app_a2d_data_cb(uint8_t *data, int32_t len);

// static void a2d_app_heart_beat(void *arg);

// /// A2DP application state machine
// static void bt_app_av_sm_hdlr(uint16_t event, void *param);

// /* A2DP application state machine handler for each state */
// static void bt_app_av_state_unconnected(uint16_t event, void *param);
// static void bt_app_av_state_connecting(uint16_t event, void *param);
// static void bt_app_av_state_connected(uint16_t event, void *param);
// static void bt_app_av_state_disconnecting(uint16_t event, void *param);

// static esp_bd_addr_t peer_bda = {0};
// static uint8_t peer_bdname[ESP_BT_GAP_MAX_BDNAME_LEN + 1];
// static int m_a2d_state = APP_AV_STATE_IDLE;
// static int m_media_state = APP_AV_MEDIA_STATE_IDLE;
// static int m_intv_cnt = 0;
// static int m_connecting_intv = 0;
// static uint32_t m_pkt_cnt = 0;

// TimerHandle_t tmr;

// static char *bda2str(esp_bd_addr_t bda, char *str, size_t size)
// {
//     if (bda == NULL || str == NULL || size < 18) {
//         return NULL;
//     }

//     uint8_t *p = bda;
//     sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
//             p[0], p[1], p[2], p[3], p[4], p[5]);
//     return str;
// }

/* event for handler "bt_av_hdl_stack_up */
enum {
    BT_APP_EVT_STACK_UP = 0,
};

/* handler for bluetooth stack enabled events */
static void bt_av_hdl_stack_evt(uint16_t event, void *p_param);

void AudioInputA2DP::init()
{
    if(initialised)
        return;

	update_setup();			//This signals that the update method will block so we don't end up with an update loop out of control

    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((err = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    if ((err = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    if ((err = esp_bluedroid_init()) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    if ((err = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    /* create application task */
    bt_app_task_start_up();

    /* Bluetooth device name, connection mode and profile set up */
    bt_app_work_dispatch(bt_av_hdl_stack_evt, BT_APP_EVT_STACK_UP, NULL, 0, NULL);

    /* Set default parameters for Secure Simple Pairing */
    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_IO;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));

    /*
     * Set default parameters for Legacy Pairing
     * Use fixed pin code
     */
    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_FIXED;
    esp_bt_pin_code_t pin_code;
    pin_code[0] = '1';
    pin_code[1] = '2';
    pin_code[2] = '3';
    pin_code[3] = '4';
    esp_bt_gap_set_pin(pin_type, 4, pin_code);

    printf("Bluetooth initialised.\n");
    initialised = true;
}

void IRAM_ATTR AudioInputA2DP::update(void)
{
	audio_block_t *new_out=NULL;

	//Serial.println("update");
    if(!initialised)
    {
        return;
    }

	size_t bytesRead = 0;
	//i2s_adc_enable(I2S_NUM_0);
	//i2s_read(I2S_NUM_0, (char*)&inputSampleBuffer, (AUDIO_BLOCK_SAMPLES * sizeof(uint32_t)), &bytesRead, portMAX_DELAY);		//Block but yield to other tasks
    //i2s_adc_disable(I2S_NUM_0);

	new_out = allocate();
	if(new_out != NULL){
		for(int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
		{
			//new_out->data[i] = ((float)(inputSampleBuffer[i] & 0xfff)/2048.0f) - 1.0f;
		}

		transmit(new_out);
		release(new_out);
	}

	//Serial.println(new_out->data[0]);
}

void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    switch (event) {
    case ESP_BT_GAP_AUTH_CMPL_EVT: {
        if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS) {
            ESP_LOGI(BT_AV_TAG, "authentication success: %s", param->auth_cmpl.device_name);
            esp_log_buffer_hex(BT_AV_TAG, param->auth_cmpl.bda, ESP_BD_ADDR_LEN);
        } else {
            ESP_LOGE(BT_AV_TAG, "authentication failed, status:%d", param->auth_cmpl.stat);
        }
        break;
    }
    case ESP_BT_GAP_CFM_REQ_EVT:
        ESP_LOGI(BT_AV_TAG, "ESP_BT_GAP_CFM_REQ_EVT Please compare the numeric value: %d", param->cfm_req.num_val);
        esp_bt_gap_ssp_confirm_reply(param->cfm_req.bda, true);
        break;
    case ESP_BT_GAP_KEY_NOTIF_EVT:
        ESP_LOGI(BT_AV_TAG, "ESP_BT_GAP_KEY_NOTIF_EVT passkey:%d", param->key_notif.passkey);
        break;
    case ESP_BT_GAP_KEY_REQ_EVT:
        ESP_LOGI(BT_AV_TAG, "ESP_BT_GAP_KEY_REQ_EVT Please enter passkey!");
        break;
    default: {
        ESP_LOGI(BT_AV_TAG, "event: %d", event);
        break;
    }
    }
    return;
}

static void bt_av_hdl_stack_evt(uint16_t event, void *p_param)
{
    ESP_LOGD(BT_AV_TAG, "%s evt %d", __func__, event);
    switch (event) {
    case BT_APP_EVT_STACK_UP: {
        /* set up device name */
        char *dev_name = "ESP_SPEAKER";
        esp_bt_dev_set_device_name(dev_name);

        esp_bt_gap_register_callback(bt_app_gap_cb);
        /* initialize A2DP sink */
        esp_a2d_register_callback(&bt_app_a2d_cb);
        esp_a2d_sink_register_data_callback(bt_app_a2d_data_cb);
        esp_a2d_sink_init();

        /* initialize AVRCP controller */
        esp_avrc_ct_init();
        esp_avrc_ct_register_callback(bt_app_rc_ct_cb);

        /* set discoverable and connectable mode, wait to be connected */
        esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
        break;
    }
    default:
        ESP_LOGE(BT_AV_TAG, "%s unhandled evt %d", __func__, event);
        break;
    }
}
