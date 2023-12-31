#ifndef _QWiFiAP_
#define _QWiFiAP_

#include "config.h"

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Updater.h>
#include "LittleFS.h"
#include "index.h"
#include "styles.h"
#include "script.h"


class QWiFiAP
{
private:
    AsyncWebServer *_ap_server = NULL;
    uint16_t _port = 80;
    struct wifiCreds
    {
        String ap_ssid = QWIFIAP_DEFAULT_AP_SSID;
        String ap_pass = QWIFIAP_DEFAULT_AP_PASS;
    } wc;

    struct faultData
    {
        uint8_t led_pin = 2;
        uint8_t sev = 0; // 1-5 highest to lowest, 0 for no fault
        uint32_t _t = 0;
        uint32_t blink_intr[5] = {QWIFIAP_FAULT_B_INTR_1,
                                  QWIFIAP_FAULT_B_INTR_2,
                                  QWIFIAP_FAULT_B_INTR_3,
                                  QWIFIAP_FAULT_B_INTR_4,
                                  QWIFIAP_FAULT_B_INTR_5};

    } fdat;

    bool _ap_server_definition(void);
    bool _load_FS(void);
    
    bool _parse_creds(uint8_t *data, String *ap_ssid, String *ap_pass);
    bool _validate_creds(String ap_ssid, String ap_pass, String *msg);
    bool _save_FS(void);
    void _fault_indicator_init(void);
    void _set_error(int8_t);
public:
    QWiFiAP();
    QWiFiAP(uint16_t port);
    void on(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction handler);
    void begin(void);
    void fault_indicator(void);
    bool reset_credentials(void);
};

#endif _QWiFiAP_