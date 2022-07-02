
#include <Arduino.h>
#include <CayenneMQTTESP8266.h>
#include "Sensors.h"
#include <SimpleTimer.h>
#include "CayenneConfig.h"


/*     Cayenne   */
void transmitData(){            //  Cayenne LPP
	//Cayenne.virtualWrite(0, millis());
Cayenne.celsiusWrite(1, cur180Temp);                //gauge  BMP 180
Cayenne.hectoPascalWrite(2, calToSeaPres);			//gauge  BMP 180. relative. converted.
Cayenne.celsiusWrite(3, cur180Temp);                //Chart  BMP 180
Cayenne.hectoPascalWrite(4, calToSeaPres);			//Chart  BMP 180. relative. converted.

//Cayenne.virtualWrite(V4, ahtHumid,"bp","hpa");

}

void initCayenne(){
         Cayenne.begin(username, password, clientID); //, ssid, wifiPassword
}

void runCayenne(){
 Cayenne.loop();
}