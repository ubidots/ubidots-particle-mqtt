/****************************************
 * Include Libraries
 ****************************************/

#include <UbidotsMQTT.h>

/****************************************
 * Define Constants
 ****************************************/

#ifndef TOKEN
#define TOKEN "Your TOKEN"  // Add here your Ubidots TOKEN
#endif

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println("payload obtained from server:");
  for (int i = 0; i < length; i++) {
    Serial.print(
        (char)payload[i]);  // prints the answer of the broker for debug purpose
  }
  // Some stuff to make with the payload obtained
  //
  //
  Serial.println();
}

/****************************************
 * Instances
 ****************************************/

UbidotsMQTT client(TOKEN, callback);

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  client.initialize();

  // Uncomment this line if you have a business Ubidots account
  // client.ubidotsSetBroker("business.api.ubidots.com");

  if (client.isConnected()) {
    // Insert as first parameter the device to subscribe and as second the
    // variable label
    client.ubidotsSubscribe("device-to-subscribe", "water-level");
  }
}

void loop() {
  if (!client.isConnected()) {
    client.reconnect();
    // Insert as first parameter the device to subscribe and as second the
    // variable label
    client.ubidotsSubscribe("device-to-subscribe", "water-level");
  }

  // Client loop for publishing and to maintain the connection
  client.loop();
  delay(1000);
}
