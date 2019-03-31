//Example: Theremin

#include "Audio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "VL53L1X.h"
#include "arduino.h"
#include <Wire.h>

AudioControlI2S          i2s;
AudioControlPCM3060      pcm3060;
AudioControlAFSTwoTwo    afs22;

// GUItool: begin automatically generated code
AudioInputI2S            i2sInput1;      //xy=215,226
AudioEffectCalibration   calibrationInL;   //xy=374,212
AudioEffectCalibration   calibrationInR; //xy=375,246
AudioSynthWaveformSine   sine1;          //xy=586.0000152587891,224.3333339691162
AudioEffectEnvelope      envelope1;      //xy=651,359
AudioEffectCalibration   calibrationOutR; //xy=809,245
AudioEffectCalibration   calibrationOutL; //xy=811,211
AudioOutputI2S           i2sOutput1;     //xy=985,229
AudioConnection          patchCord1(i2sInput1, 0, calibrationInL, 0);
AudioConnection          patchCord2(i2sInput1, 1, calibrationInR, 0);
AudioConnection          patchCord3(sine1, envelope1);
AudioConnection          patchCord4(envelope1, calibrationOutR);
AudioConnection          patchCord5(envelope1, calibrationOutL);
AudioConnection          patchCord6(calibrationOutR, 0, i2sOutput1, 1);
AudioConnection          patchCord7(calibrationOutL, 0, i2sOutput1, 0);
// GUItool: end automatically generated code

void audioTask( void * parameter )
{
  AudioStream *p;  
  for(;;){
    p->update_all();    
  }
}

// void calibrationTask (void * parameter)
// {
//   static const char *TAG = "CalibrationTask";
//   const int delayMs = 100;
//   float in0V, in5V, out0V, out5V;
//   calibrationInL.enableAverage();

//   //Input L calibration
//   calibrationInL.inputNormal();
//   ESP_LOGI(TAG, "Setting left input to 0V.");
//   afs22.setLeftInputChannelTo0V();
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   in0V = calibrationInL.getInputAverage(); 
//   ESP_LOGI(TAG, "Setting left input to 5V.");
//   afs22.setLeftInputChannelTo5V();
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   in5V = calibrationInL.getInputAverage();
//   ESP_LOGI(TAG, "Left input 0V: %.4f, 5V: %.4f", in0V * 10, in5V * 10);
//   if(in0V < 0.05 && in0V > -0.05 && in5V > 0.45 && in5V < 0.55)
//     calibrationInL.calibrate(0, in0V, 0.5, in5V);

//   ESP_LOGI(TAG, "Setting left input to 0V.");
//   afs22.setLeftInputChannelTo0V();
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   in0V = calibrationInL.getOutputAverage(); 
//   ESP_LOGI(TAG, "Setting left input to 5V.");
//   afs22.setLeftInputChannelTo5V();
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   in5V = calibrationInL.getOutputAverage();
//   ESP_LOGI(TAG, "Left input 0V: %.4f, 5V: %.4f", in0V * 10, in5V * 10);
//   if(in0V < 0.001 && in0V > -0.001 && in5V > 0.499 && in5V < 0.501)
//     ESP_LOGI(TAG, "Input L calibration confirmed.");
//   else
//     ESP_LOGI(TAG, "Input L calibration failed.");
  
//   //Output L calibration
//   afs22.setLeftInputChannelToOutL();
//   ESP_LOGI(TAG, "Setting left input to Out L at 0V.");
//   calibrationOutL.inputDC(0.0);
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   out0V = calibrationInL.getOutputAverage();
//   ESP_LOGI(TAG, "Setting left input to Out L at 5V.");
//   calibrationOutL.inputDC(0.5);
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   out5V = calibrationInL.getOutputAverage();
//   ESP_LOGI(TAG, "Left output 0V: %.4f, 5V: %.4f", out0V * 10, out5V * 10);
//   if(out0V < 0.05 && out0V > -0.05 && out5V > 0.45 && out5V < 0.55) 
//     calibrationOutL.calibrate(0, out0V, 0.5, out5V);

//   ESP_LOGI(TAG, "Setting left input to Out L at 0V.");
//   afs22.setLeftInputChannelToOutL();
//   calibrationOutL.inputDC(0.0);
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   out0V = calibrationInL.getOutputAverage();
//   ESP_LOGI(TAG, "Setting left input to Out L at 5V.");
//   calibrationOutL.inputDC(0.5);
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   out5V = calibrationInL.getOutputAverage();
//   ESP_LOGI(TAG, "Left output 0V: %.4f, 5V: %.4f", out0V * 10, out5V * 10);
//   if(out0V < 0.001 && out0V > -0.001 && out5V > 0.499 && out5V < 0.501) 
//     ESP_LOGI(TAG, "Output L calibration confirmed.");
//   else
//     ESP_LOGI(TAG, "Output L calibration failed.");

//   calibrationOutL.inputNormal();
//   calibrationInL.disableAverage();
//   calibrationInR.enableAverage();

