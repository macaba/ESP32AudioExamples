//Example: Sine

#include "Audio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

AudioControlI2S          i2s;
AudioControlPCM3060      pcm3060;

// GUItool: begin automatically generated code
AudioInputI2S            i2sInput1;           //xy=401,596
AudioOutputI2S           i2sOutput1;           //xy=750,560
AudioPlaySdMmcWav        playSdMmc1;
AudioConnection          patchCord3(playSdMmc1, 0, i2sOutput1, 0);
AudioConnection          patchCord4(playSdMmc1, 1, i2sOutput1, 1);
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
    vTaskDelay(1000/portTICK_RATE_MS);
    pcm3060.init();

    playSdMmc1.loadFile("/sdcard/DR660 Synthbass.wav");
    xTaskCreatePinnedToCore(audioTask, "AudioTask", 10000, NULL, configMAX_PRIORITIES - 1, NULL, 1);
}