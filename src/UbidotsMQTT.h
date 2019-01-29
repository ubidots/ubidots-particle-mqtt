/*
Copyright (c) 2013-2018 Ubidots.
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

Developed and maintained by Jose Garcia for IoT Services Inc
@jotathebest at github: https://github.com/jotathebest
*/

#ifndef UbidotsMQTT_H
#define UbidotsMQTT_H

#include "MQTT.h"
#include "application.h"

const uint8_t MAX_VALUES = 5;
const uint16_t MAX_BUFFER_SIZE = 700;
const char FIRST_PART_TOPIC[15] = "/v1.6/devices/";

typedef struct Dot {
  char *_context;
  float _value;
  char *_variableLabel;
  unsigned long _timestamp;
  uint16_t _timestampMillis;
} Dot;

typedef struct ContextUbi {
  char *key_label;
  char *key_value;
} ContextUbi;

class Ubidots {
 private:
  void (*callback)(char *, uint8_t *, unsigned int);
  MQTT *_client;
  Dot *dot;
  char *_clientName;
  bool _debug = true;
  uint8_t _currentValue;
  int8_t _current_context;
  char *_server;
  char *_token;
  ContextUbi *_context;
  bool _reconnect(uint8_t maxRetries);
  void _buildPayload(char *payload);

 public:
  Ubidots(char *token, void (*callback)(char *, uint8_t *, unsigned int));
  void add(char *variableLabel, float value);
  void add(char *variableLabel, float value, char *context);
  void add(char *variableLabel, float value, char *context,
           unsigned long timestamp);
  void add(char *variableLabel, float value, char *context,
           unsigned long timestamp, uint16_t timestampMillis);
  void addContext(char *key_label, char *key_value);
  void getContext(char *context_result);
  bool connect(uint8_t maxRetries = 0);
  bool isConnected();
  bool loop();
  bool ubidotsPublish(char *device_label);
  bool ubidotsSubscribe(char *deviceLabel, char *variableLabel);
  void ubidotsSetBroker(char *broker, uint16_t port = 1883);
  void ubidotsSetDebug(bool debug);
};

#endif