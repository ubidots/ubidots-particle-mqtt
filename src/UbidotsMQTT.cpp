/*
Copyright (c) 2013-2016 Ubidots.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Made by: Jose García -- Developer at Ubidots Inc

*/
#include "UbidotsMQTT.h"

#if defined(ARDUINO)
#include "Arduino.h"
#elif defined(SPARK)
#include "application.h"
#endif

UbidotsMQTT::UbidotsMQTT(char* token,
                         void (*callback)(char*, uint8_t*, unsigned int)) {
  this->callback = callback;
  _server = SERVER;
  _token = token;
  _currentValue = 0;
  val = (ValueMQTT*)malloc(MAX_VALUES * sizeof(ValueMQTT));
}

void UbidotsMQTT::add(char* variableLabel, float value) {
  add(variableLabel, value, NULL, NULL);
}

void UbidotsMQTT::add(char* variableLabel, float value, char* context) {
  add(variableLabel, value, context, NULL);
}

void UbidotsMQTT::add(char* variableLabel, float value, char* context,
                      unsigned long timestamp) {
  (val + _currentValue)->_variableLabel = variableLabel;
  (val + _currentValue)->_value = value;
  (val + _currentValue)->_context = context;
  (val + _currentValue)->_timestamp = timestamp;
  _currentValue++;
  if (_currentValue > MAX_VALUES) {
    Serial.println(
        F("You are adding more than the maximum of the allowed consecutive "
          "variables"));
    _currentValue = MAX_VALUES;
  }
}

bool UbidotsMQTT::isConnected() { return _client->isConnected(); }

bool UbidotsMQTT::connect() {
  bool connected = false;
  if (!_client->isConnected()) {
    connected = reconnect();
  }
  return connected;
}

void UbidotsMQTT::initialize() {
  this->_client = new MQTT(_server, 1883, this->callback, 512);
  _clientName = System.deviceID();
  _client->connect(_clientName, _token, NULL);
  bool connected = _client->isConnected();
  if (connected) {
    Serial.println("connected to broker");
  }
}

bool UbidotsMQTT::loop() { return _client->loop(); }

bool UbidotsMQTT::reconnect() {
  if (!_client->isConnected()) {
    Serial.println("attemping to connect");
  }
  while (!_client->isConnected()) {
    _client->connect(_clientName, _token, NULL);
    Serial.print(".");
    delay(1000);
  }
  return true;
}

bool UbidotsMQTT::ubidotsPublish(char* device) {
  char topic[150];
  char payload[500];
  String str;
  sprintf(topic, "%s%s", FIRST_PART_TOPIC, device);
  sprintf(payload, "{");
  for (int i = 0; i <= _currentValue;) {
    str = String((val + i)->_value, 2);
    sprintf(payload, "%s\"%s\": [{\"value\": %s", payload,
            (val + i)->_variableLabel, str.c_str());
    if ((val + i)->_timestamp != NULL) {
      sprintf(payload, "%s, \"timestamp\": %lu", payload,
              (val + i)->_timestamp);
    }
    if ((val + i)->_context != NULL) {
      sprintf(payload, "%s, \"context\": {%s}", payload, (val + i)->_context);
    }
    i++;
    if (i >= _currentValue) {
      sprintf(payload, "%s}]}", payload);
      break;
    } else {
      sprintf(payload, "%s}], ", payload);
    }
  }
  if (_debug) {
    Serial.println("publishing to TOPIC: ");
    Serial.println(topic);
    Serial.print("JSON dict: ");
    Serial.println(payload);
  }
  _currentValue = 0;
  return _client->publish(topic, payload);
}

bool UbidotsMQTT::ubidotsSubscribe(char* deviceLabel, char* variableLabel) {
  char topic[150];
  sprintf(topic, "%s%s/%s/lv", FIRST_PART_TOPIC, deviceLabel, variableLabel);
  if (_debug) {
    Serial.println("Subscribing to: ");
    Serial.println(topic);
  }
  return _client->subscribe(topic);
}

void UbidotsMQTT::ubidotsSetBroker(char* broker) {
  if (_debug) {
    Serial.print("setting broker to: ");
    Serial.print(broker);
  }
  _server = broker;
}

void UbidotsMQTT::ubidotsSetDebug(bool debug) { _debug = debug; }
