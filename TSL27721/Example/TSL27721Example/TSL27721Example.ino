/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL27721
        This code is designed to work with the TSL27721_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <TSL27721.h>

TSL27721 tsl;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tsl.getAddr_TSL27721(TSL27721_DEFAULT_ADDRESS);           // 0x39

    // The Digital Ambient Light and Proximity Sensor
    //Internal Integration Time of the ALS Channel and Proximity ADC’s in 2.73ms Increments,
    // Wait Time, Proximity LED Drive Strength, Proximity Diode Selector
    // Proximity and ALS Gain
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Digital Ambient Light and Proximity Sensor:
    // Sleep After, Proximity and ALS Interrupt, Wait , Proximity, ALS and Power Enable,
    // ALS Gain Level, Wait Long and Proximity Drive Level.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    tsl.setATime(ATIME_2_73MS);                 // Cycles: 1, Time: 2.73 ms Max Count: 1024
    // tsl.setATime(ATIME_27_3MS);              // Cycles: 10, Time: 27.3 ms Max Count: 10240
    // tsl.setATime(ATIME_101MS);               // Cycles: 37, Time: 101 ms Max Count: 37888
    // tsl.setATime(ATIME_175MS);               // Cycles: 64, Time: 175 ms Max Count: 65535
    // tsl.setATime(ATIME_699MS);               // Cycles: 256, Time: 699 ms Max Count: 65535

    tsl.setPTime(PTIME_2_73MS);                 // Cycles: 1, Time: 2.73 ms Max Count: 1023

    tsl.setWTime(WTIME_1);                      // Wait Time: 1, Time (WLONG = 0): 2.73 ms Time (WLONG = 1):  0.033 sec
    // tsl.setWTime(WTIME_74);                  // Wait Time: 74, Time (WLONG = 0): 202 ms Time (WLONG = 1):  2.4 sec
    // tsl.setWTime(WTIME_256);                 // Wait Time: 256, Time (WLONG = 0): 699 ms Time (WLONG = 1):  8.4 sec

    tsl.setProximityDrive(PDRIVE_120);          // LED Strength – PDL=0: 120mA, LED Strength – PDL=1: 15mA
    // tsl.setProximityDrive(PDRIVE_60);        // LED Strength – PDL=0: 60mA, LED Strength – PDL=1: 7.5mA
    // tsl.setProximityDrive(PDRIVE_30);        // LED Strength – PDL=0: 30mA, LED Strength – PDL=1: 3.8mA
    // tsl.setProximityDrive(PDRIVE_15);        // LED Strength – PDL=0: 15mA, LED Strength – PDL=1: 1.9mA

    tsl.setProximityDiode(PDIODE_CH1);          // Proximity Uses the CH1 Diode
    // tsl.setProximityDiode(PDIODE_CH0);       // Proximity Uses the CH0 Diode
    // tsl.setProximityDiode(PDIODE_NONE);      // Proximity Uses the Neither Diode

    tsl.setProximityGain(PGAIN_1X);             // Proximity Gain: 1X
    // tsl.setProximityGain(PGAIN_2X);          // Proximity Gain: 2X
    // tsl.setProximityGain(PGAIN_4X);          // Proximity Gain: 4X
    // tsl.setProximityGain(PGAIN_8X);          // Proximity Gain: 8X

    tsl.setALSGain(AGAIN_1X);                   // ALS Gain: 1X
    // tsl.setALSGain(AGAIN_8X);                // ALS Gain: 8X
    // tsl.setALSGain(AGAIN_16X);               // ALS Gain: 16X
    // tsl.setALSGain(AGAIN_120X);              // ALS Gain: 120X

    tsl.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // TSL27721 Digital Ambient Light and Proximity Sensor Address
    address = tsl.tsl_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting ALS and Proximity Data Readings from TSL27721");
        Serial.println(" ");
        // Set up for Digital Ambient Light and Proximity Sensor
        tsl.setUpALSProximity();
        delay(500);

        // Display the results for ALS and Proximity Values
        tsl.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Ambient Light Illuminance: ");
        Serial.print(tsl.tsl_alsproxData.L);
        Serial.println(" lx");
        Serial.print("Proximity of the Sensor: ");
        Serial.println(tsl.tsl_alsproxData.P);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("TSL27721 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
