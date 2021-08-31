#pragma once

#include "driver/timer.h"

namespace TIMER
{
    class AppTimer
    {
        public:
        esp_err_t Init(void);
        esp_err_t Start(void);
        esp_err_t Stop(void);

        private:
        timer_config_t timer_config {};
        static bool apptimer_isr_callback(void* args);  // TODO Trigger event or xQueueSendFromISR. Try Events
    }; // Class AppTimer
} // namespace TIMER
