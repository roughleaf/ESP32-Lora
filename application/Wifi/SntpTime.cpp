#include "SntpTime.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"

namespace SNTP
{

    //Statics
    std::chrono::_V2::system_clock::time_point Sntp::last_update{};
    Sntp::time_source_e Sntp::source{Sntp::time_source_e::TIME_SRC_UNKNOWN};
    bool Sntp::_running{false};

    void Sntp::callback_on_ntp_update(timeval *tv)
    {
        ESP_LOGI(_log_tag, "Time is %s", ascii_time_now()); // TODO remove extra newline
        // TODO update "last_update"
    }

    esp_err_t Sntp::Init(void)
    {
        if (!_running)
        {
            while (state_e::CONNECTED != Wifi::get_state())
            {
                vTaskDelay(pdMS_TO_TICKS(1000));
            }

            setenv("TZ", "GMT-2", 1); // South Africa Time (GMT+2)
            tzset();

            sntp_setoperatingmode(SNTP_OPMODE_POLL);
            sntp_setservername(0, "time.google.com");
            sntp_setservername(1, "pool.ntp.com");

            sntp_set_time_sync_notification_cb(&callback_on_ntp_update);
            sntp_set_sync_interval(60* 60 * 1000); // TODO change intervals to 1 hour

            sntp_init();

             ESP_LOGI(_log_tag, "SNTP Initialised");

            source = TIME_SRC_NTP;

            _running = true;
        }

        if (_running)
            return ESP_OK;
        return ESP_FAIL;
    }

    [[nodiscard]] const char *Sntp::ascii_time_now(void)
    {
        const std::time_t time_now{std::chrono::system_clock::to_time_t(time_point_now())};

        return std::asctime(std::localtime(&time_now));
    }

} // namespace SNTP