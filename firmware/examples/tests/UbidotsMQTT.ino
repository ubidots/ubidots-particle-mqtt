/****************************************
 * Include Libraries
 ****************************************/

#include "MQTT.h"

/****************************************
 * Define Constants
 ****************************************/

#define TOKEN "p4uuT2OIIFJwv7ncTVfoVqcfImwRQW"  // Add here your Ubidots TOKEN
#define DEVICE_LABEL "test-device"
#define VARIABLE_LABEL "test-var"
#define CLIENT_ID "awu7WSAcl912" //Random ASCII


/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length);

void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);
    Serial.write(payload, length);
    Serial.println(topic);
}


/****************************************
 * Instances
 ****************************************/

MQTT clientM = MQTT("things.ubidots.com", 1883, callback, 512);

/****************************************
 * Main Functions
 ****************************************/

void setup() {
    Serial.begin(115200);
    clientM.connect("aschcini32esdwws", "p4uuT2OIIFJwv7ncTVfoVqcfImwRQW", NULL);
}

void loop() {
    if(!clientM.isConnected()){
        Serial.println("attemping to connect");
    }
    while(!clientM.isConnected()){
        clientM.connect("asdbvnbhbuyu347uids00", "p4uuT2OIIFJwv7ncTVfoVqcfImwRQW", NULL);
        Serial.print(".");
        delay(5000);
    }
    Serial.println("connected");
    //clientM.publish("/v1.6/devices/test-m", "{\"test-var\":1}");
}
