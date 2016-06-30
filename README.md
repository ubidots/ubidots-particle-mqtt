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

# WORK IN PROGRESS