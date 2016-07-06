// This #include statement was automatically added by the Particle IDE.
#include "MQTT/MQTT.h"

// This #include statement was automatically added by the Particle IDE.
#include "UbidotsMQTT.h"
#define TOKEN "Your_Token_Here"  // Add here your Ubidots TOKEN
#define VARIABLE_IDENTIFIER_ONE "humidity" // Add a variable identifier, it must be in lowercase
#define VARIABLE_IDENTIFIER_TWO "temperature" // Add a variable identifier, it must be in lowercase
#define DATA_SOURCE_NAME "My_beautiful_device"

void callback(char* topic, byte* payload, unsigned int length);

Ubidots client(TOKEN, callback);

void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);
    Serial.write(payload, length);
    Serial.println(topic);
}

void setup() {
    Serial.begin(115200);
    while (client.connect());
    client.setDataSourceLabel(DATA_SOURCE_NAME);
}

void loop() {
    float value_one = analogRead(A0);
    float value_two = analogRead(A1);
    client.add(VARIABLE_IDENTIFIER_ONE, value_one);
    client.add(VARIABLE_IDENTIFIER_TWO, value_two);
    client.sendValues();
}
