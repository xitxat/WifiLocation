#include <Arduino.h>
#include "Sensors.h"
#include <Wire.h>
        //#include <Adafruit_AHTX0.h>  //  10 & 20
#include <Adafruit_BMP085.h> // includes BMP180
#include <OneWire.h>
        //#include <DallasTemperature.h>

/*  NOTES
    Dust sensor is  5 Volts
    I2C:    SDA=D2 and SCL=D1
*/

/*  DUST SENSOR */
unsigned long dustDuration;
unsigned long dustStartTime;             // millis timeer
unsigned long dustSampletime_ms = 30000; // sample period 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

/*  TURBIDITY SENSOR */
int digitalTurbidVal = 0;                  //  Turbidity sensor init val.
unsigned long turbidStartTime;             // millis timeer
unsigned long turbidSampletime_ms = 30000; // sample period 30s ;

/*  LDR SENSOR  */
const int PIN_LDR = A0; // Light sensor with 10kOhm res.

int analogPinVal; // Analog value from the sensor
int lux;          // Lux value

/*  AHT Temp Humidity   */
float ahtHumid; // AHT 2x
float ahtTemp;  // AHT 2x

/* BMP 180 Temp Pressure hPa    */
float abPres;       // raw value
float calToSeaPres; // convert raw to relative
float cur180Temp;   // BMP180

/*  DALLAS  */
int numberOfDevices; // Number of Dallas temperature devices found
float coreTemp;

/*  CREATE  */
        //Adafruit_AHTX0 aht;
Adafruit_BMP085 bmp;

OneWire oneWire(ONE_WIRE_BUS);
        // DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature sensor
        // DeviceAddress tempDeviceAddress;     // We'll use this variable to store a found device address
        // DeviceAddress wildHiveCoreProbe = {0x28, 0x00, 0x3D, 0x1C, 0x26, 0x4C, 0x0E, 0xD0};  //  Extended fcns avail. ref:WildHive project
        //                                                                                 //  Blue wrap. Not waterproof.
/*  FUNCTIONS */


void publishBlink()
{
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
        digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
}

void ledFlip(int pin)        //  BLUE led for water level
{                            //  Yellow led for moisture
    digitalWrite(pin, HIGH); //
    delay(200);              //   RED water dirty
    digitalWrite(pin, LOW);
    delay(200);
}

  void dotDash()
  {
  Serial.print(".");
  delay(250);
  Serial.print("..");
  delay(250);
  Serial.print("...");
  delay(250);
  Serial.print("....");
  delay(250);
  Serial.println("*");
  }

void initDust()
{
    dustStartTime = millis(); // get the current time for Dust Sensor;
}

void initTurb()
{
    turbidStartTime = millis(); // get the current time for Dust Sensor;
}

void runDust()
{
    dustDuration = pulseIn(PIN_DUST, LOW);
    lowpulseoccupancy = lowpulseoccupancy + dustDuration;

    if ((millis() - dustStartTime) > dustSampletime_ms) // if the sample time == 30s
    {
        ratio = lowpulseoccupancy / (dustSampletime_ms * 10.0);                         // Integer percentage 0=>100
        concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve
        Serial.println(" ");
        Serial.println("~~~ Dust Sensor ~~~");
        Serial.print("Low Pulse Occupancy: ");
        Serial.println(lowpulseoccupancy);
        Serial.print("              Ratio: ");
        Serial.println(ratio);
        Serial.print("      Concentration: ");
        Serial.println(concentration);
        Serial.println(" ");
        lowpulseoccupancy = 0;
        dustStartTime = millis();
    }
}

void runTurbidity()
{
    if ((millis() - turbidStartTime) > turbidSampletime_ms) // if the sample time == 30s
    {
        Serial.println("~~~ Turbidity Sensor ~~~ ");
        digitalTurbidVal = digitalRead(PIN_TURB);
        Serial.print("Raw pin: ");
        Serial.println(digitalTurbidVal);

        if (digitalTurbidVal == 0)
        {
            ledFlip(LED_TURBID);
            Serial.println("Water is too dirty.");
        }
        else
        {
            digitalWrite(LED_TURBID, LOW);
        }
        Serial.println(" ");
        turbidStartTime = millis();
    }
}

int sensorRawToPhys(int raw) //  analog read to Lux
{
    // Conversion rule
    float Vout = float(raw) * (VIN / float(1023)); // Conversion analog to voltage
    float RLDR = (R * (VIN - Vout)) / Vout;        // Conversion voltage to resistance
    int phys = 500 / (RLDR / 1000);                // Conversion resitance to lumen
    return phys;
}

