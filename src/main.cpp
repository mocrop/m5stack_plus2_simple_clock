#include <Arduino.h>
#include <M5Unified.h>
#include "watch.hpp"

Watch watch;

void setup() {
  M5.begin();
  M5.Lcd.setBrightness(1);
}

void loop() {
  M5.update();
  if(M5.BtnB.wasClicked()) {
    watch.start_setting();
    sleep(100);
  } else if(M5.BtnPWR.wasClicked()) {
    watch.set_time(-1);
    sleep(100);
  } else if(M5.BtnA.wasClicked()) {
    watch.set_time();
    sleep(100);
  } 
  watch.update();
}

