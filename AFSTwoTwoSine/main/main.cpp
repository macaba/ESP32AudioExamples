//Example: Sine

#include "Audio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

AudioControlI2S          i2s;
AudioControlPCM3060      pcm3060;
AudioControlAFSTwoTwo    afs22;

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=398,548
AudioSynthWaveformSine   sine2;
AudioInputI2S            i2sInput1;           //xy=401,596
AudioOutputI2S           i2sOutput1;           //xy=750,560
AudioEffectCalibration   calibrationOutL;
AudioEffectCalibration   calibrationOutR;
AudioEffectCalibration   calibrationInL;
AudioEffectCalibration   calibrationInR;
AudioConnection          patchCord1(sine1, calibrationOutL);
AudioConnection          patchCord2(sine2, calibrationOutR);
AudioConnection          patchCord3(calibrationOutL, 0, i2sOutput1, 0);
AudioConnection          patchCord4(calibrationOutR, 0, i2sOutput1, 1);
AudioConnection          patchCord5(i2sInput1, 0, calibrationInL, 0);
AudioConnection          patchCord6(i2sInput1, 1, calibrationInR, 0);
// GUItool: end automatically generated code

void audioTask( void * parameter )
{
  AudioStream *p;  
  for(;;){
    p->update_all();    
  }
}

