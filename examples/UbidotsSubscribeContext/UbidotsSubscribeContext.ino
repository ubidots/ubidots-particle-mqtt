#include <string.h>
#include <math.h>

#include <UbidotsMQTT.h>

/** How often the main loop is executed. It is a busy delay loop. */
#define LOOP_PERIOD_SEC 5

/**
 * Function called by the MQTT client when a message is received.
 * 
 * @param topic[in] Topic that cased the callback to be called
 * @param payload[in] Message payload bytes
 * @param length[in] Message payload size in bytes
 */
void callback(char* topic, uint8_t* payload, unsigned int length);

/** Tasks to be done when MQTT client connects to the broker. */
void onConnect();

/** Your Ubidots account token */
char UBIDOTS_TOKEN[] = "my-token";
/** Device label to send data to */
char DEVICE_LABEL[] = "my-dev";
/** Variable label to send data to */
char VARIABLE_LABEL[] = "my-var";
/** MQTT client instance */
UbidotsMQTT clientMQTT(UBIDOTS_TOKEN, callback);

/** Perform initialization tasks */
void setup() {
  Serial.begin(115200);

  // Comment below line to disable debug messages.
  clientMQTT.ubidotsSetDebug(true);

  // Connect to broker.
  clientMQTT.connect(5);
  if (clientMQTT.isConnected()) {
    onConnect();
  }
}

/** Tasks to be done over and over... */
void loop() {
  char context[64 + sizeof("")] = "";
  int value = random();
  
  if (!clientMQTT.isConnected()) {
    clientMQTT.connect(5);
    if (clientMQTT.isConnected()) {
      onConnect();
    }
  }
  
  // Publish a value with context so the example is self-contained.
  clientMQTT.addContext("ctx_key_0", "ctx_val_0");
  clientMQTT.addContext("ctx_key_1", "ctx_val_1");
  clientMQTT.getContext(context);
  
  clientMQTT.add(VARIABLE_LABEL, value, context);
  clientMQTT.ubidotsPublish(DEVICE_LABEL);
  
  clientMQTT.loop();
  delay(1000*LOOP_PERIOD_SEC);
}

void callback(char* topic, uint8_t* payload, unsigned int length) {
  /**
   * @note For the sake of simplicity, this callback only handles received
   * payload as a serialized JSON object. Make sure you subscribe only to 
   * complete dots, e.g. clientMQTT.ubidotsSubscribe("<device>", "<variable>", false),
   * or make proper adjustments to support last value.
   */
  
  Serial.println("--------------------------------");
  
  Serial.printlnf("Message from '%s': %.*s", topic, length, payload);
  
  char *jsonStr = strndup((char *) payload, length);
  
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
        
        if (0 == strcmp("ctx_key_0", ctxKey)) {
          ctxSsid = (const char *) ctxVal.toString();
        }
        else if (0 == strcmp("ctx_key_1", ctxKey)) {
          ctxBssid = (const char *) ctxVal.toString();
        }
      }
    }
  }
  
  Serial.printlnf("Variable value: %f", varValue);
  Serial.printlnf("Context ctx_key_0: %s", ctxSsid);
  Serial.printlnf("Context ctx_key_1: %s", ctxBssid);
  
  Serial.println("--------------------------------");
  
  free(jsonStr);
}

void onConnect() {
  // Subscribe to variable.
  clientMQTT.ubidotsSubscribe(
    DEVICE_LABEL,
    VARIABLE_LABEL,
    false // Set to false to receive full dot on payload as a serialized JSON object
  );
}
