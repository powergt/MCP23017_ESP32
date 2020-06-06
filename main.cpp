#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp;

volatile bool int_toggle=0;
unsigned long t=0;

const int LED=8;
const int BUTTON=7;
const int ARDUINO_INT=25;
const int LED_ONBOARD=1;
void IRAM_ATTR mcpInterrupt(){
  int_toggle=1;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("avvio");
  mcp.begin();
  // We mirror INTA and INTB, so that only one line is required between MCP and Arduino for int reporting
  // INTs will be signaled at every CHANGE

  pinMode(LED_ONBOARD, OUTPUT);  // use ONBOARD LED as debugging
  mcp.pinMode(LED, OUTPUT);  
  mcp.pinMode(BUTTON, INPUT);  
  mcp.pullUp(BUTTON, HIGH);  // turn on a 100K pullup internally
   
  mcp.setupInterrupts(true,false,LOW);
  mcp.setupInterruptPin(BUTTON,CHANGE);
  mcp.readGPIOAB(); // Initialise for interrupts.

  pinMode(ARDUINO_INT, INPUT);
  attachInterrupt(ARDUINO_INT,mcpInterrupt,FALLING);
}



void toggle() {
mcp.digitalWrite(LED,!mcp.digitalRead(LED));

}

void loop() {
  if (int_toggle) {
  uint8_t pin = mcp.getLastInterruptPin();
  uint8_t val = mcp.getLastInterruptPinValue();
    Serial.printf("i:%d,%d\r\n",pin,val);
    toggle();
    int_toggle=0;
    //mcp.readGPIOAB(); //necessary to clear interrupt if not use  mcp.getLastInterruptPin / mcp.getLastInterruptPinValue();
  }
  if (millis()-t>1000) {
    Serial.print('.');
    t=millis();
  }

}
