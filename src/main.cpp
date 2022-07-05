#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include "creds.h"
#include "secrets.h"

const char* hostName = "lazyBoy";

WiFiClient client;

boolean triggered = false;

byte PIN_RELAY_A = D2;

int data = 0;
int lastData = 0;
int call_interval = 15250;
int one_second = 1000;
int api_delay = call_interval;
int statusCode = 0;

unsigned long curMillis = millis();
unsigned long lastReadMillis = curMillis;
unsigned long lastWriteMillis = curMillis;
unsigned long lastTriggerMillis = curMillis;

void relayStop() {
  digitalWrite(PIN_RELAY_A, LOW);
}

void relayStart() {
  digitalWrite(PIN_RELAY_A, HIGH);
}

boolean reclining() {
  return (digitalRead(PIN_RELAY_A) == HIGH) ? true : false;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_RELAY_A, OUTPUT);
  
  relayStop();

  WiFi.hostname(hostName);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    os_printf("STA: Failed!\n");
  }
  
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);  

  ThingSpeak.setField(1, data);

  statusCode = ThingSpeak.writeFields(myChannelNumber, WRITE_API_KEY);

  lastWriteMillis = millis();

  if(statusCode == 200){         
      Serial.printf("Wrote: Field %d value: %d at %d\n", 1, data, lastWriteMillis);
  }

  statusCode = ThingSpeak.readMultipleFields(myChannelNumber, READ_API_KEY);
  
  lastReadMillis = millis();

  if(statusCode == 200){   
      data = ThingSpeak.getFieldAsInt(1);      
      Serial.printf("Read: Field %d value: %d\n", 1, data);
  }

  if(data == 0) {
    Serial.printf("Startup OK\n");
  }

}


void loop() {
  
  curMillis = millis();
  
  if(curMillis - lastTriggerMillis >= 5 * one_second && triggered){
    triggered = false;
    if(reclining()){
      relayStop();
      Serial.println("Stopping relay...\n");
    }
  }

  if(curMillis - lastReadMillis >= one_second) {

    if((millis() - lastWriteMillis) <= api_delay) {
      Serial.printf("API writing availabe in %ds...\n", (api_delay - (millis() - lastWriteMillis)) / 1000);
    } else {
      Serial.printf("API available for writing...\n");
    }

    statusCode = ThingSpeak.readMultipleFields(myChannelNumber, READ_API_KEY);

    if(statusCode == 200){   
      data = ThingSpeak.getFieldAsInt(1);
      lastData = data;
      Serial.printf("Field %d value: %d\n", 1, data);      
    }    

    lastReadMillis = millis();
  
    if(data == 1 && !triggered) {
      Serial.printf("Starting recline...\n");
      triggered =  true;
      
      lastTriggerMillis = millis();
      lastWriteMillis = millis();
    
      if(!reclining()){
        relayStart();
      }

      data = 0; // set stop

      ThingSpeak.setField(1, data);

      int _delay = api_delay - (millis() - lastWriteMillis);
      Serial.printf("Delaying for %ds...\n", _delay / 1000);
      delay(_delay);
      
      statusCode = ThingSpeak.writeFields(myChannelNumber, WRITE_API_KEY);
      
      lastWriteMillis = millis();
      
      if(statusCode == 200){         
        Serial.printf("Wrote: Field %d value: %d at %d\n", 1, data, lastWriteMillis);
      }    

    }
  }
    
  if(curMillis - lastWriteMillis >= api_delay && lastData == 1 && triggered){    
    
    if(reclining()){
      relayStop();
    }
    
  }
}
