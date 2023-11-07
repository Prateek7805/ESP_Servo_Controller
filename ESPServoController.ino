#include "QWiFiAP.h"
#include <Servo.h>
#include <OneButton.h>

#define BTN_PIN 12
Servo servo;
struct servo_details
{
    uint16_t angle = 0;
    uint8_t pin = 13;
} servo_params;

QWiFiAP q_server(80);
int16_t strToI16(String dec)
{
    char *endptr;
    uint16_t value = (uint16_t)strtol(dec.c_str(), &endptr, 10); // string, end of conversion pointer, base of int conversion
    if (*endptr != '\0')
    {
        return -1;
    }
    return value;
}

OneButton btn = OneButton(BTN_PIN, true, true);

void btn_handler_long_press(){
  Serial.println("Long Pressed");
  if(!q_server.reset_credentials()){
    Serial.println("[btn_handler_long_press]: Error in reseting credentials");
  }
  ESP.restart();
}

void setup() {
  Serial.begin(115200);
  servo.attach(servo_params.pin, 500, 2500);
  btn.attachLongPressStart(btn_handler_long_press);
  q_server.on("/angle", HTTP_GET, [](AsyncWebServerRequest * req){
        if(!req->hasParam("value")){
            req->send(400, "text/plain", "Invalid Paramter");
            return;
        }
        String angle_st = req->getParam("value")->value();
        int16_t angle = strToI16(angle_st);
        if(angle == -1){
            Serial.println("[/angle] : angle parameter value error");
            req->send(400, "text/plain", "Invalid Paramter value");
            return;
        }
        if(angle<0 || angle > 180){
            Serial.println("[/angle] : angle parameter value error");
            req->send(400, "text/plain", "Parameter - angle has value not in range: 0 - 360 deg");
            return;
        };
        servo_params.angle = angle;
        servo.write(servo_params.angle);
        req->send(200, "text/plain", "");
    });
  q_server.begin();
}

void loop() {
   q_server.fault_indicator();
   btn.tick();
}
