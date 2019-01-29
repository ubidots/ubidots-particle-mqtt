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

Ubidots::Ubidots(char* token, void (*callback)(char*, uint8_t*, unsigned int)) {
  _server = "industrial.ubidots.com";
  _token = token;
  _currentValue = 0;
  String deviceId = System.deviceID();
  _clientName = new char[deviceId.length() + 1];
  strcpy(_clientName, deviceId.c_str());
  this->callback = callback;
  dot = (Dot*)malloc(MAX_VALUES * sizeof(Dot));
  this->_client = new MQTT(_server, 1883, this->callback, 512);
}

void Ubidots::add(char* variableLabel, float value) {
  add(variableLabel, value, NULL, NULL);
}

void Ubidots::add(char* variableLabel, float value, char* context) {
  add(variableLabel, value, context, NULL);
}

void Ubidots::add(char* variableLabel, float value, char* context,
                  unsigned long timestamp) {
  add(variableLabel, value, context, timestamp, NULL);
}

void Ubidots::add(char* variableLabel, float value, char* context,
                  unsigned long timestamp, uint16_t timestampMillis) {
  (dot + _currentValue)->_variableLabel = variableLabel;
  (dot + _currentValue)->_value = value;
  (dot + _currentValue)->_context = context;
  (dot + _currentValue)->_timestamp = timestamp;
  (dot + _currentValue)->_timestampMillis = timestampMillis;
  _currentValue++;
  if (_currentValue > MAX_VALUES) {
    Serial.println(
        F("You are adding more than the maximum of the allowed consecutive "
          "variables"));
    _currentValue = MAX_VALUES;
  }
}

bool Ubidots::isConnected() { return _client->isConnected(); }

bool Ubidots::connect(uint8_t maxRetries) {
  bool connected = false;
  _client->connect(_clientName, _token, NULL);
  if (!_client->isConnected()) {
    Serial.println("trying to connect to broker");
    connected = _reconnect(maxRetries);
  }

  if (_debug && connected) {
    Serial.println("connected");
  }

  return connected;
}

bool Ubidots::loop() { return _client->loop(); }

bool Ubidots::_reconnect(uint8_t maxRetries) {
  uint8_t retries = 0;
  while (!_client->isConnected()) {
    _client->connect(_clientName, _token, NULL);
    Serial.print(".");

    // 255 is the max 8-bit integer value
    if (maxRetries == retries || retries == 255) {
      break;
    }

    retries += 1;
    delay(100);  // Waits 100 ms before of trying to open a new socket
  }
  return _client->isConnected();
}

void Ubidots::_buildPayload(char* payload) {
  sprintf(payload, "{");
  for (int i = 0; i <= _currentValue;) {
    sprintf(payload, "%s\"%s\": [{\"value\": %f", payload,
            (dot + i)->_variableLabel, (dot + i)->_value);

    if ((dot + i)->_context != NULL) {
      sprintf(payload, "%s, \"context\": {%s}", payload, (dot + i)->_context);
    }

    if ((dot + i)->_timestamp != NULL) {
      sprintf(payload, "%s,\"timestamp\":%lu", payload, (dot + i)->_timestamp);
      // Adds timestamp milliseconds
      if ((dot + i)->_timestampMillis != NULL) {
        char milliseconds[3];
        int timestamp_millis = (dot + i)->_timestampMillis;
        uint8_t units = timestamp_millis % 10;
        uint8_t dec = (timestamp_millis / 10) % 10;
        uint8_t hund = (timestamp_millis / 100) % 10;
        sprintf(milliseconds, "%d%d%d", hund, dec, units);
        sprintf(payload, "%s%s", payload, milliseconds);
      } else {
        sprintf(payload, "%s000", payload);
      }
    }

    i++;
    if (i >= _currentValue) {
      sprintf(payload, "%s}]}", payload);
      break;
    } else {
      sprintf(payload, "%s}], ", payload);
    }
  }
}

bool Ubidots::ubidotsPublish(char* device) {
  char topic[150];
  sprintf(topic, "%s%s", FIRST_PART_TOPIC, device);
  char* payload = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
  _buildPayload(payload);

  if (_debug) {
    Serial.printlnf("publishing to TOPIC: %s\nJSON dict: %s", topic, payload);
  }
  _currentValue = 0;
  bool result = _client->publish(topic, payload);
  free(payload);
  return result;
}

bool Ubidots::ubidotsSubscribe(char* deviceLabel, char* variableLabel) {
  char topic[150];
  sprintf(topic, "%s%s/%s/lv", FIRST_PART_TOPIC, deviceLabel, variableLabel);
  if (_debug) {
    Serial.printlnf("Subscribing to: %s", topic);
  }
  return _client->subscribe(topic);
}

void Ubidots::ubidotsSetBroker(char* broker, uint16_t port) {
  if (_debug) {
    Serial.printlnf("New settings:\nBroker Url: %s\nPort: %d", broker, port);
  }
  _client->setBroker(broker, port);
}

void Ubidots::ubidotsSetDebug(bool debug) { _debug = debug; }
