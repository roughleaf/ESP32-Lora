#pragma once

#include <cstring>
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
        constexpr static const char *ssid{"TestGuest"};
        constexpr static const char *password{"00000000"};

    public:
        enum class state_e
        {
            NOT_INITIALISED,
            INITIALISED,
            WAITING_FOR_CREDENTIALS,
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

        esp_err_t init(void);  // Setup the stuff
        esp_err_t begin(void); // Start Wifi, connect, etc...

        state_e get_state(void);

        constexpr static const char *get_mac(void) { return mac_addr_cstr; }

    private:
        static esp_err_t _init(void);

        static void state_machine(void);
        static state_e _state;

        // Get default MAC from API and convert to ASCII HEX
        static esp_err_t _get_mac(void);

        static char mac_addr_cstr[13];

        static std::mutex init_mutx;

        static wifi_init_config_t wifi_init_cfg;
        static wifi_config_t wifi_cfg;

    }; // Wifi class

} // namaspace WIFI