#include "Wifi.h"

#define LOG_TAG "myWIFI"

namespace WIFI
{
    // Statics
    char Wifi::mac_addr_cstr[]{};    ///< Buffer to hold MAC
    std::mutex Wifi::init_mutx{};    ///< Initialisation Mutex
    std::mutex Wifi::connect_mutx{}; ///< Connect Mutex
    std::mutex Wifi::state_mutx{};   ///< State change Mutex
    Wifi::state_e Wifi::_state{state_e::NOT_INITIALISED};
    wifi_init_config_t Wifi::wifi_init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    wifi_config_t Wifi::wifi_cfg{};

    constexpr static const char *ssid{"TestGuest"};
    constexpr static const char *password{"00000000"};

    // Wifi Contrustor
    Wifi::Wifi(void)
    {
        if (!mac_addr_cstr[0])
        {
            if (ESP_OK != _get_mac())
            {
                esp_restart();
            }
        }
    }

    void Wifi::wifi_event_handler(void *arg, esp_event_base_t event_base,
                                  int32_t event_id, void *event_data)
    {
        if (WIFI_EVENT == event_base)
        {
            const wifi_event_t event_type{static_cast<wifi_event_t>(event_id)};

            switch (event_type)
            {
            case WIFI_EVENT_STA_START:
            {
                std::lock_guard<std::mutex> state_guard(state_mutx);
                _state = state_e::READY_TO_CONNECT;
                ESP_LOGI(_log_tag, "Ready to connect");
                break;
            }

            case WIFI_EVENT_STA_CONNECTED:
            {
                std::lock_guard<std::mutex> state_guard(state_mutx);
                _state = state_e::WAITING_FOR_IP;
                ESP_LOGI(_log_tag, "Waiting for IP");
                break;
            }

            case WIFI_EVENT_STA_DISCONNECTED:
            {
                std::lock_guard<std::mutex> state_guard(state_mutx);
                _state = state_e::DISCONNECTED;
                ESP_LOGI(_log_tag, "Disconnected from AP");
                break;
            }

            default:
                // TODO STOP and DISCONNECTED, and others
                break;
            }
        }
    }

    void Wifi::ip_event_handler(void *arg, esp_event_base_t event_base,
                                int32_t event_id, void *event_data)
    {
        if (IP_EVENT == event_base)
        {
            const ip_event_t event_type{static_cast<ip_event_t>(event_id)};

            switch (event_type)
            {
            case IP_EVENT_STA_GOT_IP:
            {
                std::lock_guard<std::mutex> state_guard(state_mutx);
                _state = state_e::CONNECTED;
                ESP_LOGI(_log_tag, "Connected");
                break;
            }

            case IP_EVENT_STA_LOST_IP:
            {
                std::lock_guard<std::mutex> state_guard(state_mutx);
                if (state_e::DISCONNECTED != _state)
                {
                    _state = state_e::WAITING_FOR_IP;
                }
                ESP_LOGI(_log_tag, "Lost IP");
                break;
            }

            default:
                // TODO IP6
                break;
            }
        }
    }

    // This function will only be called from the AppTimer event handler
    esp_err_t Wifi::Begin(void)
    {
        std::lock_guard<std::mutex> connect_guard(connect_mutx);
        std::lock_guard<std::mutex> state_guard(state_mutx);

        esp_err_t status{ESP_OK};

        switch (_state)
        {
        case state_e::READY_TO_CONNECT:
        case state_e::DISCONNECTED:
            status = esp_wifi_connect();
            if (ESP_OK == status)
            {
                _state = state_e::CONNECTING;
            }
            break;
        case state_e::CONNECTING:
        case state_e::WAITING_FOR_IP:
        case state_e::CONNECTED:
            break;
        case state_e::NOT_INITIALISED:
        case state_e::INITIALISED:
        case state_e::ERROR:
            ESP_LOGI(LOG_TAG, "Wifi Begin Fail");
            status = ESP_FAIL;
            break;
        }
        return status;
    }

    esp_err_t Wifi::_init(void)
    {
        std::lock_guard<std::mutex> init_guard(init_mutx);
        std::lock_guard<std::mutex> state_guard(state_mutx);

        esp_err_t status{ESP_OK};

        if (state_e::NOT_INITIALISED == _state)
        {
            status |= esp_netif_init();
            if (ESP_OK == status)
            {
                const esp_netif_t *const p_netif = esp_netif_create_default_wifi_sta();

                if (!p_netif)
                {
                    status = ESP_FAIL;
                }
            }

            if (ESP_OK == status)
            {
                status = esp_wifi_init(&wifi_init_cfg);
            }

            if (ESP_OK == status)
            {
                status = esp_event_handler_instance_register(WIFI_EVENT,
                                                             ESP_EVENT_ANY_ID,
                                                             &wifi_event_handler,
                                                             nullptr,
                                                             nullptr);
            }

            if (ESP_OK == status)
            {
                status = esp_event_handler_instance_register(IP_EVENT,
                                                             ESP_EVENT_ANY_ID,
                                                             &ip_event_handler,
                                                             nullptr,
                                                             nullptr);
            }

            if (ESP_OK == status)
            {
                // TODO Keep track of Wifi Mode
                status = esp_wifi_set_mode(WIFI_MODE_STA);
            }

            if (ESP_OK == status)
            {
                memcpy(wifi_cfg.sta.ssid, ssid,
                       std::min(strlen(ssid), sizeof(wifi_cfg.sta.ssid)));

                memcpy(wifi_cfg.sta.password, password,
                       std::min(strlen(password), sizeof(wifi_cfg.sta.password)));

                wifi_cfg.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
                wifi_cfg.sta.pmf_cfg.capable = true;
                wifi_cfg.sta.pmf_cfg.required = false;

                status = esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg);
            }

            if (ESP_OK == status)
            {
                status = esp_wifi_start(); // start Wifi
            }

            if (ESP_OK == status)
            {
                _state = state_e::INITIALISED;
            }
        }

        else if (state_e::ERROR == _state)
        {
            _state = state_e::NOT_INITIALISED;
        }

        return status;
    }

    esp_err_t Wifi::Init(void)
    {
        return _init();
    }

    // Get default MAC from API and convert to ASCII HEX
    esp_err_t Wifi::_get_mac(void)
    {
        uint8_t mac_byte_buffer[6]{};

        const esp_err_t status{esp_efuse_mac_get_default(mac_byte_buffer)};

        if (ESP_OK == status)
        {
            snprintf(mac_addr_cstr, sizeof(mac_addr_cstr), "%02X%02X%02X%02X%02X%02X",
                     mac_byte_buffer[0],
                     mac_byte_buffer[1],
                     mac_byte_buffer[2],
                     mac_byte_buffer[3],
                     mac_byte_buffer[4],
                     mac_byte_buffer[5]);
        }

        return status;
    }

} // namespace WIFI