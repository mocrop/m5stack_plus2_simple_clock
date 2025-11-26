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
  if(M5.BtnB.wasPressed()) {
    watch.start_setting();
  } else if(M5.BtnA.wasPressed()) {
    watch.set_time();
  }
  watch.update();
}

