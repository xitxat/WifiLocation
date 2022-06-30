
#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <WiFi.h>


#include <WifiLocation.h>
#include <bingMapsGeocoding.h>

#if __has_include("wificonfig.h")
#include "wificonfig.h"
#else
const char* googleApiKey = "YOUR_GOOGLE_API_KEY";
const char* ssid = "SSID";
const char* passwd = "PASSWD";
const char* bingMapsKey = "YOUR_BING_MAPS_KEY";
#endif

WifiLocation location (googleApiKey);
BingGeoCoding bingGeoCoding (bingMapsKey);

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

    location_t loc = location.getGeoFromWiFi ();

    Serial.println ("Location request data");
    Serial.println (location.getSurroundingWiFiJson () + "\n");
    Serial.println ("Location: " + String (loc.lat, 7) + "," + String (loc.lon, 7));
    //Serial.println("Longitude: " + String(loc.lon, 7));
    Serial.println ("Accuracy: " + String (loc.accuracy));
    Serial.println ("Geolocation result: " + location.wlStatusStr (location.getStatus ()));

    bingGeoData_t geoData = bingGeoCoding.getGeoFromPosition (loc.lat, loc.lon);

    Serial.println ("Geocoding result: " + bingGeoCoding.bgStatusStr (bingGeoCoding.getStatus ()));
    Serial.printf ("Address: %s\n", bingGeoCoding.getCompleteAddress (&geoData).c_str ());

}

void loop () {

}
