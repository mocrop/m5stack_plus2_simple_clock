#include "watch.hpp"
#include <Arduino.h>

void Watch::update()
{
  static long currentMillis = 0;
  if(millis() - currentMillis < 1000) {
    return;
  }
  setting_timeout--;
  if(setting_timeout <= 0) {
    setting = false;
  }
  currentMillis = millis();
  static float x, y, z;
  float tx, ty, tz;
  static int rotation = 0;
  static int cur_roataion = 0;
  static int sleep_cnt = 0;
  M5.Imu.getAccel(&tx, &ty, &tz);
  tx = round(tx * 10);
  ty = round(ty * 10);
  tz = round(tz * 10);
  if(x != tx || y != ty || z != tz) {
    sleep_cnt = 0;
    x = tx;
    y = ty;
    z = tz;
  } else {
    sleep_cnt++;
  }
  if(y > 7) {
    cur_roataion = 0;
  }else if(x > 7) {
    cur_roataion = 1;
  } else if(y < -7) {
    cur_roataion = 2;
  } else if(x < -7) {
    cur_roataion = 3;
  } 
  
  if(rotation != cur_roataion) {
    M5.Lcd.clear();
    rotation = cur_roataion;
    M5.Lcd.setRotation(rotation);
  }

  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(2);
  if(power_level != M5.Power.getBatteryLevel()) {
    power_level = M5.Power.getBatteryLevel();
    M5.Lcd.fillRect(M5.Lcd.getCursorX(), M5.Lcd.getCursorY(), M5.Lcd.width(), M5.Lcd.fontHeight(), BLACK);
  } 
  M5.Lcd.printf("Bat: %d%%\n", M5.Power.getBatteryLevel());
  auto cur_dt = M5.Rtc.getDateTime();
  int index = 0;
  // 年月日
  for(index = 0; index < 3; index++) {
    printTime(index, cur_dt);
    if( index < 2) {
      M5.Lcd.print("/");
    }
  }
  const char* week_days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  M5.Lcd.printf(" %s", week_days[cur_dt.date.weekDay]);
  // 时分秒
  M5.Lcd.print("\n");
  // 横向
  if(cur_roataion % 2 != 0) {
    M5.Lcd.print("\n\n");
    M5.Lcd.setTextSize(5);
    for(; index < 6; index++) {
      printTime(index, cur_dt);
      if( index < 5) {
        M5.Lcd.print(":");
      }
    }   
  } else {
      // 竖向
      M5.Lcd.setTextSize(8);
      for(; index < 6; index++) {
        M5.Lcd.setCursor(20, M5.Lcd.getCursorY());
        printTime(index, cur_dt);
        M5.Lcd.print("\n");
      }
  }        
  // 自动关机
  if(sleep_cnt > 60) {
    M5.Speaker.tone(5000, 200);
    M5.Power.powerOff();
  }
}
// 开启时间设置，选择设置项
void Watch::start_setting()
{
    if(!setting) {
        selected = 0;
    } else {
        selected = (selected + 1) % 6;
    }
    setting = true;
    setting_timeout = 30; // 30秒后退出设置模式
}
// 设置时间
void Watch::set_time(int delta)
{
    switch (selected)
    {
    case 0:
        dt.date.year += delta;
        if(dt.date.year > 2099) {
            dt.date.year = 1900;
        } else if(dt.date.year < 1900) {
            dt.date.year = 2099;
        }
        break;
    case 1:
        dt.date.month += delta;
        if(dt.date.month > 12) {
            dt.date.month = 1;
        } else if(dt.date.month < 1) {
            dt.date.month = 12;
        }
        break;
    case 2:
        dt.date.date += delta;
        if(dt.date.date > 31) {
            dt.date.date = 1;
        } else if(dt.date.date < 1) {
            dt.date.date = 31;
        }
        break;
    case 3:
        dt.time.hours += delta;
        if(dt.time.hours > 23) {
            dt.time.hours = 0;
        } else if(dt.time.hours < 0) {
            dt.time.hours = 23;
        }
        break;
    case 4: 
        dt.time.minutes += delta;
        if(dt.time.minutes > 59) {
            dt.time.minutes = 0;
        } else if(dt.time.minutes < 0) {
            dt.time.minutes = 59;
        }
        break;
    case 5:   
        dt.time.seconds = 0;
        break;  
    default:
        break;
    }
    M5.Rtc.setDateTime(dt);
}

void Watch::printTime(int index, m5::rtc_datetime_t& cur_dt)
{
    if(index == selected && setting) {
        M5.Lcd.setTextColor(YELLOW);
    } 
    switch (index)
    {
    case 0:
        if(cur_dt.date.year != dt.date.year) {
          dt.date.year = cur_dt.date.year;
          clear_time_display(4);
        }
        M5.Lcd.printf("%04d",
                    dt.date.year);
        break;
    case 1:
        if(cur_dt.date.month != dt.date.month) {
          dt.date.month = cur_dt.date.month;
          clear_time_display(2);
        }
        M5.Lcd.printf("%02d",
                    dt.date.month);
        break;
        case 2:
        if(cur_dt.date.date != dt.date.date) {
          dt.date.date = cur_dt.date.date;
          clear_time_display(2);
        }
        M5.Lcd.printf("%02d",
                    dt.date.date);
        break;
    case 3:
        if(cur_dt.time.hours != dt.time.hours) {
          dt.time.hours = cur_dt.time.hours;
          clear_time_display(2);
        }
        M5.Lcd.printf("%02d",
                    dt.time.hours);
        break;
        case 4: 
        if(cur_dt.time.minutes != dt.time.minutes) {
          dt.time.minutes = cur_dt.time.minutes;
          clear_time_display(2);
        }
        M5.Lcd.printf("%02d",
                    dt.time.minutes);
        break;
    case 5:
        if(cur_dt.time.seconds != dt.time.seconds) {
          dt.time.seconds = cur_dt.time.seconds;
          clear_time_display(2);
        }
        M5.Lcd.printf("%02d",
                    dt.time.seconds);
        break;  
    default:
        break;
    }
    M5.Lcd.setTextColor(WHITE);
}

void Watch::clear_time_display(int size)
{
  M5.Lcd.fillRect(M5.Lcd.getCursorX(), M5.Lcd.getCursorY(), M5.Lcd.fontWidth()*size, M5.Lcd.fontHeight(), BLACK);
}
