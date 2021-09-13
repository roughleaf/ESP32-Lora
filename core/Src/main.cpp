#include "main.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"
#define LOG_TAG "MAIN"

static Main App;

extern "C" void app_main(void)
{
    ESP_LOGI(LOG_TAG, "Creating default even loop");
    ESP_ERROR_CHECK(esp_event_loop_create_default()); // Create even handler

    ESP_LOGI(LOG_TAG, "Initialising NVS");
    ESP_ERROR_CHECK(nvs_flash_init());

    ESP_ERROR_CHECK(App.setup());

    while (true)
    {
        App.run();
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
    App.wifiState = App.Wifi.get_state();

    switch (App.wifiState)
    {
    case WIFI::Wifi::state_e::READY_TO_CONNECT:
    case WIFI::Wifi::state_e::DISCONNECTED:
        ESP_LOGI(LOG_TAG, "Trying to connect");
        App.led1Red.On();
        App.led1Green.Toggle();
        App.Wifi.Begin();
        break;
    case WIFI::Wifi::state_e::CONNECTING:
        App.led1Red.On();
        App.led1Green.Toggle();
        ESP_LOGI(LOG_TAG, "Trying to connect");
        break;
    case WIFI::Wifi::state_e::WAITING_FOR_IP:
        ESP_LOGI(LOG_TAG, "Waiting for IP");
        App.led1Red.Toggle();
        App.led1Green.On();
        break;
    case WIFI::Wifi::state_e::ERROR:
        ESP_LOGI(LOG_TAG, "Error");
        App.led1Red.On();
        App.led1Green.Off();
        break;
    case WIFI::Wifi::state_e::CONNECTED:
        ESP_LOGI(LOG_TAG, "Connected");
        App.led1Red.Off();
        App.led1Green.On();
        break;
    case WIFI::Wifi::state_e::NOT_INITIALISED:
        ESP_LOGI(LOG_TAG, "Not Initialised State");
        break;
    case WIFI::Wifi::state_e::INITIALISED:
        ESP_LOGI(LOG_TAG, "Initialised State");
        break;
    }    
}