void calibrationTask (void * parameter)
{
  static const char *TAG = "CalibrationTask";
  const int delayMs = 100;
  float in0V, in5V, out0V, out5V;
  calibrationInL.enableAverage();

  //Input L calibration
  calibrationInL.inputNormal();
  ESP_LOGI(TAG, "Setting left input to 0V.");
  afs22.setLeftInputChannelTo0V();
  vTaskDelay(delayMs/portTICK_RATE_MS);
  in0V = calibrationInL.getInputAverage(); 
  ESP_LOGI(TAG, "Setting left input to 5V.");
  afs22.setLeftInputChannelTo5V();
  vTaskDelay(delayMs/portTICK_RATE_MS);
  in5V = calibrationInL.getInputAverage();
  ESP_LOGI(TAG, "Left input 0V: %.4f, 5V: %.4f", in0V * 10, in5V * 10);
  if(in0V < 0.05 && in0V > -0.05 && in5V > 0.45 && in5V < 0.55)
    calibrationInL.calibrate(0, in0V, 0.5, in5V);

  ESP_LOGI(TAG, "Setting left input to 0V.");
  afs22.setLeftInputChannelTo0V();
  vTaskDelay(delayMs/portTICK_RATE_MS);
  in0V = calibrationInL.getOutputAverage(); 
  ESP_LOGI(TAG, "Setting left input to 5V.");
  afs22.setLeftInputChannelTo5V();
  vTaskDelay(delayMs/portTICK_RATE_MS);
  in5V = calibrationInL.getOutputAverage();
  ESP_LOGI(TAG, "Left input 0V: %.4f, 5V: %.4f", in0V * 10, in5V * 10);
  if(in0V < 0.001 && in0V > -0.001 && in5V > 0.499 && in5V < 0.501)
    ESP_LOGI(TAG, "Input L calibration confirmed.");
  else
    ESP_LOGI(TAG, "Input L calibration failed.");
  
  //Output L calibration
  afs22.setLeftInputChannelToOutL();
  ESP_LOGI(TAG, "Setting left input to Out L at 0V.");
  calibrationOutL.inputDC(0.0);
  vTaskDelay(delayMs/portTICK_RATE_MS);
  out0V = calibrationInL.getOutputAverage();
  ESP_LOGI(TAG, "Setting left input to Out L at 5V.");
  calibrationOutL.inputDC(0.5);
  vTaskDelay(delayMs/portTICK_RATE_MS);
  out5V = calibrationInL.getOutputAverage();
  ESP_LOGI(TAG, "Left output 0V: %.4f, 5V: %.4f", out0V * 10, out5V * 10);
  if(out0V < 0.05 && out0V > -0.05 && out5V > 0.45 && out5V < 0.55) 
    calibrationOutL.calibrate(0, out0V, 0.5, out5V);

  ESP_LOGI(TAG, "Setting left input to Out L at 0V.");
  afs22.setLeftInputChannelToOutL();
  calibrationOutL.inputDC(0.0);
  vTaskDelay(delayMs/portTICK_RATE_MS);
  out0V = calibrationInL.getOutputAverage();
  ESP_LOGI(TAG, "Setting left input to Out L at 5V.");
  calibrationOutL.inputDC(0.5);
  vTaskDelay(delayMs/portTICK_RATE_MS);
  out5V = calibrationInL.getOutputAverage();
  ESP_LOGI(TAG, "Left output 0V: %.4f, 5V: %.4f", out0V * 10, out5V * 10);
  if(out0V < 0.001 && out0V > -0.001 && out5V > 0.499 && out5V < 0.501) 
    ESP_LOGI(TAG, "Output L calibration confirmed.");
  else
    ESP_LOGI(TAG, "Output L calibration failed.");

  calibrationOutL.inputNormal();
  calibrationInL.disableAverage();
  calibrationInR.enableAverage();

  //Input R calibration
  calibrationInR.inputNormal();
  ESP_LOGI(TAG, "Setting right input to 0V.");
  afs22.setRightInputChannelTo0V();
  vTaskDelay(delayMs/portTICK_RATE_MS);
  in0V = calibrationInR.getInputAverage(); 
  ESP_LOGI(TAG, "Setting right input to 5V.");
  afs22.setRightInputChannelTo5V();
  vTaskDelay(delayMs/portTICK_RATE_MS);
  in5V = calibrationInR.getInputAverage();
  ESP_LOGI(TAG, "Right input 0V: %.4f, 5V: %.4f", in0V * 10, in5V * 10);
  if(in0V < 0.05 && in0V > -0.05 && in5V > 0.45 && in5V < 0.55) 
    calibrationInR.calibrate(0, in0V, 0.5, in5V);

  ESP_LOGI(TAG, "Setting right input to 0V.");
  afs22.setRightInputChannelTo0V();
  vTaskDelay(delayMs/portTICK_RATE_MS);
  in0V = calibrationInR.getOutputAverage(); 
  ESP_LOGI(TAG, "Setting right input to 5V.");
  afs22.setRightInputChannelTo5V();
  vTaskDelay(delayMs/portTICK_RATE_MS);
  in5V = calibrationInR.getOutputAverage();
  ESP_LOGI(TAG, "Right input 0V: %.4f, 5V: %.4f", in0V * 10, in5V * 10);
  if(in0V < 0.001 && in0V > -0.001 && in5V > 0.499 && in5V < 0.501)
    ESP_LOGI(TAG, "Input R calibration confirmed.");
  else
    ESP_LOGI(TAG, "Input R calibration failed.");

  //Output R calibration
  afs22.setRightInputChannelToOutR();
  ESP_LOGI(TAG, "Setting right input to Out R at 0V.");
  calibrationOutR.inputDC(0.0);
  vTaskDelay(delayMs/portTICK_RATE_MS);
  out0V = calibrationInR.getOutputAverage();
  ESP_LOGI(TAG, "Setting right input to Out R at 5V.");
  calibrationOutR.inputDC(0.5);
  vTaskDelay(delayMs/portTICK_RATE_MS);
  out5V = calibrationInR.getOutputAverage();
  ESP_LOGI(TAG, "Right output 0V: %.4f, 5V: %.4f", out0V * 10, out5V * 10);
  if(out0V < 0.05 && out0V > -0.05 && out5V > 0.45 && out5V < 0.55) 
    calibrationOutR.calibrate(0, out0V, 0.5, out5V);

  afs22.setRightInputChannelToOutR();
  ESP_LOGI(TAG, "Setting right input to Out R at 0V.");
  calibrationOutR.inputDC(0.0);
  vTaskDelay(delayMs/portTICK_RATE_MS);
  out0V = calibrationInR.getOutputAverage();
  ESP_LOGI(TAG, "Setting right input to Out R at 5V.");
  calibrationOutR.inputDC(0.5);
  vTaskDelay(delayMs/portTICK_RATE_MS);
  out5V = calibrationInR.getOutputAverage();
  ESP_LOGI(TAG, "Right output 0V: %.4f, 5V: %.4f", out0V * 10, out5V * 10);
  if(out0V < 0.001 && out0V > -0.001 && out5V > 0.499 && out5V < 0.501) 
    ESP_LOGI(TAG, "Output R calibration confirmed.");
  else
    ESP_LOGI(TAG, "Output R calibration failed.");

  calibrationOutR.inputNormal();
  calibrationInR.disableAverage();
  afs22.setInputsToNormal();
  ESP_LOGI(TAG, "Inputs set to input pins.");

  vTaskDelete( NULL );
}

extern "C" {
   void app_main();
}

void app_main()
{
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    AudioMemory(10);
    i2s.default_codec_rx_tx_24bit();
    vTaskDelay(1000/portTICK_RATE_MS);
    pcm3060.init();

    sine1.frequency(220);
    sine1.amplitude(0.1);
    sine2.frequency(440);
    sine2.amplitude(0.1);
 
    xTaskCreatePinnedToCore(audioTask, "AudioTask", 10000, NULL, configMAX_PRIORITIES - 1, NULL, 1);
    xTaskCreatePinnedToCore(calibrationTask, "CalibrationTask", 2000, NULL, 1, NULL, 1);
}