#include "Wifi.h"

#define LOG_TAG "WIFI"

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

    void Wifi::event_handler(void *arg, esp_event_base_t event_base,
                              int32_t event_id, void *event_data)
    {
        if(WIFI_EVENT == event_base)
        {
            return wifi_event_handler(arg, event_base, event_id, event_data);
        }
        else if(IP_EVENT == event_base)
        {
            return ip_event_handler(arg, event_base, event_id, event_data);
        }
        else
        {
            ESP_LOGE("myWIFI", "Unexpected event: %s", event_base); // TODO logging
        }
    }

    void Wifi::wifi_event_handler(void *arg, esp_event_base_t event_base,
                              int32_t event_id, void *event_data)
    {
        if (WIFI_EVENT == event_base)
    {
        const wifi_event_t event_type{static_cast<wifi_event_t>(event_id)};

        switch(event_type)
        {
        case WIFI_EVENT_STA_START:
        {
            std::lock_guard<std::mutex> state_guard(state_mutx);
            _state = state_e::READY_TO_CONNECT;
            break;
        }

        case WIFI_EVENT_STA_CONNECTED:
        {
            std::lock_guard<std::mutex> state_guard(state_mutx);
            _state = state_e::WAITING_FOR_IP;
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
                std::lock_guard<std::mutex> guard(state_mutx);
                _state = state_e::CONNECTED;
                break;
            }

            case IP_EVENT_STA_LOST_IP:
            {
                std::lock_guard<std::mutex> guard(state_mutx);
                _state = state_e::WAITING_FOR_IP;
                break;
            }

            default:
                // TODO IP6
                break;
            }
        }
    }

    esp_err_t Wifi::Begin(void)
    {
        std::lock_guard<std::mutex> guard(connect_mutx);
        std::lock_guard<std::mutex> state_guard(state_mutx);

        esp_err_t status{ESP_OK};

        switch (_state)
        {
        case state_e::READY_TO_CONNECT:
            status = esp_wifi_connect();
            if(ESP_OK == status)
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
        case state_e::DISCONNECTED:
        case state_e::ERROR:
            status = ESP_FAIL;
            break;
        }
        return status;
    }

    esp_err_t Wifi::_init(void)
    {
        std::lock_guard<std::mutex> guard(init_mutx);
        std::lock_guard<std::mutex> state_guard(state_mutx);

        esp_err_t status{ESP_OK};

        if(state_e::NOT_INITIALISED == _state)
        {
            status |= esp_netif_init();
            if (ESP_OK == status)
            {
                const esp_netif_t* const p_netif = esp_netif_create_default_wifi_sta();

                if(!p_netif)
                {
                    status = ESP_FAIL;
                }
            }

            if(ESP_OK == status)
            {
                status = esp_wifi_init(&wifi_init_cfg);
            }

            if(ESP_OK == status)
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

            if(ESP_OK == status)
            {
                status = esp_wifi_start(); // start Wifi
            }

            if(ESP_OK == status)
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