# Ubidots MQTT library for Particle Devices

This library is based on the MQTT library for Spark Core made by Hirotakaster which is one of the most popular libraries for MQTT in the Particle environment. We had to make a small modification on line 121 in the MQTT.h file. When trying to wrapp up the library a memory allocation for IP address is not erased properly, so in order to fix it, we've declared as NULL and hence avoids taking a random value. Additionally, some guards were added.

In a few lines you should be able to publish or subscribe to Ubidots devices and variables.

## Requirements

* [Particle Core, Photon or Electron](https://store.particle.io/).
* Micro USB cable.
* Internet connection.
* The [MQTT library by hirotakaster](https://github.com/hirotakaster/MQTT).

## Setup

1. Setting up your Particle device:
    - If you are using a Particle Electron, please follow these steps to set up the Particle data plan. [Particle Electron steps](https://setup.particle.io/).
    - If you are using a Particle Core you have two ways to set it up:
        - [Using your smart phone](https://docs.particle.io/guide/getting-started/start/core/).
        - [Connecting it to your computer over USB](https://docs.particle.io/guide/getting-started/connect/core/).
    - If you are using a Particle Photon you have two ways to set it up:
        - [Using your smart phone](https://docs.particle.io/guide/getting-started/start/Photon/).
        - [Connecting it to your computer over USB](https://docs.particle.io/guide/getting-started/connect/Photon/).

2. After claiming your Particle Device and setting up your Ubidots account, let's go to [Particle's Web IDE](https://build.particle.io/build).
    * In the Particle's Web IDE create a new app and set the name.
    * Go to the library tab.
    * Search "Ubidots" and select the UbidotsMQTT library.
    * Click on **INCLUDE IN APP**. And return to your app.
    * Do the same to add hirotakaster's MQTT library.

# Methods Available

### Ubidots Constructor
```
Ubidots(char* token, void (*callback)(char*, uint8_t*, unsigned int))
```
> Creates an Ubidots instance, you must setup as input your Ubidots TOKEN and the callback function. Keep in mind that the callback function must be declared even if you don't need to subscribe to any variable, so the below code line should always be in your scripts:

```cpp
void callback(char* topic, byte* payload, unsigned int length);
```
### add
```
add(char* variableLabel, float value, char *context, char *timestamp);
```
> Add a variable with a value, context and timestamp to be sent to a certain data source, once you use add() you can publish your variable using the ubidotsPublish() method. You can add 5 variables maximum before of publish them. Context and timestamp are optionals.
## connect
```
connect();
```
> Tries to connect to the broker
## initialize
```
initialize();
```
> Creates the proper credentials to connect to Ubidots' broker
## loop
```
loop();
```
> Infinite loop for MQTT connection, insert it at the end of your routine
## ubidotsPublish
```
ubidotsPublish();
```
> Publish the values added using the add() method
## ubidotsSetBroker
```
ubidotsSetBroker(char* broker);
```
> Sets the broker properly for publish and subscribe to Ubidots accounts. If your account if a business one, set "business.api.ubidots.com" or the endpoint provided by Ubidots as your broker, see examples for more information.
By default, broker will be set to publish and subscribe to free educational version accounts with broker "things.ubidots.com".
## ubidotsSubscribe
```
ubidotsSubscribe(char* deviceLabel, char* variableLabel);
```
> Subscribe to the specified device label and variable label of your Ubidots account.

# Examples
## Subscribe to a variable

To subscribe to a variable, you need to specify your device and variable lables as input arguments for the ubidotsSubscribe() function. The incoming value will be returned by the MQTT library in the payload variable, by default the subscribed function only gets the last value of the variable subscribed. Retain value feature is only available for business users.

```cpp
/****************************************
 * Include Libraries
 ****************************************/

#include "UbidotsMQTT.h"

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
    for (int i=0;i<length;i++) {
        Serial.print((char)payload[i]); // prints the answer of the broker for debug purpose
    }
    // Some stuff to make with the payload obtained
        //
   //
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
    client.initialize();

    // Uncomment this line if you have a business Ubidots account
    //client.ubidotsSetBroker("business.api.ubidots.com");

    if(client.isConnected()){
        // Insert as first parameter the device to subscribe and as second the variable label
        client.ubidotsSubscribe("device-to-subscribe", "water-level"); 
    }
}

void loop() {
    if(!client.isConnected()){
        client.reconnect();
    }

    // Client loop for publishing and to maintain the connection
    client.loop();
    delay(1000);
}

```

## Publish values to a device

To publish values to Ubidots, you have to call first the add() function first for storing the values to send (max of 5 values per request), so insert as parameters for the add() function the variable label, the context and the timestamp (these last two are optionals):

```cpp
/****************************************
 * Include Libraries
 ****************************************/

#include "UbidotsMQTT.h"

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
    for (int i=0;i<length;i++) {
        Serial.print((char)payload[i]); // prints the answer of the broker for debug purpose
    }
    // Some stuff to make with the payload obtained
        //
   //
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
    client.initialize();

    // Uncomment this line if you have a business Ubidots account
    //client.ubidotsSetBroker("business.api.ubidots.com");
}

void loop() {
    if(!client.isConnected()){
        client.reconnect();
    }

    // Publish routine, if the device and variables are not created they will be created
    float value = 1;
    Serial.println("Sending value");
    client.add("test-var-1", value); // Insert as first parameter your variable label
    client.add("test-var-2", value, "\"lat\":10.302, \"lng\":2.9384"); //Adds value with context
    client.add("test-var-3", value, NULL, 1492445109); // Adds value with custom timestamp
    client.ubidotsPublish("test-device"); // Insert your device label where the values will be stored in Ubidots

    // Client loop for publishing and to maintain the connection
    client.loop();
    delay(1000);
}
```