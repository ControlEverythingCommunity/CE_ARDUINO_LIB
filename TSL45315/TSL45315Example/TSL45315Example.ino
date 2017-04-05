/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL45315
        This code is designed to work with the TSL45315_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <TSL45315.h>

TSL45315 tsl;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tsl.getAddr_TSL45315(TSL45315_DEFAULT_ADDRESS);           // 0x29

    // The Digital Ambient Light Sensor
    // Timer Control sets the Integration Time
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Operating and PowerSave Mode
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    tsl.setTCntrl(TCNTRL_400MS);                    // Tint = 400 ms
    // tsl.setTCntrl(TCNTRL_200MS);                 // Tint = 200 ms
    // tsl.setTCntrl(TCNTRL_100MS);                 // Tint = 100 ms

    tsl.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // TSL45315 Digital Ambient Light Sensor Address
    address = tsl.tsl_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting ALS Data Readings from TSL45315");
        Serial.println(" ");
        // Set up for Digital Ambient Light Sensor
        tsl.setUpSensor();
        delay(500);

        // Display the results for ALS Values
        tsl.Measure_ALS();

        // Output Data to Screen
        Serial.print("Ambient Light Illuminance: ");
        Serial.print(tsl.tsl_alsData.L);
        Serial.println(" lx");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("TSL45315 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
