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
    vTaskDelay(100/portTICK_RATE_MS);
    afs22.init();
    pcm3060.init();

    sine1.frequency(220);
    sine1.amplitude(0.1);
    sine2.frequency(440);
    sine2.amplitude(0.1);
 
    xTaskCreatePinnedToCore(audioTask, "AudioTask", 10000, NULL, configMAX_PRIORITIES - 1, NULL, 1);
    afs22.calibrate(&calibrationInL, &calibrationOutL, &calibrationInR, &calibrationOutR);
}