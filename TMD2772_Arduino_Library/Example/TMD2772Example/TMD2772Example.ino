/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMD2772
        This code is designed to work with the TMD2772_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=TMD2772_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <TMD2772.h>

TMD2772 tmd;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tmd.getAddr_TMD2772(TMD2772_DEFAULT_ADDRESS);           // 0x39

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

    tmd.setATime(ATIME_2_73MS);                 // Cycles: 1, Time: 2.73 ms Max Count: 1024
    // tmd.setATime(ATIME_27_3MS);              // Cycles: 10, Time: 27.3 ms Max Count: 10240
    // tmd.setATime(ATIME_101MS);               // Cycles: 37, Time: 101 ms Max Count: 37888
    // tmd.setATime(ATIME_175MS);               // Cycles: 64, Time: 175 ms Max Count: 65535
    // tmd.setATime(ATIME_699MS);               // Cycles: 256, Time: 699 ms Max Count: 65535

    tmd.setPTime(PTIME_2_73MS);                 // Cycles: 1, Time: 2.73 ms Max Count: 1023

    tmd.setWTime(WTIME_1);                      // Wait Time: 1, Time (WLONG = 0): 2.73 ms Time (WLONG = 1):  0.033 sec
    // tmd.setWTime(WTIME_74);                  // Wait Time: 74, Time (WLONG = 0): 202 ms Time (WLONG = 1):  2.4 sec
    // tmd.setWTime(WTIME_256);                 // Wait Time: 256, Time (WLONG = 0): 699 ms Time (WLONG = 1):  8.4 sec

    tmd.setProximityDrive(PDRIVE_100);          // LED Strength – PDL=0: 100%, LED Strength – PDL=1: 11.1%
    // tmd.setProximityDrive(PDRIVE_50);        // LED Strength – PDL=0: 50%, LED Strength – PDL=1: 5.6%
    // tmd.setProximityDrive(PDRIVE_25);        // LED Strength – PDL=0: 25%, LED Strength – PDL=1: 2.8%
    // tmd.setProximityDrive(PDRIVE_12_5);      // LED Strength – PDL=0: 12.5%, LED Strength – PDL=1: 1.4%

    tmd.setProximityDiode(PDIODE_CH1);          // Proximity Uses the CH1 Diode
    // tmd.setProximityDiode(PDIODE_CH0);       // Proximity Uses the CH0 Diode
    // tmd.setProximityDiode(PDIODE_NONE);      // Proximity Uses the Neither Diode

    tmd.setProximityGain(PGAIN_1X);             // Proximity Gain: 1X
    // tmd.setProximityGain(PGAIN_2X);          // Proximity Gain: 2X
    // tmd.setProximityGain(PGAIN_4X);          // Proximity Gain: 4X
    // tmd.setProximityGain(PGAIN_8X);          // Proximity Gain: 8X

    tmd.setALSGain(AGAIN_1X);                   // ALS Gain: 1X
    // tmd.setALSGain(AGAIN_8X);                // ALS Gain: 8X
    // tmd.setALSGain(AGAIN_16X);               // ALS Gain: 16X
    // tmd.setALSGain(AGAIN_120X);              // ALS Gain: 120X

    tmd.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // TMD2772 Digital Ambient Light and Proximity Sensor Address
    address = tmd.tmd_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting ALS and Proximity Data Readings from TMD2772");
        Serial.println(" ");
        // Set up for Digital Ambient Light and Proximity Sensor
        tmd.setUpALSProximity();
        delay(500);

        // Display the results for ALS and Proximity Values
        tmd.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Ambient Light Illuminance: ");
        Serial.print(tmd.tmd_alsproxData.L);
        Serial.println(" lx");
        Serial.print("Proximity of the Sensor: ");
        Serial.println(tmd.tmd_alsproxData.P);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("TMD2772 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
