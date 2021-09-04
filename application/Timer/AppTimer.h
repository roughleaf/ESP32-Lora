#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_event.h"
#include "driver/timer.h"

namespace TIMER
{
    ESP_EVENT_DECLARE_BASE(TIMER_EVENTS);

    class AppTimer
    {
        constexpr static const char *_log_tag{"AppTimer"};

        public:
        esp_err_t Init(int group, int timer, int timer_interval_ms, esp_event_handler_t timer_e_h);
        esp_err_t Start(void);
        esp_err_t Stop(void);

        private:
        timer_config_t timer_config {};
        timer_group_t _group {};
        timer_idx_t _timer {};

        enum
        {                           // declaration of the specific events under the timer event family
            TIMER_EVENT_STARTED,    // raised when the timer is first started
            TIMER_EVENT_ISR,        // raised when timer alarm triggers interrupt
            TIMER_EVENT_STOPPED     // raised when the timer has been stopped
        };

        static bool apptimer_isr_callback(void* args);  // TODO Trigger event or xQueueSendFromISR. Try Events
        //static void* apptimer_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data);
    }; // Class AppTimer
} // namespace TIMER
