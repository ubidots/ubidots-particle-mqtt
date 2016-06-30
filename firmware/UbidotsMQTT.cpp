#include "UbidotsMQTT.h"

#if defined(ARDUINO)
#include "Arduino.h"
#elif defined(SPARK)
#include "application.h"
#endif

// ------------------------------------------------------------
// -----------------Ubidots Fucntions added--------------------.
// ------------------------------------------------------------
Ubidots::Ubidots(char* token, char* server) {
    _server = server;
    _token = token;
    _dsName = "Particle";
    currentValue = 0;
    val = (Value *)malloc(MAX_VALUES*sizeof(Value));
    String str = Particle.deviceID();
    _pId = new char[str.length() + 1];
    strcpy(_pId, str.c_str());
}
bool Ubidots::init() {
    _broker.connect(_pId, _token, "");
}
bool Ubidots::getValueSubscribe(char* labelDataSource, char* labelVariable) {
    char *topic = (char*)malloc(100);
    sprintf(topic, "/v1.6/devices/%s/%s/lv", labelDataSource, labelVariable);
    int timeout = 0;
    if (_broker.isConnected()) {
        _broker.subscribe(topic);
    } else {
        while (!_broker.connect(_pId, _token, "")) {
            timeout++;
            delay(1);
            if (timeout > 5000)
                Serial.println("Connection problems");
            free(topic);
            return false;
        }
    }
    free(topic);
    return true;
}
bool Ubidots::sendValues() {
    char *topic = (char*)malloc(50);
    char *payload = (char*)malloc(512);
    uint8_t i = 0;
    sprintf(topic, "%s%s", FIRST_PART_TOPIC, _pId);
    sprintf(payload, "{");
    // {"temperature":{"value": 10, "timestamp":1464661369000}, "humidity": 50}
    while (i < currentValue) {
        sprintf(payload, "%s\"%s\":{\"value\":%f", payload, (val+i)->labelName, (val+i)->value);
        if ((val+i)->timestamp != NULL) {
            sprintf(payload, "%s, \"timestamp\":%f", payload, (val+i)->timestamp);
        }
        if ((val+i)->context != NULL) {
            sprintf(payload, "%s, \"context\":{%s}", payload, (val+i)->context);
        }
        sprintf(payload, "%s}", payload);
        i++;
        if (i >= currentValue) {
            break;
        } else {
            sprintf(payload, "%s, ", payload);
        }
    }
    sprintf(payload, "}");
    int timeout = 0;
    if (_broker.isConnected()) {
        _broker.publish(topic, payload);
    } else {
        while(!_broker.connect(_pId, _token, "")) {
            timeout++;
            delay(1);
            if (timeout > 5000)
                Serial.println("Connection problems");
            free(topic);
            free(payload);
            return false;
        }
    }
    free(topic);
    free(payload);
    return true;
}
bool Ubidots::add(char* label, float value) {
    return add(label, value, NULL, NULL);
}
bool Ubidots::add(char* label, float value, char* context) {
    return add(label, value, context, NULL);
}
bool Ubidots::add(char* label, float value, char* context, double timestamp) {
    (val+currentValue)->labelName = label;
    (val+currentValue)->value = value;
    (val+currentValue)->context = context;
    (val+currentValue)->timestamp = timestamp;
    currentValue++;
    if (currentValue > MAX_VALUES) {
        Serial.println(F("You are sending more than the maximum of consecutive variables"));
        currentValue = MAX_VALUES;
    }
}
void callback(char* topic, uint8_t* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);
    Serial.println(message);
    delay(1000);
}
