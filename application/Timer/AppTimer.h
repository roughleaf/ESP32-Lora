#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/timer.h"

namespace TIMER
{
    class AppTimer
    {
        constexpr static const char *_log_tag{"AppTimer"};

        public:
        esp_err_t Init(int group, int timer, int timer_interval_ms);
        esp_err_t Start(void);
        esp_err_t Stop(void);

        private:
        timer_config_t timer_config {};
        timer_group_t _group {};
        timer_idx_t _timer {};
        static bool apptimer_isr_callback(void* args);  // TODO Trigger event or xQueueSendFromISR. Try Events
    }; // Class AppTimer
} // namespace TIMER
