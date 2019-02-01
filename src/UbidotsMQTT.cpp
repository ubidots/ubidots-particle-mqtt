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

#include "UbidotsMQTT.h"

/**************************************************************************
 * Overloaded constructors
 ***************************************************************************/
Ubidots::Ubidots(char* token, void (*callback)(char*, uint8_t*, unsigned int)) {
  _server = "industrial.api.ubidots.com";
  _token = token;
  _currentValue = 0;
  _currentContext = 0;
  String deviceId = System.deviceID();
  _clientName = new char[deviceId.length() + 1];
  strcpy(_clientName, deviceId.c_str());
  this->callback = callback;
  dot = (Dot*)malloc(MAX_VALUES * sizeof(Dot));
  this->_client = new MQTT(_server, 1883, this->callback, 512);
}

/***************************************************************************
FUNCTIONS TO SEND DATA
***************************************************************************/

/*
 * Add a value of variable to save
 * @arg variableLabel [Mandatory] variable label where the dot will be stored
 * @arg value [Mandatory] Dot's value.
 * @arg context [Optional] Dot's context to store. Default NULL
 * @arg dotTimestampSeconds [Optional] Dot's dotTimestampSeconds in seconds, usefull for
 * datalogger. Default NULL
 * @arg dotTimestampMillis [Optional] Dot dotTimestampSeconds in millis to add to
 * dotTimestampSeconds, usefull for datalogger.
 */
void Ubidots::add(char* variableLabel, float value) {
  add(variableLabel, value, NULL, NULL);
}

void Ubidots::add(char* variableLabel, float value, char* context) {
  add(variableLabel, value, context, NULL);
}

void Ubidots::add(char* variableLabel, float value, char* context,
    unsigned long dotTimestampSeconds) {
  add(variableLabel, value, context, dotTimestampSeconds, NULL);
}

void Ubidots::add(char* variableLabel, float value, char* context,
                  unsigned long dotTimestampSeconds, uint16_t dotTimestampMillis) {
  (dot + _currentValue)->_variableLabel = variableLabel;
  (dot + _currentValue)->_value = value;
  (dot + _currentValue)->_context = context;
  (dot + _currentValue)->_dotTimestampSeconds = dotTimestampSeconds;
  (dot + _currentValue)->_dotTimestampMillis = dotTimestampMillis;
  _currentValue++;
  if (_currentValue > MAX_VALUES) {
    Serial.println(
        F("You are adding more than the maximum of the allowed consecutive "
          "variables"));
    _currentValue = MAX_VALUES;
  }
}

/*
 * Sends data to Ubidots
 * @arg deviceLabel [Optional] device label where data will be sent to in Ubidots.
 */

bool Ubidots::ubidotsPublish() {
  return ubidotsPublish(_clientName);
}

bool Ubidots::ubidotsPublish(char* deviceLabel) {
  char topic[150];
  sprintf(topic, "%s%s", FIRST_PART_TOPIC, deviceLabel);
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

/***************************************************************************
FUNCTIONS TO RETRIEVE DATA
***************************************************************************/

/*
 * Retrieves data from Ubidots
 * @arg deviceLabel [Mandatory] device label to retrieve values from
 * @arg variableLabel [Mandatory] variable label to retrieve values from
 */
bool Ubidots::ubidotsSubscribe(char* deviceLabel, char* variableLabel) {
  char topic[150];
  sprintf(topic, "%s%s/%s/lv", FIRST_PART_TOPIC, deviceLabel, variableLabel);
  if (_debug) {
    Serial.printlnf("Subscribing to: %s", topic);
  }
  return _client->subscribe(topic);
}

/***************************************************************************
AUXILIAR FUNCTIONS
***************************************************************************/

/*
 * Adds to the context structure values to retrieve later it easily by the user
 */

void Ubidots::addContext(char* keyLabel, char* keyValue) {
  (_context + _currentContext)->keyLabel = keyLabel;
  (_context + _currentContext)->keyValue = keyValue;
  _currentContext++;
  if (_currentContext >= MAX_VALUES) {
    Serial.println(
        F("You are adding more than the maximum of consecutive key-values "
          "pairs"));
    _currentContext = MAX_VALUES;
  }
}

/*
 * Builds the actual stored context properly formatted
 */

void Ubidots::getContext(char* contextResult) {
  // TCP context type
  sprintf(contextResult, "{");
  for (uint8_t i = 0; i < _currentContext;) {
    sprintf(contextResult, "%s%s:%s", contextResult,
            (_context + i)->keyLabel, (_context + i)->keyValue);
    i++;
    if (i < _currentContext) {
      sprintf(contextResult, "%s,", contextResult);
    } else {
      sprintf(contextResult, "%s}", contextResult);
      _currentContext = 0;
    }
  }
}

/**
 * Builds the payload to send and saves it to the input char pointer.
 * @payload [Mandatory] char payload pointer to store the built structure.
 */

void Ubidots::_buildPayload(char* payload) {
  sprintf(payload, "{");
  for (int i = 0; i <= _currentValue;) {
    // Adds the variable label and the dot's value
    sprintf(payload, "%s\"%s\": [{\"value\": %f", payload,
            (dot + i)->_variableLabel, (dot + i)->_value);

    // Adds the context
    if ((dot + i)->_context != NULL) {
      sprintf(payload, "%s, \"context\": {%s}", payload, (dot + i)->_context);
    }

    // Adds the dotTimestampSeconds
    if ((dot + i)->_dotTimestampSeconds != NULL) {
      sprintf(payload, "%s,\"dotTimestampSeconds\":%lu", payload, (dot + i)->_dotTimestampSeconds);

      // Adds dotTimestampSeconds milliseconds
      if ((dot + i)->_dotTimestampMillis != NULL) {
        char milliseconds[3];
        int dotTimestampSecondsMillis = (dot + i)->_dotTimestampMillis;
        uint8_t units = dotTimestampSecondsMillis % 10;
        uint8_t dec = (dotTimestampSecondsMillis / 10) % 10;
        uint8_t hund = (dotTimestampSecondsMillis / 100) % 10;
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

/*
 * Returns true if the device has a socket opened
 */
bool Ubidots::isConnected() { return _client->isConnected(); }

/*
 * Opens a TCP socket to the broker
 * @maxRetries [Optional] [default=0]: Maximum number of connection attempts
 */
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

/*
 * Attempts to open a TCP socket to the broker
 * @maxRetries [Optional] [default=0]: Maximum number of connection attempts
 */
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

/*
 * Returns the MQTT loop method necessary to maintain the TCP socket opened
 */
bool Ubidots::loop() { return _client->loop(); }

/*
 * Sets a new broker and port to connect with
 * @broker [Mandatory]: Broker DNS
 * @port [Optional] [Default=1883]: TCP port to use to open the socket
 */
void Ubidots::ubidotsSetBroker(char* broker, uint16_t port) {
  if (_debug) {
    Serial.printlnf("New settings:\nBroker Url: %s\nPort: %d", broker, port);
  }
  _client->setBroker(broker, port);
}

/*
  Makes debug messages available
*/
void Ubidots::ubidotsSetDebug(bool debug) { _debug = debug; }
