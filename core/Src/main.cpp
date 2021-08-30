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

    while(true)
    {
        my_main.run();
    }
}

esp_err_t Main::setup(void)
{ 
    esp_err_t status{ESP_OK};
    ESP_LOGI(LOG_TAG, "Setup");

    Spi_3.Init(SPI3_HOST, spi_3_miso, spi_3_mosi, spi_3_sclk);
    Lora.SpiSetup(&Spi_3, lora_ss_pin, lora_reset_pin);
    Wifi.Init();
    SntpTime.Init();

    std::cout << "Mac Address: " << Wifi.get_mac() << std::endl;

    return status;
}
    
void Main::run(void)
{
    led.High();
    //ESP_LOGI(LOG_TAG, "LED on");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    //ESP_LOGI(LOG_TAG, "LED off");
    led.Low();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    //ESP_LOGI(LOG_TAG, testMac.get_mac());
    //std::cout << "Mac Address: " << Wifi.get_mac() << std::endl;
    //LoraDev.WriteRegister(0x01, 0x0B);
    std::cout << "Lora SX1278 Operating Mode: " << (int)Lora.ReadRegister(LORA::RegOpMode) << std::endl;
    std::cout << "Lora SX1278 Revision  : " << (int)Lora.ReadRegister(LORA::RegVersion1) << std::endl;
}