#include <M5Unified.h>

class Watch {
    public:
        m5::rtc_datetime_t dt;
        int selected = 0;
        
        void update();
        void start_setting();
        void set_time();
    private:
        bool setting = false;
        int setting_timeout = 0;
        void printTime(int index);
};