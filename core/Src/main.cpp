#include "main.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"
#define LOG_TAG "MAIN"

Main App;

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

    AppTimer.init(0, 0, 1, &apptimer_event_handler);
    Spi_3.init(SPI3_HOST, spi_3_miso, spi_3_mosi, spi_3_sclk);
    Lora.spiSetup(&Spi_3, lora_ss_pin, lora_reset_pin);
    Lora.irqEnable(true);
    Lora.irqSetup(GPIO_NUM_2, &lora_event_handler);
    Lora.init(434E6);
    Wifi.init();
    SntpTime.init();
    Lora.listen();

    std::cout << "Mac Address: " << Wifi.get_mac() << std::endl;

    return status;
}

void Main::run(void)
{
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    //ESP_LOGI(LOG_TAG, "Transmitting byte on LORA");
    //Lora.transmitByte('T');

    //ESP_LOGI(LOG_TAG, testMac.get_mac());

    //std::cout << "Lora SX1278 Revision  : " << (int)Lora.readRegister(LORA::RegVersion1) << '\n';
    //std::cout << "Lora Spi Handle       : " << (int)Lora.getSpiHandle() << '\n';
}
