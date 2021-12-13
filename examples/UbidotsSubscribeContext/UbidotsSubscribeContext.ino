/****************************************
 * Include Libraries
 ****************************************/

#include <math.h>
#include <UbidotsMQTT.h>

/****************************************
 * Define Constants
 ****************************************/

#ifndef TOKEN
#define TOKEN "token"  // Ubidots token
#endif

#define VARIABLE_LABEL "my-var"
#define DEVICE_LABEL "my-dev"
#define LOOP_PERIOD_SEC 5

/****************************************
 * Globals
 ****************************************/

UbidotsMQTT clientMQTT(TOKEN, callback);

bool subscribedToVar = false;
char *ssid = NULL;
char bssid[2*6 + 5 + sizeof("")] = "";
char context[64 + sizeof("")] = "";

/****************************************
 * User Functions
 ****************************************/

void callback(char* topic, uint8_t* payload, unsigned int length) {
  Serial.println("--------------------------------");
  
  Serial.printlnf("Message from '%s': %*s", topic, length, payload);
  
  char *jsonStr = (char *) malloc(length*sizeof(*payload) + sizeof(""));
  sprintf(jsonStr, "%*s", length, payload);
  
  float varValue = NAN;
  const char *ctxSsid = NULL;
  const char *ctxBssid = NULL;
  
  JSONValue rootObj = JSONValue::parse(jsonStr, strlen(jsonStr));
  JSONObjectIterator rootIter(rootObj);
  while (rootIter.next()) {
    const char *key = (const char *) rootIter.name();
    JSONValue val = rootIter.value();
    
    if (0 == strcmp("value", key)) {
      varValue = val.toDouble();
    }
    else if (0 == strcmp("context", key)) {
      JSONObjectIterator ctxIter(val);
      while (ctxIter.next()) {
        const char *ctxKey = (const char *) ctxIter.name();
        JSONValue ctxVal = ctxIter.value();
        
        if (0 == strcmp("ssid", ctxKey)) {
          ctxSsid = (const char *) ctxVal.toString();
        }
        else if (0 == strcmp("bssid", ctxKey)) {
          ctxBssid = (const char *) ctxVal.toString();
        }
      }
    }
  }
  
  Serial.printlnf("Value: %f", varValue);
  Serial.printlnf("Context SSID: %s", ctxSsid);
  Serial.printlnf("Context BSSID: %s", ctxBssid);
  
  Serial.println("--------------------------------");
  
  free(jsonStr);
}

void onConnect() {
  if (!subscribedToVar) {
    // Subscribe to variable
    subscribedToVar = clientMQTT.ubidotsSubscribe(
      DEVICE_LABEL,
      VARIABLE_LABEL,
      false
    );
  }
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);

  // Comment below line to disable debug messages.
  clientMQTT.ubidotsSetDebug(true);

  // Connect to broker.
  clientMQTT.connect(5);
  if (clientMQTT.isConnected()) {
    onConnect();
  }
  
  ssid = strdup(WiFi.SSID());
  byte buff[6];
  WiFi.BSSID(buff);
  snprintf(bssid, sizeof(bssid), "%02X:%02X:%02X:%02X:%02X:%02X", buff[0], buff[1], buff[2], buff[3], buff[4], buff[5]);
}

void loop() {
  if (!clientMQTT.isConnected()) {
    clientMQTT.connect(5);
    if (clientMQTT.isConnected()) {
      onConnect();
    }
  }
  
  // Publish a value with context
  WiFiSignal signal = WiFi.RSSI();
  float strength = signal.getStrengthValue();
  
  clientMQTT.addContext("ssid", ssid);
  clientMQTT.addContext("bssid", bssid);
  clientMQTT.getContext(context);
  
  clientMQTT.add(VARIABLE_LABEL, strength, context);
  clientMQTT.ubidotsPublish(DEVICE_LABEL);
  
  clientMQTT.loop();
  delay(1000*LOOP_PERIOD_SEC);
}