//   //Input R calibration
//   calibrationInR.inputNormal();
//   ESP_LOGI(TAG, "Setting right input to 0V.");
//   afs22.setRightInputChannelTo0V();
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   in0V = calibrationInR.getInputAverage(); 
//   ESP_LOGI(TAG, "Setting right input to 5V.");
//   afs22.setRightInputChannelTo5V();
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   in5V = calibrationInR.getInputAverage();
//   ESP_LOGI(TAG, "Right input 0V: %.4f, 5V: %.4f", in0V * 10, in5V * 10);
//   if(in0V < 0.05 && in0V > -0.05 && in5V > 0.45 && in5V < 0.55) 
//     calibrationInR.calibrate(0, in0V, 0.5, in5V);

//   ESP_LOGI(TAG, "Setting right input to 0V.");
//   afs22.setRightInputChannelTo0V();
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   in0V = calibrationInR.getOutputAverage(); 
//   ESP_LOGI(TAG, "Setting right input to 5V.");
//   afs22.setRightInputChannelTo5V();
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   in5V = calibrationInR.getOutputAverage();
//   ESP_LOGI(TAG, "Right input 0V: %.4f, 5V: %.4f", in0V * 10, in5V * 10);
//   if(in0V < 0.001 && in0V > -0.001 && in5V > 0.499 && in5V < 0.501)
//     ESP_LOGI(TAG, "Input R calibration confirmed.");
//   else
//     ESP_LOGI(TAG, "Input R calibration failed.");

//   //Output R calibration
//   afs22.setRightInputChannelToOutR();
//   ESP_LOGI(TAG, "Setting right input to Out R at 0V.");
//   calibrationOutR.inputDC(0.0);
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   out0V = calibrationInR.getOutputAverage();
//   ESP_LOGI(TAG, "Setting right input to Out R at 5V.");
//   calibrationOutR.inputDC(0.5);
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   out5V = calibrationInR.getOutputAverage();
//   ESP_LOGI(TAG, "Right output 0V: %.4f, 5V: %.4f", out0V * 10, out5V * 10);
//   if(out0V < 0.05 && out0V > -0.05 && out5V > 0.45 && out5V < 0.55) 
//     calibrationOutR.calibrate(0, out0V, 0.5, out5V);

//   afs22.setRightInputChannelToOutR();
//   ESP_LOGI(TAG, "Setting right input to Out R at 0V.");
//   calibrationOutR.inputDC(0.0);
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   out0V = calibrationInR.getOutputAverage();
//   ESP_LOGI(TAG, "Setting right input to Out R at 5V.");
//   calibrationOutR.inputDC(0.5);
//   vTaskDelay(delayMs/portTICK_RATE_MS);
//   out5V = calibrationInR.getOutputAverage();
//   ESP_LOGI(TAG, "Right output 0V: %.4f, 5V: %.4f", out0V * 10, out5V * 10);
//   if(out0V < 0.001 && out0V > -0.001 && out5V > 0.499 && out5V < 0.501) 
//     ESP_LOGI(TAG, "Output R calibration confirmed.");
//   else
//     ESP_LOGI(TAG, "Output R calibration failed.");

//   calibrationOutR.inputNormal();
//   calibrationInR.disableAverage();
//   afs22.setInputsToNormal();
//   ESP_LOGI(TAG, "Inputs set to input pins.");

//   vTaskDelete( NULL );
// }

VL53L1X sensor;
bool noteOn = false;
void thereminTask(void * parameter)
{
  static const char *TAG = "ThereminTask";
  //static const uint8_t adr = 0x29;
  vTaskDelay(5000/portTICK_RATE_MS);
  ESP_LOGI(TAG, "Starting theremin...");
  //pinMode(27, OUTPUT);
  //digitalWrite(27, HIGH);

  
  Wire.begin(18, 19);
  Wire.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    ESP_LOGI(TAG, "Failed to detect and initialize sensor!");
    //Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }
  
  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // medium and long distance modes. See the VL53L1X datasheet for more
  // information on range and timing limits.
  sensor.setDistanceMode(VL53L1X::Short);
  sensor.setMeasurementTimingBudget(20000);

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  sensor.startContinuous(20);

  for(;;){
    //vTaskDelay(5/portTICK_RATE_MS);
    uint16_t distance = sensor.read();
    if (sensor.timeoutOccurred()) { ESP_LOGI(TAG, "Sensor timeout!"); }
    else{
      if(sensor.ranging_data.range_status == 0){
        if(distance < 300){
          sine1.frequency(220 + distance);
          if(!noteOn){
            noteOn = true;
            envelope1.noteOn();
          }
        }
      }
      else{
        if(noteOn && envelope1.isSustain()){
          noteOn = false;
          envelope1.noteOff();
        }
      }
    }
  }
}

extern "C" {
   void app_main();
}

void app_main()
{
    initArduino();
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

    sine1.frequency(260);
    sine1.amplitude(0.5);
 
    xTaskCreatePinnedToCore(audioTask, "AudioTask", 10000, NULL, configMAX_PRIORITIES - 1, NULL, 1);
    //xTaskCreatePinnedToCore(calibrationTask, "CalibrationTask", 2000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(thereminTask, "ThereminTask", 2000, NULL, 1, NULL, 1);
}