/*
Copyright (c) 2016 Ubidots.
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

Made by: Jose García -- Developer at Ubidots Inc

*/

#ifndef UbidotsMQTT_H
#define UbidotsMQTT_H

#include "MQTT.h"
#include "application.h"

#define MQTT_PORT 1883
#define SERVER "things.ubidots.com"
#define BUFFER_SIZE 512
#define MAX_VALUES 5

typedef struct Value {
    char  *_variableLabel;
    float _value;
    char *_context;
    long _timestamp;
} Value;



class Ubidots {

 private:
    void (*callback)(char*,uint8_t*,unsigned int);
    MQTT _client;
    char* _clientName;
    char* _server;
    char* _token;
    uint8_t _currentValue;
    Value * val;
 
 public:
    Ubidots(char* token, char* clientName, void (*callback)(char*,uint8_t*,unsigned int));
    bool add(char* variableLabel, float value);
    bool add(char* variableLabel, float value, char* context);
    bool add(char* variableLabel, float value, char* context, long timestamp);
    bool publish(char* deviceLabel);
};

#endif