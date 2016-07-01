# Ubidots MQTT library for Paritcle Devices

This library is to use easily MQTT library of hirotakaster. This library is integrated with Ubidots Cloud, then with few lines you will be able to post or get variable values of Ubidots Cloud.

## Requirements

## Requirements

* [Particle Photon, Electron, Core](https://store.particle.io/).
* Micro USB cable.
* Internet connection.
* Add [MQTT library of hirotakaster](https://github.com/hirotakaster/MQTT).

## Setup

1. Setting up your Particle device:
    - If you are using a Particle Electron, please follow this steps to set up the Particle data plan. [Particle Electron steps](https://setup.particle.io/).
    - If you are using a Particle Core you have two ways to set it up:
        - [Using your smart phone](https://docs.particle.io/guide/getting-started/start/core/).
        - [Connecting it to your computer over USB](https://docs.particle.io/guide/getting-started/connect/core/).
    - If you are using a Particle Photon you have two ways to set it up:
        - [Using your smart phone](https://docs.particle.io/guide/getting-started/start/Photon/).
        - [Connecting it to your computer over USB](https://docs.particle.io/guide/getting-started/connect/Photon/).

2. After claiming your Particle Device and setting up your Ubidots account, let's go to [Particle's Web IDE](https://build.particle.io/build).
    * In the Particle's Web IDE create a new app and set the name.
    * Go to the library tab.
    * In contributed library write Ubidots and select the Ubidots library.
    * Click on **INCLUDE IN APP**. And return to "MYAPP.ino"
    * Do the same to add MQTT library.


**Note: You will have to include MQTT and UbidotsMQTT libraries manually, because in this momment Particle doesn't support include both automatically**

## Suscribe to a variable

# WORK IN PROGRESS

## Publish variables

You could use the next code to post variables to Ubidots Cloud:

```cpp
// This #include statement was automatically added by the Particle IDE.
#include "MQTT/MQTT.h"

// This #include statement was automatically added by the Particle IDE.
#include "UbidotsMQTT.h"
#define TOKEN "Your_Token_Here"  // Add here your Ubidots TOKEN
#define VARIABLE_IDENTIFIER_ONE "humidity" // Add a variable identifier, it must be in lowercase
#define VARIABLE_IDENTIFIER_TWO "temperature" // Add a variable identifier, it must be in lowercase

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
        client.getValueSubscribe("23002e000551343530343432","temperature");
    }
}

void loop() {
    float value_one = analogRead(A0);
    float value_two = analogRead(A1);
    client.add(VARIABLE_IDENTIFIER_ONE, value_one);
    client.add(VARIABLE_IDENTIFIER_TWO, value_two);
    client.sendValues();
}
```