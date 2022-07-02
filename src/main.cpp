
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WifiLocation.h>
#include <millisDelay.h>
#include <PinFlasher.h>
#include "Functions.h"
#include <Sensors.h>
#include <Wire.h> //  I2C




void setup () {
    Serial.begin (115200);
    pinMode(LED_BUILTIN, OUTPUT);

    startSequence();            // Blink controller

    Wire.begin(D2, D1);         // join i2c bus with SDA=D2 and SCL=D1 of NodeMCU

    scanI2cBus();

    initBMP180();

    initWiFi();

    setClock ();

initGoogleLoc();


}

void loop () {
    //loopBlink();
    //blinkWiFi();                // Blue led setup signal

  runBMP180();
  printGPS();



//delay(10000);
}