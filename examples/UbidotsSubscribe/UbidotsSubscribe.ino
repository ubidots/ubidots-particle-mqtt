/****************************************
 * Include Libraries
 ****************************************/

#include <UbidotsMQTT.h>

/****************************************
 * Define Constants
 ****************************************/

#ifndef TOKEN
#define TOKEN "YOUR_UBIDOTS_TOKEN"  // Add here your Ubidots TOKEN
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
  for (int i = 0; i < length; i++) {
    Serial.print(
        (char)payload[i]);  // prints the answer of the broker for debug purpose
  }
  // Some stuff to make with the payload obtained

  Serial.println();
}

/****************************************
 * Instances
 ****************************************/

UbidotsMQTT clientMQTT(TOKEN, callback);

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);

  // Comment below line to disable debug messages.
  clientMQTT.ubidotsSetDebug(true);

  // Uncomment below line if you have an Ubidots for Education account
  // clientMQTT.ubidotsSetBroker("things.ubidots.com");

  // Connect to broker.
  clientMQTT.connect(5);

  if (clientMQTT.isConnected()) {
    // Insert as first parameter the device to subscribe and as second the
    // variable label
    clientMQTT.ubidotsSubscribe(DEVICE_LABEL_TO_SUBSCRIBE,
                                VARIABLE_LABEL_TO_SUBSCRIBE);
    // Subscribe to full dot
    clientMQTT.ubidotsSubscribe(DEVICE_LABEL_TO_SUBSCRIBE,
                                VARIABLE_LABEL_TO_SUBSCRIBE,
                                false);
  }
}

void loop() {
  if (!clientMQTT.isConnected()) {
    clientMQTT.connect(5);
    // Insert as first parameter the device to subscribe and as second the
    // variable label
    clientMQTT.ubidotsSubscribe(DEVICE_LABEL_TO_SUBSCRIBE,
                                VARIABLE_LABEL_TO_SUBSCRIBE);
    // Subscribe to full dot
    clientMQTT.ubidotsSubscribe(DEVICE_LABEL_TO_SUBSCRIBE,
                                VARIABLE_LABEL_TO_SUBSCRIBE,
                                false);
  }

  clientMQTT.loop();
  delay(5000);
}
