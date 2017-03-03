/****************************************
 * Include Libraries
 ****************************************/

#include "UbidotsMQTT.h"


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

Ubidots client(TOKEN, CLIENT_ID, callback);


/****************************************
 * Main Functions
 ****************************************/

void setup() {
    Serial.begin(115200);
    //MQTT clientM = MQTT("things.ubidots.com", 1883, callback, 512);
    //clientM.connect("aschcini32esdwws", "p4uuT2OIIFJwv7ncTVfoVqcfImwRQW", NULL);
}

void loop() {
    client.connect();
    float value_one = analogRead(A0);
    float value_two = analogRead(A1);
    client.add(VARIABLE_LABEL, value_one);
    client.add(VARIABLE_LABEL, value_one);
    client.publish(DEVICE_LABEL);
    delay(10000);
}
