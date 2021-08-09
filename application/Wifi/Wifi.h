#pragma once

#include "esp_wifi.h"
#include "esp_log.h"

#include <mutex>

#include <cstring>

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

        Wifi(void);

        esp_err_t init(void);  // Setup the stuff
        esp_err_t begin(void); // Start Wifi, connect, etc...

        state_e get_state(void);
        const char *get_mac(void) { return mac_addr_cstr; }

    private:
        void state_machine(void);

        esp_err_t _get_mac(void);
        esp_err_t _wifi_initialise(void);

        static char mac_addr_cstr[13];

        static std::mutex first_call_mutx;
        static bool first_call;

    }; // Wifi class

} // namaspace WIFI