void runLDR()
{
    analogPinVal = analogRead(PIN_LDR);
    if (analogPinVal == 1024) //(analogRead(PIN_LDR) < 1024)
    {
        analogPinVal = 1023;
    }
    lux = sensorRawToPhys(analogPinVal);

    Serial.print("Raw value from sensor= ");
    Serial.println(analogPinVal); // the analog reading
    Serial.print("Physical value from sensor = ");
    Serial.print(lux);        // the analog reading
    Serial.println(" lumen"); // the analog reading
    delay(DELAY_LDR);
}

        // void initAHT()
        // {
        //     Serial.println();
        //     Serial.println("init AHT");

        //     if (!aht.begin())
        //     {
        //         Serial.println("Could not find AHT. Check wiring");
        //         while (1)
        //             delay(10);
        //     }
        //     Serial.println("AHT10 or AHT20 found");
        // }

        // void runAHT()
        // {
        //     sensors_event_t humidity, temp;
        //     aht.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data

        //     ahtTemp = temp.temperature;
        //     ahtHumid = humidity.relative_humidity;

        //     Serial.println();
        //     Serial.println("~~~~~~ AHT temp & humidity");
        //     Serial.print("AHT Outside Temperature: ");
        //     Serial.print(temp.temperature);
        //     Serial.println(" degrees C");
        //     Serial.print("AHT Humidity: ");
        //     Serial.print(humidity.relative_humidity);
        //     Serial.println("% rH");
        //     Serial.println();
        // }


void initBMP180()
{
    if (!bmp.begin())
    {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1)
        {
        }
    }

    Serial.println("BMP 180 found");
}

void runBMP180()
{
    // absol Pres to relative Pres:  SLpressure_mB = (((pressure)/pow((1-((float)(ELEVATION))/44330), 5.255))/100.0)
    cur180Temp = bmp.readTemperature();
    abPres = bmp.readPressure();

    Serial.println("~~~~~~  BMP 180 Calibrated ");
    calToSeaPres = (((abPres) / pow((1 - ((float)(1040)) / 44330), 5.255)) / 100.0); // 1040 ALTITUDE

    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(calToSeaPres);
    Serial.println(" mBar");
    Serial.println(" ");

    //  from ESP Lounge
    // abPres = bmp.readPressure() / 100.0;
    // calToSeaPres = abPres * exp(1040 / (29.3 * (abPres + 300))); //  273.15

    // from Net
    // calToSeaPres = abPres + 1040 / 8.3;
    // Serial.println(calToSeaPres);
    // Serial.println(" ");

    // from Ada Lib
    // Serial.println(" ");
    // Serial.println("~~~~~~  BMP 180");
    // Serial.print("Temperature = ");
    // Serial.print(bmp.readTemperature());
    // Serial.println(" *C");

    // Serial.print("Pressure = ");
    // Serial.print(bmp.readPressure());
    // Serial.println(" Pa");
    // Serial.println(" ");
}

        // void initDallas()
        // {
        //     Serial.println("~~~~~~  DALLAS setup");
        //     Serial.println(" ");
        //     sensors.begin(); // Start the DS18B20 sensor

        //     Serial.println("~~~~~~  OneWire Scan");
        //     oneWireScanner();

        //     Serial.println(" ");
        // }

        // void printAddress(DeviceAddress deviceAddress) // function to print a device address
        // {
        // for (uint8_t i = 0; i < 8; i++)
        // {
        //     Serial.print("0x");
        //     // zero pad the address if necessary

        //     if (deviceAddress[i] < 16)
        //     Serial.print("0"); // <16
        //     Serial.print(deviceAddress[i], HEX);

        //     if (i < 7)
        //     {
        //     Serial.print(", ");
        //     }
        // }
        // }

        // void runDallasByIndex()
        // {
        //     sensors.requestTemperatures();
        //     float temperatureC = sensors.getTempCByIndex(0);
        //     Serial.println("DALLAS Hive Core Temperature:");

        //     Serial.print(temperatureC);
        //     Serial.print("ºC");
        //         Serial.println("    (Device index 0)");
        //     Serial.println(" ");

        //     delay(1000);
        // }

        // void oneWireScanner()
        // {
        //     // locate devices on the bus
        //     Serial.print("Locating devices...");
        //     Serial.print("Found ");
        //     Serial.print(sensors.getDeviceCount(), DEC);
        //     Serial.println(" devices.");
        //     Serial.println(" ");
        // delay(500);

        //     // Grab a count of devices on the wire
        //     numberOfDevices = sensors.getDeviceCount();
        //     if (numberOfDevices >= 1)
        //     {
        //         // Loop through each device, print out address
        //         for(int i=0;i<numberOfDevices; i++)
        //         {
        //             // Search the wire for address
        //             if(sensors.getAddress(tempDeviceAddress, i))
        //             {
        //             Serial.print("Found device ");
        //             Serial.print(i, DEC);
        //             Serial.print(" with address: ");
        //             printAddress(tempDeviceAddress);
        //             Serial.println();
        // delay(500);
        //             Serial.print("   Setting resolution: ");
        //             Serial.println(TEMPERATURE_PRECISION, DEC);

        //             // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
        //             sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);

        //             Serial.print("Confirming resolution: ");
        //             Serial.print(sensors.getResolution(tempDeviceAddress), DEC);
        //             Serial.println();
        //             delay(500);
        //             }
        //             else{
        //             Serial.print("Found ghost device at ");
        //             Serial.print(i, DEC);
        //             Serial.print(" but could not detect address. Check power and cabling");
        //             Serial.println();
        //             }
        //         }
        //     }
        //     delay(200);
        // }


        // void printDualProbes()                            //  Print soil and water temps
        // {
        // coreTemp = sensors.getTempC(wildHiveCoreProbe);
        // Serial.print("Wild Hive Core Temp: ");
        // Serial.println(coreTemp);
        // //   soilTemp = sensors.getTempC(soilProbe);
        // //   Serial.print("Soil temp is: ");
        // //   Serial.println(soilTemp);

        // Serial.println();
        // }
