
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WifiLocation.h>
#include "wificonfig.h"
#include <Sensors.h>
#include <Wire.h> //  I2C

float latitude{0.0};
float longitude{0.0};
WifiLocation location (googleApiKey);


// Set time via NTP, as required for x.509 validation
void setClock () {
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

void setup () {
    Serial.begin (115200);
    pinMode(LED_BUILTIN, OUTPUT);
    publishBlink();         // Blue led setup signal

    Wire.begin(D2, D1);     // join i2c bus with SDA=D2 and SCL=D1 of NodeMCU

    Serial.println("~~~~~~  I2C Scanner setup");
    dotDash();
    scanI2cBus();

  initBMP180();

    // Connect to WPA/WPA2 network
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
    loopBlink();
  runBMP180();

    Serial.println (" " );
    Serial.println ("~~~~   Location:" );
    Serial.print ("longitudegitude: " );
    Serial.println (longitude );
    Serial.print ("Latitude:  " );
    Serial.println (latitude );
    Serial.println (" " );
delay(1000);
}