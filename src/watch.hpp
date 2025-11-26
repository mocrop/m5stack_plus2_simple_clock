#include <M5Unified.h>

class Watch {
    public:
        m5::rtc_datetime_t dt;
        int selected = 0;
        int32_t power_level = 0;
        void update();
        void start_setting();
        void set_time(int delta = 1);
    private:
        bool setting = false;
        int setting_timeout = 0;
        void printTime(int index, m5::rtc_datetime_t& cur_dt);
        void clear_time_display(int size = 2);
};