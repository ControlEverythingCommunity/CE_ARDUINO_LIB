/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMD2771
        This code is designed to work with the TMD2771_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=TMD2771_I2CS#tabs-0-product_tabset-2
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <TMD2771.h>

TMD2771 tmd;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tmd.getAddr_TMD2771(TMD2771_DEFAULT_ADDRESS);           // 0x39

    // The Digital Ambient Light and Proximity Sensor
    //Internal Integration Time of the ALS Channel and Proximity ADC’s in 2.72ms Increments,
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

    tmd.setATime(ATIME_2_72MS);                 // Cycles: 1, Time: 2.72 ms Max Count: 1024
    // tmd.setATime(ATIME_27_2MS);              // Cycles: 10, Time: 27.2 ms Max Count: 10240
    // tmd.setATime(ATIME_101MS);               // Cycles: 37, Time: 101 ms Max Count: 37888
    // tmd.setATime(ATIME_174MS);               // Cycles: 64, Time: 174 ms Max Count: 65535
    // tmd.setATime(ATIME_696MS);               // Cycles: 256, Time: 696 ms Max Count: 65535

    tmd.setPTime(PTIME_2_72MS);                 // Cycles: 1, Time: 2.72 ms Max Count: 1023

    tmd.setWTime(WTIME_1);                      // Wait Time: 1, Time (WLONG = 0): 2.72 ms Time (WLONG = 1):  0.032 sec
    // tmd.setWTime(WTIME_74);                  // Wait Time: 74, Time (WLONG = 0): 200 ms Time (WLONG = 1):  2.4 sec
    // tmd.setWTime(WTIME_256);                 // Wait Time: 256, Time (WLONG = 0): 700 ms Time (WLONG = 1):  8.3 sec

    tmd.setProximityDrive(PDRIVE_100);          // LED Strength – PDL=0: 100%
    // tmd.setProximityDrive(PDRIVE_50);        // LED Strength – PDL=0: 50%
    // tmd.setProximityDrive(PDRIVE_25);        // LED Strength – PDL=0: 25%
    // tmd.setProximityDrive(PDRIVE_12_5);      // LED Strength – PDL=0: 12.5%

    tmd.setProximityDiode(PDIODE_CH1);          // Proximity Uses the CH1 Diode
    // tmd.setProximityDiode(PDIODE_CH0);       // Proximity Uses the CH0 Diode
    // tmd.setProximityDiode(PDIODE_NONE);      // Proximity Uses the Neither Diode

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

    // TMD2771 Digital Ambient Light and Proximity Sensor Address
    address = tmd.tmd_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting ALS and Proximity Data Readings from TMD2771");
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
        Serial.println("TMD2771 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
