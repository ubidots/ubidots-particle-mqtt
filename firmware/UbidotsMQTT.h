/*
Copyright (c) 2013-2016 Ubidots.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Made by Mateo Velez - Metavix for Ubidots Inc

*/

#ifndef UbidotsMQTT_H
#define UbidotsMQTT_H
#include "MQTT/MQTT.h"

#if defined(SPARK) || (PLATFORM_ID==88)
#include "spark_wiring_string.h"
#include "application.h"
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_usbserial.h"
#elif defined(ARDUINO)
#include "Client.h"
#include "Arduino.h"
#endif

#define MQTT_PORT 1883
#define SERVER "things.ubidots.com"
#define MAX_VALUES 5
#define FIRST_PART_TOPIC "/v1.6/devices/"

typedef struct Value {
    char  *labelName;
    char  *context;
    double timestamp;
    float value;
} Value;

void callback(char* topic, byte* payload, unsigned int length);

class Ubidots {
 private:
    MQTT _broker = MQTT(SERVER, MQTT_PORT, callback);
    uint8_t currentValue;
    char* _server;
    char* _token;
    char* _pId;
    char* _dsName;
    Value * val;

 public:
    Ubidots(char* token, char* server = SERVER);
    bool loop();
    bool sendValues();
    bool getValueSubscribe(char* labelDataSource, char* labelVariable);
    bool connect();
    bool add(char* label, float value);
    bool add(char* label, float value, char* context);
    bool add(char* label, float value, char* context, double timestam);
};

#endif
