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
}

void loop() {
  if (!client.isConnected()) {
    client.reconnect();
  }

  // Publish routine, if the device and variables are not created they will be
  // created
  float value = 1;
  Serial.println("Sending value");
  client.add("test-var-1",
             value);  // Insert as first parameter your variable label
  client.add("test-var-2", value,
             "\"lat\":10.302, \"lng\":2.9384");  // Adds value with context
  client.add("test-var-3", value, NULL,
             1492445109);                // Adds value with custom timestamp
  client.ubidotsPublish("test-device");  // Insert your device label where the
                                         // values will be stored in Ubidots

  // Client loop for publishing and to maintain the connection
  client.loop();
  delay(1000);
}
