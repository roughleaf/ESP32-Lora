#include "AppTimer.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"

#define TIMER_DIVIDER (16)                           //  Hardware timer clock divider
#define TIMER_SCALE (TIMER_BASE_CLK / TIMER_DIVIDER) // convert counter value to seconds

namespace TIMER
{
    ESP_EVENT_DEFINE_BASE(TIMER_EVENTS);

    bool AppTimer::apptimer_isr_callback(void *args)
    {
        //ESP_LOGI(_log_tag, "AppTimer Callback ISR called");
        esp_event_isr_post(TIMER_EVENTS, TIMER_EVENT_ISR, NULL, 0, NULL);
        return true;
    }

    esp_err_t AppTimer::Init(int group, int timer, int timer_interval_ms, esp_event_handler_t timer_e_h)
    {
        esp_err_t status{ESP_OK};

        _group = (timer_group_t)group;
        _timer = (timer_idx_t)timer;

        timer_config.alarm_en = TIMER_ALARM_EN;
        timer_config.counter_en = TIMER_PAUSE;
        timer_config.intr_type = TIMER_INTR_LEVEL;
        timer_config.counter_dir = TIMER_COUNT_UP;
        timer_config.auto_reload = TIMER_AUTORELOAD_EN;
        timer_config.divider = 16;

        status = timer_init(_group, _timer, &timer_config);
        ESP_LOGI(_log_tag, "timer_init:");

        status |= timer_set_counter_value(_group, _timer, 0);
        ESP_LOGI(_log_tag, "timer_set_counter_value");

        status |= timer_set_alarm_value(_group, _timer, timer_interval_ms * TIMER_SCALE);
        ESP_LOGI(_log_tag, "timer_set_alarm_value");

        status |= timer_enable_intr(_group, _timer);
        ESP_LOGI(_log_tag, "timer_enable_intr");

        status |= timer_isr_callback_add(_group, _timer, apptimer_isr_callback, nullptr, 0);
        ESP_LOGI(_log_tag, "timer_isr_callback_add");

        status |= esp_event_handler_instance_register(TIMER_EVENTS, TIMER_EVENT_ISR, timer_e_h, 0, NULL);

        status |= timer_start(_group, _timer);
        ESP_LOGI(_log_tag, "timer_start");

        return status;
    }

    esp_err_t AppTimer::Start(void)
    {
        return timer_start(_group, _timer);
    }

    esp_err_t AppTimer::Stop(void)
    {
        return timer_pause(_group, _timer);
    }

} // namespace TIMER