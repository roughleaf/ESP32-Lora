#include "main.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"
#define LOG_TAG "MAIN"

static Main my_main;

extern "C" void app_main(void)
{
    ESP_LOGI(LOG_TAG, "Creating default even loop");
    ESP_ERROR_CHECK(esp_event_loop_create_default()); // Create even handler

    ESP_LOGI(LOG_TAG, "Initialising NVS");
    ESP_ERROR_CHECK(nvs_flash_init());

    ESP_ERROR_CHECK(my_main.setup());

    while (true)
    {
        my_main.run();
    }
}

esp_err_t Main::setup(void)
{
    esp_err_t status{ESP_OK};
    ESP_LOGI(LOG_TAG, "Setup");

    AppTimer.Init(0, 0, 1, &apptimer_event_handler);
    Spi_3.Init(SPI3_HOST, spi_3_miso, spi_3_mosi, spi_3_sclk);
    Lora.SpiSetup(&Spi_3, lora_ss_pin, lora_reset_pin);
    Wifi.Init();
    SntpTime.Init();

    std::cout << "Mac Address: " << Wifi.get_mac() << std::endl;

    return status;
}

void Main::run(void)
{
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    //ESP_LOGI(LOG_TAG, testMac.get_mac());
    //std::cout << "Mac Address: " << Wifi.get_mac() << std::endl;
    //LoraDev.WriteRegister(0x01, 0x0B);
    //std::cout << "Lora SX1278 Operating Mode: " << (int)Lora.ReadRegister(LORA::RegOpMode) << std::endl;
    //std::cout << "Lora SX1278 Revision  : " << (int)Lora.ReadRegister(LORA::RegVersion1) << std::endl;
}

void Main::apptimer_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    //TODO change to switch/case

    // Wifi LED toggle between red and orange
    if (WIFI::Wifi::state_e::READY_TO_CONNECT == my_main.Wifi.get_state() || WIFI::Wifi::state_e::DISCONNECTED == my_main.Wifi.get_state())
    {
        ESP_LOGI(LOG_TAG, "Trying to connect");
        my_main.led1Red.On();
        my_main.led1Green.Toggle();
        my_main.Wifi.Begin();
    }

    else if (WIFI::Wifi::state_e::CONNECTING == my_main.Wifi.get_state())
    {
        my_main.led1Red.On();
        my_main.led1Green.Toggle();
        ESP_LOGI(LOG_TAG, "Trying to connect");
    }

    // Wifi LED toggle between green and orange
    else if (WIFI::Wifi::state_e::WAITING_FOR_IP == my_main.Wifi.get_state())
    {
        ESP_LOGI(LOG_TAG, "Waiting for IP");
        my_main.led1Red.Toggle();
        my_main.led1Green.On();
    }

    // Wifi Status LED RED and TODO Error handling
    else if (WIFI::Wifi::state_e::ERROR == my_main.Wifi.get_state())
    {
        ESP_LOGI(LOG_TAG, "Error");
        my_main.led1Red.On();
        my_main.led1Green.Off();
        //my_main.Wifi.Begin();
    }

    // Wifi Status LED Green
    else if (WIFI::Wifi::state_e::CONNECTED == my_main.Wifi.get_state())
    {
        ESP_LOGI(LOG_TAG, "Connected");
        my_main.led1Red.Off();
        my_main.led1Green.On();
    }

    // Wifi LED Orange
    else
    {
        if (WIFI::Wifi::state_e::NOT_INITIALISED == my_main.Wifi.get_state())
        {
            ESP_LOGI(LOG_TAG, "Not Initialised State");
        }
        if (WIFI::Wifi::state_e::INITIALISED == my_main.Wifi.get_state())
        {
            ESP_LOGI(LOG_TAG, "Initialised State");
        }
        else
        {
            ESP_LOGI(LOG_TAG, "Unhandeled State");
        }
        my_main.led1Red.On();
        my_main.led1Green.On();
    }
    //my_main.led.Toggle();
}