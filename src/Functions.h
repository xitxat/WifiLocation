#ifndef FUNCTIONS_H
#define FUNCTIONS_H



/*     FCN protos   */

void setClock ();
void initGoogleLoc();
void printGPS();
void initWiFi();
void scanI2cBus();

void loopBlink();
/*     mD Blink  protos   */

void startSequence();
void toggleLedSequence();
void blinkWiFi();

#endif