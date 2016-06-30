
#ifndef UbidotsMQTT_H
#define UbidotsMQTT_H
#include "MQTT/MQTT.h"

#if defined(SPARK) || (PLATFORM_ID==88)
#include "spark_wiring_string.h"
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_usbserial.h"
#elif defined(ARDUINO)
#include "Client.h"
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
void callback(char* topic, uint8_t* payload, unsigned int length);




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
    bool sendValues();
    bool getValueSubscribe(char* labelDataSource, char* labelVariable);
    bool init();
    bool add(char* label, float value);
    bool add(char* label, float value, char* context);
    bool add(char* label, float value, char* context, double timestam);
};

#endif 

