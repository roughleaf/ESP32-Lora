#include "main.h"

void Main::apptimer_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
    {
        App.wifiState = App.Wifi.get_state();

        switch (App.wifiState)
        {
        case WIFI::Wifi::state_e::READY_TO_CONNECT:
        case WIFI::Wifi::state_e::DISCONNECTED:
            ESP_LOGI(AppTimer_tag, "Trying to connect");
            App.led1Red.On();
            App.led1Green.Toggle();
            App.Wifi.Begin();
            break;
        case WIFI::Wifi::state_e::CONNECTING:
            App.led1Red.On();
            App.led1Green.Toggle();
            ESP_LOGI(AppTimer_tag, "Trying to connect");
            break;
        case WIFI::Wifi::state_e::WAITING_FOR_IP:
            ESP_LOGI(AppTimer_tag, "Waiting for IP");
            App.led1Red.Toggle();
            App.led1Green.On();
            break;
        case WIFI::Wifi::state_e::ERROR:
            ESP_LOGI(AppTimer_tag, "Error");
            App.led1Red.On();
            App.led1Green.Off();
            break;
        case WIFI::Wifi::state_e::CONNECTED:
            ESP_LOGI(AppTimer_tag, "Connected");
            App.led1Red.Off();
            App.led1Green.On();
            break;
        case WIFI::Wifi::state_e::NOT_INITIALISED:
            ESP_LOGI(AppTimer_tag, "Not Initialised State");
            break;
        case WIFI::Wifi::state_e::INITIALISED:
            ESP_LOGI(AppTimer_tag, "Initialised State");
            break;
        }
    }

    void IRAM_ATTR Main::lora_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
    {
        if (LORA::Lora::lora_interrupt_t::tx_int == App.Lora.GetInterruptMode())
        {
            ESP_LOGI(Lora_tag, "Lora TX Event Triggered");
            App.Lora.ClearIrqFlags();
            App.LoraLedGreen.Toggle();
            ESP_LOGI(Lora_tag, "IRQ flags cleared");
        }
        if (LORA::Lora::lora_interrupt_t::rx_int == App.Lora.GetInterruptMode())
        {
            ESP_LOGI(Lora_tag, "Lora RX Event Triggered");
            App.Lora.ClearIrqFlags();
            App.LoraLedGreen.Toggle();
            ESP_LOGI(Lora_tag, "IRQ flags cleared");
        }
    }

    void Main::lora_task(void *arg)
    {
        //uint32_t io_num;
        for (;;)
        {
            ESP_LOGI(Lora_tag, "Lora task doing stuff");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            //App.LoraLedGreen.Toggle();
            //App.Lora.ReadRegister(LORA::RegVersion1);
            //if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY))
            //{
            //    printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
            //}
            std::cout << "Lora Spi Handle in Task: " << (int)App.Lora.GetSpiHandle() << '\n';
        }
    }