#pragma once

#include <cstring>
#include <algorithm>
#include <mutex>

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sys.h"





namespace WIFI
{
    class Wifi
    {
        constexpr static const char* _log_tag{"WiFI"};

        constexpr static const char* ssid{"TestGuest"};
        constexpr static const char* password{"00000000"};

    public:
        enum class state_e
        {
            NOT_INITIALISED,
            INITIALISED,
            READY_TO_CONNECT,
            CONNECTING,
            WAITING_FOR_IP,
            CONNECTED,
            DISCONNECTED,
            ERROR
        };

        // "Rule of 5" Constructors and assignment operators
        // Ref: https://en.cppreference.com/w/cpp/language/rule_of_three
        Wifi(void);
        ~Wifi(void)                     = default;
        Wifi(const Wifi&)               = default;
        Wifi(Wifi&&)                    = default;
        Wifi& operator=(const Wifi&)    = default;
        Wifi& operator=(Wifi&&)         = default;

        esp_err_t Init(void);  // Setup the stuff
        esp_err_t Begin(void); // Start Wifi, connect, etc...

        constexpr const state_e& get_state(void) { return _state; }

        constexpr static const char *get_mac(void) { return mac_addr_cstr; }

    private:
        static esp_err_t _init(void);
        static wifi_init_config_t wifi_init_cfg;
        static wifi_config_t wifi_cfg;

        static void state_machine(void);

        static void event_handler(void *arg, esp_event_base_t event_base,
                                  int32_t event_id, void *event_data);
        static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                                  int32_t event_id, void *event_data);
        static void ip_event_handler(void *arg, esp_event_base_t event_base,
                                  int32_t event_id, void *event_data);                                                    

        static state_e _state;

        // Get default MAC from API and convert to ASCII HEX
        static esp_err_t _get_mac(void);

        static char mac_addr_cstr[13];

        static std::mutex init_mutx;        ///< Initialisation Mutex
        static std::mutex connect_mutx;     ///< Connect Mutex
        static std::mutex state_mutx;       ///< State change Mutex
    }; // Wifi class

} // namaspace WIFI