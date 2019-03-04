/****************************************
 * Include Libraries
 ****************************************/

#include <UbidotsMQTT.h>

/****************************************
 * Define Constants
 ****************************************/

#ifndef TOKEN
#define TOKEN "YOUR_UBIDOTS_TOKEN" // Put here your Ubidots TOKEN
#endif

#define VARIABLE_LABEL "YOUR_VARIABLE_LABEL"

void callback(char *topic, byte *payload, unsigned int length){}; // callback most be defined

/****************************************
 * Instances
 ****************************************/

UbidotsMQTT clientMQTT(TOKEN, callback);

/****************************************
 * Auxiliar Functions
 ****************************************/

// Put here your auxiliar functions.

/****************************************
 * Main Functions
 ****************************************/

void setup(){
    Serial.begin(115200);

    // Comment below line to disable debug messages in the serial port.
    clientMQTT.ubidotsSetDebug(true);

    // Uncomment this line if you have an Ubidots for Education account
    //clientMQTT.ubidotsSetBroker("things.ubidots.com");

    // Connects to Ubidot Ubidots' Broker
    clientMQTT.connect(5);
}

void loop(){

    if (!clientMQTT.isConnected()){
        clientMQTT.connect(5);
    }

    // Publish routine, if the device and variables are not created they will be created
    float value = analogRead(A0);

    // Adds context key-value pairs
    clientMQTT.addContext("weather-status", "sunny");
    clientMQTT.addContext("time", "11:40:56 pm");

    // Reserves memory to store context array
    char *context = (char *)malloc(sizeof(char) * 60);

    // Builds the context with the array above to send to Ubidots
    clientMQTT.getContext(context);

    clientMQTT.add(VARIABLE_LABEL, value, context); // Insert as first parameter your variable label

    clientMQTT.ubidotsPublish();
    free(context);

    // Client loop for publishing and to maintain the connection
    clientMQTT.loop();
    delay(5000);
}
