#pragma once

#include "esp_wifi.h"

namespace WIFI
{
    class Wifi
    {
    private:
        void state_machine(void);
        esp_err_t _get_mac(void);
        static char mac_addr_cstr[13];

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
        esp_err_t begin(void); // Start Wifi, conmnect, etc...

        state_e get_state(void);
        const char* get_mac(void) { return mac_addr_cstr; }

    }; // Wifi class

} // namaspace WIFI