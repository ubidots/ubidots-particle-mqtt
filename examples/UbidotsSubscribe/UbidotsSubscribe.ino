/****************************************
 * Include Libraries
 ****************************************/

#include <UbidotsMQTT.h>

/****************************************
 * Define Constants
 ****************************************/

#ifndef TOKEN
#define TOKEN "YOUR_TOKEN"  // Add here your Ubidots TOKEN
#endif

#define VARIABLE_LABEL_TO_SUBSCRIBE "YOUR_VARIABLE_LABEL"
#define DEVICE_LABEL_TO_SUBSCRIBE "YOUR_DEVICE_LABEL"


/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, uint8_t* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    Serial.println("payload obtained from server:");
    for (int i=0;i<length;i++) {
        Serial.print((char)payload[i]); // prints the answer of the broker for debug purpose
    }
    // Some stuff to make with the payload obtained
    
    Serial.println();
}


/****************************************
 * Instances
 ****************************************/

Ubidots client(TOKEN, callback);

/****************************************
 * Main Functions
 ****************************************/

void setup() {
    Serial.begin(115200);

    // Comment below line to disable debug messages.
    client.ubidotsSetDebug(true);

    // Uncomment below line if you have an Ubidots for Education account
    //client.ubidotsSetBroker("things.ubidots.com");

    // Connect to broker.
    client.connect(5); 

    if(client.isConnected()){
        // Insert as first parameter the device to subscribe and as second the variable label
        client.ubidotsSubscribe(DEVICE_LABEL_TO_SUBSCRIBE, VARIABLE_LABEL_TO_SUBSCRIBE); 
    }
}

void loop() {
    if(!client.isConnected()){
        client.connect(5);
        // Insert as first parameter the device to subscribe and as second the variable label
        client.ubidotsSubscribe(DEVICE_LABEL_TO_SUBSCRIBE, VARIABLE_LABEL_TO_SUBSCRIBE); 
    }

    client.loop();
    delay(5000);
}
