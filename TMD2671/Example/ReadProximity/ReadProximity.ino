/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMD2671
        This code is designed to work with the TMD2671_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Proximity?sku=TMD2671_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <TMD2671.h>

TMD2671 tmd;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tmd.getAddr_TMD2671(TMD2671_DEFAULT_ADDRESS);           // 0x39

    // The Infrared Digital Proximity Detector
    //Internal Integration Time of the ALS Channel and Proximity ADC’s in 2.72ms Increments,
    // Wait Time, Proximity LED Drive Strength, Proximity Diode Selector
    // can be changed via the following functions

    /*
    // Also, some more parameters can be changed using the following functions
    // Infrared Digital Proximity Detector:
    // Sleep After, Proximity Interrupt, Wait , Proximity and Power Enable,
    // Wait Long.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    tmd.setATime(ATIME_2_72MS);                 // Cycles: 1, Time: 2.72 ms
    // tmd.setATime(ATIME_696MS);               // Cycles: 256, Time: 696 ms

    tmd.setPTime(PTIME_2_72MS);                 // Cycles: 1, Time: 2.72 ms Max Count: 1023

    tmd.setWTime(WTIME_1);                      // Wait Time: 1, Time (WLONG = 0): 2.72 ms Time (WLONG = 1):  0.032 sec
    // tmd.setWTime(WTIME_74);                  // Wait Time: 74, Time (WLONG = 0): 200 ms Time (WLONG = 1):  2.4 sec
    // tmd.setWTime(WTIME_256);                 // Wait Time: 256, Time (WLONG = 0): 700 ms Time (WLONG = 1):  8.3 sec

    tmd.setProximityDrive(PDRIVE_100);          // LED Strength – PDL=0: 100%
    // tmd.setProximityDrive(PDRIVE_50);        // LED Strength – PDL=0: 50%
    // tmd.setProximityDrive(PDRIVE_25);        // LED Strength – PDL=0: 25m%
    // tmd.setProximityDrive(PDRIVE_12_5);      // LED Strength – PDL=0: 12.5%

    tmd.setProximityDiode(PDIODE_CH1);          // Proximity Uses the CH1 Diode
    // tmd.setProximityDiode(PDIODE_CH0);       // Proximity Uses the CH0 Diode
    // tmd.setProximityDiode(PDIODE_NONE);      // Proximity Uses the Neither Diode

    tmd.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // TMD2671 Infrared Digital Proximity Detector Address
    address = tmd.tmd_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Proximity Data Readings from TMD2671");
        Serial.println(" ");
        // Set up for Infrared Digital Proximity Detector
        tmd.setUpProximity();
        delay(500);

        // Display the results for Proximity Values
        tmd.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Proximity of the Sensor: ");
        Serial.println(tmd.tmd_proxData.P);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("TMD2671 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
