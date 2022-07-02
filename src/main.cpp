
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WifiLocation.h>
#include "GeoKeys.h"
#include <millisDelay.h>
#include "Functions.h"
#include <Sensors.h>
#include <Wire.h> //  I2C

float latitude{0.0};
float longitude{0.0};
WifiLocation location (googleApiKey);



void setup () {
    Serial.begin (115200);
    pinMode(LED_BUILTIN, OUTPUT);

    startSequence();            // Blink controller

    Wire.begin(D2, D1);         // join i2c bus with SDA=D2 and SCL=D1 of NodeMCU

    scanI2cBus();

    initBMP180();

    initWiFi();

    setClock ();

    // Google Loc
    location_t loc = location.getGeoFromWiFi();

    Serial.println("Location request data");
    Serial.println(location.getSurroundingWiFiJson()+"\n");
    Serial.println ("Location: " + String (loc.lat, 7) + "," + String (loc.lon, 7));
    //Serial.println("Longitude: " + String(loc.lon, 7));
    Serial.println ("Accuracy: " + String (loc.accuracy));
    Serial.println ("Result: " + location.wlStatusStr (location.getStatus ()));

    latitude = loc.lat;
    longitude = loc.lon;
}

void loop () {
    //loopBlink();
    //blinkWiFi();                // Blue led setup signal

  runBMP180();

    Serial.println (" " );
    Serial.println ("~~~~   Location:" );
    Serial.print ("longitudegitude: " );
    Serial.println (longitude );
    Serial.print ("Latitude:  " );
    Serial.println (latitude );
    Serial.println (" " );
//delay(10000);
}