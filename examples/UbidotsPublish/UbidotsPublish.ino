/****************************************
 * Include Libraries
 ****************************************/

#include <UbidotsMQTT.h>

/****************************************
 * Define Constants
 ****************************************/

#ifndef TOKEN
#define TOKEN "YOUR_UBIDOTS_TOKEN"  // Put here your Ubidots TOKEN
#endif

#define VARIABLE_LABEL "YOUR_VARIABLE_LABEL"

void callback(char* topic, byte* payload, unsigned int length){}; // callback most be defined

/****************************************
 * Instances
 ****************************************/

Ubidots client(TOKEN, callback);

/****************************************
 * Auxiliar Functions
 ****************************************/

// Put here your auxiliar functions.

/****************************************
 * Main Functions
 ****************************************/

void setup() {
    Serial.begin(115200);

    // Comment below line to disable debug messages in the serial port.
    client.ubidotsSetDebug(true);

    // Uncomment this line if you have an Ubidots for Education account
    //client.ubidotsSetBroker("things.ubidots.com");

    // Connects to Ubidot Ubidots' Broker
    client.connect(5);

}

void loop() {

    if(!client.isConnected()){
        client.connect(5);
    }

    // Publish routine, if the device and variables are not created they will be created
    float value = analogRead(A0);
    client.add(VARIABLE_LABEL, value); // Insert as first parameter your variable label
    client.ubidotsPublish();

    // Client loop for publishing and to maintain the connection
    client.loop();
    delay(5000);
}
