# Ubidots MQTT library for Paritcle Devices

This library connects to Ubidots' MQTT broker using the MQTT library of hirotakaster. In a few lines you should be able to publish or subscribe to Ubidots devices and variables.

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
    * In the "Libraries" tab, search "Ubidots" and select the UbidotsMQTT library.
    * Click on **INCLUDE IN APP**. And return to "MYAPP.ino"
    * Do the same to add hirotakaster's MQTT library.

## Subscribe to a variable

To subscribe to a variable, you need to specify the label of the data source and the label of your variable. The incoming value will be saved in the "message" variable of callback function.

```cpp
// This #include statement was automatically added by the Particle IDE.
#include "MQTT/MQTT.h"

// This #include statement was automatically added by the Particle IDE.
#include "UbidotsMQTT.h"
#define TOKEN "Your_Token_Here"  // Add here your Ubidots TOKEN
#define DATA_SOURCE_IDENTIFIER "my-particle-device" // The unique data source label. No commas, spaces or special characters are allowed.
#define VARIABLE_IDENTIFIER "temperature" // A unique variable label. No commas, spaces or special characters are allowed.

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
    while (client.connect()) {
        client.getValueSubscribe(DATA_SOURCE_IDENTIFIER, VARIABLE_IDENTIFIER);
    }
}

void loop() {
    client.loop();
}
```

## Publish values to a device

To publish values to a Ubidots data source:

```cpp
// This #include statement was automatically added by the Particle IDE.
#include "MQTT/MQTT.h"

// This #include statement was automatically added by the Particle IDE.
#include "UbidotsMQTT.h"
#define TOKEN "Your_Token_Here"  // Add here your Ubidots TOKEN
#define VARIABLE_ONE "humidity" // A variable identifier, in lowercase without spaces, commas or special characters
#define VARIABLE_TWO "temperature" // A variable identifier, in lowercase without spaces, commas or special characters
#define DATA_SOURCE_NAME "My_particle_device" // A unique identifier for your data source, in lowercase without spaces, commas or special characters

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
    client.add(VARIABLE_ONE, value_one);
    client.add(VARIABLE_TWO, value_two);
    client.sendValues();
}
```

If the function "setDataSourceLabel" is not used in the setup, then the data source will be named after Particle's device id.
