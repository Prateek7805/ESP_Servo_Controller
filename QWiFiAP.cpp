#include "QWiFiAP.h";

void QWiFiAP::QWiFiAP(uint16_t port, servo_params *data)
{
    _port = port;
    _data = data;
}

void QWiFiAP::QWiFiAP(servo_params *data)
{
    QWiFiAP(_port, data);
}

void QWiFiAP::_reset_credentials()
{
    if (!LittleFS.begin())
    {
        req->send(500, "text/plain", "FS Init : Please restart the device");
        return;
    }
    if (!LittleFS.remove(CREDS_PATH))
    {
        req->send(500, "text/plain", "Failed to delete the saved credentials");
        return;
    }
    req->send(200, "text/plain", "ok");
    ESP.restart();
}

int16_t QWiFiAP::_strToI16(String dec)
{
    char *endptr;
    uint8_t value = (uint8_t)strtol(dec.c_str(), &endptr, 10); // string, end of conversion pointer, base of int conversion
    if (*endptr != '\0')
    {
        return -1;
    }
    return value;
}


bool QWiFiAP::_parse_creds(uint8_t *data, String *ap_ssid, String *ap_pass)
{
    String body = (char *)data;

    // AP SSID
    int si = body.indexOf("\"dn\":\"");
    if (si == -1)
    {
        return false;
    }
    si += 6;
    int ei = body.indexOf("\",", si);
    if (ei == -1)
    {
        return false;
    }
    *ap_ssid = body.substring(si, ei);


    // AP Password
    si = body.indexOf("\"dp\":\"");
    if (si == -1)
    {
        return false;
    }
    si += 6;
    ei = body.indexOf("\"", si);
    if (ei == -1)
    {
        return false;
    }
    *ap_pass = body.substring(si, ei);

    return true;
}

bool QWiFiAP::_validate_creds(String ap_ssid, String ap_pass, String *msg)
{
    if (ap_ssid.length() < 8 || ap_ssid.length() > 20 || ap_ssid.indexOf(" ") != -1)
    {
        *msg = "Invalid Device Name";
        return false;
    }

    if (ap_pass.length() < 8 || ap_pass.length() > 20)
    {
        *msg = "Invalid Device Password";
        return false;
    }

    return true;
}

bool QWiFiAP::_save_FS(void)
{
    if (!LittleFS.begin())
    {
        Serial.println("[_save_FS] : LittleFS check unsuccessful");
        Serial.println("[_save_FS] : Try restarting the device");
        return false;
    }
    File creds_file = LittleFS.open(CREDS_PATH, "w");
    if (!creds_file)
    {
        Serial.printf("[_save_FS] : Failed to create file at %s\n", CREDS_PATH);
        return false;
    }
    creds_file.printf("%s\n%s\n", wc.ap_ssid.c_str(), wc.ap_pass.c_str());
    creds_file.close();
    return true;
}


bool QWiFiAP::_ap_server_definition(void)
{
    _ap_server = new AsyncWebServer(_port);
    if (!_ap_server)
    {
        return false; // AP server initialization failed;
    }
    _ap_server->on("/", HTTP_GET, [](AsyncWebServerRequest *req)
                   { req->send_P(200, "text/html", _index); });
    _ap_server->on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *req)
                   { req->send_P(200, "text/css", _styles); });
    _ap_server->on("/script.js", HTTP_GET, [](AsyncWebServerRequest *req)
                   { req->send_P(200, "application/javascript", _script); });
    _ap_server->on("/servo", HTTP_GET, [this](AsyncWebServerRequest * req){
        if(!req->hasParam("angle")){
            req->send(400, "text/plain", "Invalid Paramter");
            return;
        }
        String angle_st = req->param("angle").value();
        int16_t angle = _strToI16(angle_st);
        if(angle == -1){
            Serial.println("[/servo] : angle parameter value error");
            req->send(400, "text/plain", "Invalid Paramter value");
            return;
        }

        if(angle<0 || angle > 360{
            Serial.println("[/servo] : angle parameter value error");
            req->send(400, "text/plain", "Parameter - angle has value not in range: 0 - 360 deg");
            return;
        });

        _data->angle = angle;
        req->send(200, "text/plain", "");
    });
    _ap_server->on("/reset", HTTP_GET, [this](AsyncWebServerRequest *req) {
        _reset_credentials();
    });
    //POST body handler for AP credential update
    _ap_server->onRequestBody([this](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total)
                              {  
        if (len > 250){
            req->send(400, "text/plain", "body too large");
            return;
        }
        if (req->url() != "/creds"){
            req->send(400, "text/plain", "bad request");
            return;
        }
        
        String dn,dp;
        
        if (!_parse_creds(data, &dn,&dp)){
            req->send(400, "text/plain", "error parsing json body");
            return;
        }
        String msg;
        if(!_validate_creds(dn,dp, &msg)){
            req->send(400, "text/plain", msg.c_str());
            return;
        }

        wc.ap_ssid = dn;
        wc.ap_pass = dp;
        
        if(!_save_FS()){
            req->send(500, "text/plain", "Failed to save credentials (～￣▽￣)～");
            return;
        }
        req->send(200, "text/plain", "Please restart the device to apply changes"); 
    });
    return true; // AP server definition successful
}

bool QWiFiAP::_load_FS(void)
{
    if (!LittleFS.begin())
    {
        Serial.println("[_load_FS] : LittleFS initialization unsuccessful");
        Serial.println("[_load_FS] : Resorting to default AP creds");
        return false;
    }
    if (!LittleFS.exists(CREDS_PATH))
    {
        Serial.printf("[_load_FS] : No file found at %s\n", CREDS_PATH);
        Serial.println("[_load_FS] : Resorting to default AP credentials");
        return false;
    }
    File creds_file = LittleFS.open(CREDS_PATH, "r");
    if (!creds_file)
    {
        Serial.printf("[_load_FS] : Failed to read credentials file at %s\n", CREDS_PATH);
        Serial.println("[_load_FS] : Resorting to default AP creds");
        return false;
    }
    wc.ap_ssid = creds_file.readStringUntil('\n');
    wc.ap_pass = creds_file.readStringUntil('\n');
    creds_file.close();
    return true;
}

void QWiFiAP::_fault_indicator_init(){
    pinMode(fdat.led_pin, OUTPUT);
}

void QWiFiAP::_set_error(int8_t sev){
    if(sev < 0 || sev > 5) 
    {
        Serial.println("[_set_error] : invalid error severity");
        return;
    }
     fdat.sev = sev;
}

//used for errors that can be fixed by restart
void QWiFiAP::fault_indicator(void){
    if(fdat.sev == 0){
        return;
    }
    if(fdat.sev <1 || fdat.sev >5){
        Serial.println("[fault_indicator] : Invalid severity");
        return;
    }
    uint8_t index = fdat.sev-1;
    if(millis() - fdat._t > fdat.blink_intr[index]){
        digitalWrite(fdat.led_pin, !digitalRead(fdat.led_pin));
        fdat._t = millis();
    }
}

void QWiFiAP::begin(void)
{
    _fault_indicator_init();
    if(!_load_FS()){
        _set_error(5);
    }
    if (!_ap_server_definition())
    {
        Serial.println("[begin] : AP server initialization failed");
        _set_error(1);
        return;
    }
    _ap_server->begin();
}
