
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WifiLocation.h>
#include <millisDelay.h>
#include <PinFlasher.h>
#include "Functions.h"
#include <Sensors.h>
#include <Wire.h> //  I2C
#include <CayenneMQTTESP8266.h>
#include <SimpleTimer.h>

#define CAYENNE_PRINT Serial
// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "4c8b8bd0-7d42-11ec-8da3-474359af83d7";
char password[] = "caa9cff4a68e7323b043b5d165f94b62bd4dd670";
char clientID[] = "28605410-f9f8-11ec-8df2-dd50487e509b";

// Timer
SimpleTimer timer;

void transmitData(){            //  Cayenne LPP
	//Cayenne.virtualWrite(0, millis());
Cayenne.celsiusWrite(1, cur180Temp);            //gauge  BMP 180
Cayenne.hectoPascalWrite(2, calToSeaPres);			//gauge  BMP 180. relative. converted.
Cayenne.celsiusWrite(3, cur180Temp);            //Chart  BMP 180
Cayenne.hectoPascalWrite(4, calToSeaPres);			//Chart  BMP 180. relative. converted.

//Cayenne.virtualWrite(V4, ahtHumid,"bp","hpa");

}

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

     Cayenne.begin(username, password, clientID); //, ssid, wifiPassword
      timer.setInterval(1000L, transmitData); // Method to execute every 200ms
}




void loop () {
    //loopBlink();
    //blinkWiFi();                // Blue led setup signal

  runBMP180();
  printGPS();

 Cayenne.loop();
 timer.run();

//delay(10000);
}