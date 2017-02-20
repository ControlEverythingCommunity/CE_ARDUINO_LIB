/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMD26721
        This code is designed to work with the TMD26721_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Proximity?sku=TMD26721_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <TMD26721.h>

TMD26721 tmd;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tmd.getAddr_TMD26721(TMD26721_DEFAULT_ADDRESS);           // 0x39

    // The Infrared Digital Proximity Detector
    //Internal Integration Time of the Proximity ADC’s in 2.73ms Increments,
    // Wait Time, Proximity LED Drive Strength, Proximity Diode Selector
    // Proximity Gain
    // can be changed via the following functions

    /*
    // Also, some more parameters can be changed using the following functions
    // Infrared Digital Proximity Detector:
    // Sleep After, Proximity Interrupt, Wait , Proximity and Power Enable,
    // Wait Long.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    tmd.setPTime(PTIME_2_73MS);                 // Cycles: 1, Time: 2.73 ms Max Count: 1023

    tmd.setWTime(WTIME_1);                      // Wait Time: 1, Time (WLONG = 0): 2.72 ms Time (WLONG = 1):  0.032 sec
    // tmd.setWTime(WTIME_74);                  // Wait Time: 74, Time (WLONG = 0): 200 ms Time (WLONG = 1):  2.4 sec
    // tmd.setWTime(WTIME_256);                 // Wait Time: 256, Time (WLONG = 0): 700 ms Time (WLONG = 1):  8.3 sec

    tmd.setProximityDrive(PDRIVE_100);          // LED Strength – PDL=0: 100mA, LED Strength – PDL=1: 11.1mA
    // tmd.setProximityDrive(PDRIVE_50);        // LED Strength – PDL=0: 50mA, LED Strength – PDL=1: 5.6mA
    // tmd.setProximityDrive(PDRIVE_25);        // LED Strength – PDL=0: 25mA, LED Strength – PDL=1: 2.8mA
    // tmd.setProximityDrive(PDRIVE_12_5);      // LED Strength – PDL=0: 12.5mA, LED Strength – PDL=1: 1.4mA

    tmd.setProximityDiode(PDIODE_CH1);          // Proximity Uses the CH1 Diode
    // tmd.setProximityDiode(PDIODE_CH0);       // Proximity Uses the CH0 Diode
    // tmd.setProximityDiode(PDIODE_NONE);      // Proximity Uses the Neither Diode

    tmd.setProximityGain(PGAIN_1X);             // Proximity Gain: 1X
    // tmd.setProximityGain(PGAIN_2X);          // Proximity Gain: 2X
    // tmd.setProximityGain(PGAIN_4X);          // Proximity Gain: 4X
    // tmd.setProximityGain(PGAIN_8X);          // Proximity Gain: 8X

    tmd.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // TMD26721 Infrared Digital Proximity Detector Address
    address = tmd.tmd_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Proximity Data Readings from TMD26721");
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
        Serial.println("TMD26721 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
