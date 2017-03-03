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

Made by: José García -- Developer at Ubidots Inc

*/

#include "UbidotsMQTT.h"

Ubidots::Ubidots(char* token, char* clientName, void (*callback)(char*,uint8_t*,unsigned int)){
    this->callback = callback;
    _server = SERVER;
    _client = MQTT(_server, MQTT_PORT, callback, BUFFER_SIZE);
    _token = token;
    _clientName = clientName;
    _currentValue = 0;
    val = (Value *)malloc(MAX_VALUES*sizeof(Value));
}

bool Ubidots::add(char* variableLabel, float value){
    return add(variableLabel, value, NULL, NULL);
}

bool Ubidots::add(char* variableLabel, float value, char* context){
    return add(variableLabel, value, context, NULL);
}

bool Ubidots::add(char* variableLabel, float value, char* context, long timestamp){
    (val+_currentValue)->_variableLabel = variableLabel;
    (val+_currentValue)->_value = value;
    (val+_currentValue)->_context = context;
    (val+_currentValue)->_timestamp = timestamp;
    _currentValue++;
    if (_currentValue > MAX_VALUES) {
        Serial.println(F("You are sending more than the maximum of consecutive variables"));
        _currentValue = MAX_VALUES;
    }
}

bool Ubidots::publish(char* deviceLabel){
    char payload[BUFFER_SIZE];
    sprintf(payload, "{");

    for(uint i=0; i<=_currentValue;){  
        sprintf(payload, "%s\"%s\":{\"value\":%f", payload, (val+i)->_variableLabel, (val+i)->_value);
        i++;
        if (i >= _currentValue) {
            sprintf(payload, "%s}}", payload);
            break;
        } else {
            sprintf(payload, "%s}, ", payload);
        }
    }

    Serial.println("payload: ");
    Serial.println(payload);
    _currentValue=0;
    return true;
}
