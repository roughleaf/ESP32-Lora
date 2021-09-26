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

    AppTimer.Init(0, 0, 1, &apptimer_event_handler);
    Spi_3.Init(SPI3_HOST, spi_3_miso, spi_3_mosi, spi_3_sclk);
    Lora.SpiSetup(&Spi_3, lora_ss_pin, lora_reset_pin);
    Lora.IrqEnable(true);
    Lora.IrqSetup(GPIO_NUM_2, &lora_event_handler);
    Lora.Init();
    Wifi.Init();
    SntpTime.Init();
    //Lora.Listen();

    std::cout << "Mac Address: " << Wifi.get_mac() << std::endl;

    return status;
}

void Main::run(void)
{
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    //ESP_LOGI(LOG_TAG, "Transmitting byte on LORA");
    Lora.TransmitByte('T');

    //ESP_LOGI(LOG_TAG, testMac.get_mac());

    //std::cout << "Lora SX1278 Revision  : " << (int)Lora.ReadRegister(LORA::RegVersion1) << '\n';
    //std::cout << "Lora Spi Handle       : " << (int)Lora.GetSpiHandle() << '\n';
}
