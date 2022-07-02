#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WifiLocation.h>
#include "wificonfig.h"
#include <millisDelay.h>
#include <Wire.h> //  I2C

unsigned long loopCounter = 10;


        /*  GEO LOCATION     */

void setClock () {          // Set time via NTP, as required for x.509 validation
    configTime (0, 0, "pool.ntp.org", "time.nist.gov");

    Serial.print ("Waiting for NTP time sync: ");
    time_t now = time (nullptr);
    while (now < 8 * 3600 * 2) {
        delay (500);
        Serial.print (".");
        now = time (nullptr);
    }
    struct tm timeinfo;
    gmtime_r (&now, &timeinfo);
    Serial.print ("\n");
    Serial.print ("Current time: ");
    Serial.print (asctime (&timeinfo));
}

        /*  WiFi            */

void initWiFi(){    // Connect to WPA/WPA2 network

    WiFi.mode (WIFI_STA);
    WiFi.begin (ssid, passwd);
    while (WiFi.status () != WL_CONNECTED) {
        Serial.print ("Attempting to connect to WPA SSID: ");
        Serial.println (ssid);
        // wait 5 seconds for connection:
        Serial.print ("Status = ");
        Serial.println (WiFi.status ());
        delay (500);
    }
    Serial.println ("Connected");


}

        /*  SCANS            */

void scanI2cBus()
{
    byte error, address;
    int nDevices;
    Serial.println(" ");
    Serial.println("~~~~~~ I2C Scanning...");
    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0)
        {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.println(address, HEX);
            nDevices++;
        }
        else if (error == 4)
        {
            Serial.print("Unknow error at address 0x");
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
    {
        Serial.println("No I2C devices found\n");
    }
    else
    {
        Serial.println("done\n");
    }
    delay(200);
}



        /*  BLINKS           */  // *   *   *   *   *

        /*  blinks with delay  */
void loopBlink()
{
    loopCounter--;
    if (loopCounter == 0)
    {
        loopCounter = 10;
    }
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    Serial.print("LOOP ");
    Serial.println(loopCounter);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    Serial.println(" ");
}


        /*  blink sequence  */  // milliDelay
                                //re:startSequence()!PullUp  low =    on,  off,  
unsigned long onOffDelays[] = {200, 200, 200, 200, 200, 2000};
unsigned long WiFiDelaySequence[] = {200, 200, 200, 200, 200, 2000};

const int NUMBER_OF_STEPS = 6; // can be smaller the number of elements in onOffDelays but not larger
int stepIdx = 0;
bool ledOn = false;

        /*  mD objects      */
millisDelay ledOnOffDelay;      
millisDelay wiFiDelay;          


        /*  mD functions */
void startSequence() {
  stepIdx = 0;
  ledOnOffDelay.start(onOffDelays[stepIdx]);
  wiFiDelay.start(WiFiDelaySequence[stepIdx]);
  digitalWrite(LED_BUILTIN, LOW); // TURN led ON / off for first step
  ledOn = true;
}

void toggleLedSequence() {
  if (ledOnOffDelay.justFinished()) { // don't combine this test with any other condition
    // on delay timed out
    // toggle led
    ledOn = !ledOn;
    if (ledOn) {
      digitalWrite(LED_BUILTIN, LOW); // turn led on
    } else {
      digitalWrite(LED_BUILTIN, HIGH); // turn led off
    }
    stepIdx++;
    if (stepIdx >= NUMBER_OF_STEPS) {
      stepIdx = 0; // repeat sequence
    }
    ledOnOffDelay.start(onOffDelays[stepIdx]);
  }
}

void blinkWiFi() {
  if (wiFiDelay.justFinished()) { // don't combine this test with any other condition
    // on delay timed out
    // toggle led
    ledOn = !ledOn;
    if (ledOn) {
      digitalWrite(LED_BUILTIN, LOW); // turn led on
    } else {
      digitalWrite(LED_BUILTIN, HIGH); // turn led off
    }
    stepIdx++;
    if (stepIdx >= NUMBER_OF_STEPS) {
      stepIdx = 0; // repeat sequence
    }
    wiFiDelay.start(WiFiDelaySequence[stepIdx]);
  }
}



