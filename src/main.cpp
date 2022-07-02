
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WifiLocation.h>
#include <millisDelay.h>
#include <PinFlasher.h>
#include "Functions.h"
#include <Sensors.h>
#include <Wire.h> //  I2C
#include <SimpleTimer.h>
#include "MqttSend.h"

// // Timer
SimpleTimer timer;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  startSequence(); // Blink controller
  Wire.begin(D2, D1); // join i2c bus with SDA=D2 and SCL=D1 of NodeMCU
  scanI2cBus();
  initBMP180();
  initWiFi();
  setClock();
  initGoogleLoc();
  initCayenne();
  timer.setInterval(1000L, transmitData); // Send Cayenne data every N L's
  
} // setup

void loop()
{

  runBMP180();
  printGPS();

  runCayenne();
  timer.run();
}