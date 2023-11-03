#ifndef _QWiFiAP_
    #define _QWiFiAP_
    #include <ESP8266WiFi.h>
    #include <ESPAsyncTCP.h>
    #include <ESPAsyncWebServer.h>
    
    class QWiFiAP{
        private:
            AsyncWebServer * _ap_server = NULL;
            uint16_t _port = 80;
            struct wifiCreds
            {
                String ap_ssid = Q_WIFI_DEFAULT_AP_SSID;
                String ap_pass = Q_WIFI_DEFAULT_AP_PASS;
            } wc;
        public:
    };

#endif _QWiFiAP_