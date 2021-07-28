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
    status |= led.Init(GPIO_NUM_14);

    return status;
}
    
void Main::run(void)
{
    std::string tst = "Test String";

    ESP_LOGI(LOG_TAG, "Hello World");

    led.High();
    ESP_LOGI(LOG_TAG, "LED on");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    ESP_LOGI(LOG_TAG, "LED off");
    led.Low();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    //ESP_LOGI(LOG_TAG, testMac.get_mac());
    std::cout << "Mac Address: " << testMac.get_mac() << std::endl;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}