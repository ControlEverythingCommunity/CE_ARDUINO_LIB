/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LT-1PA01
        This code is designed to work with the LT-1PA01_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/
*/
/**************************************************************************/

#include <Wire.h>
#include <LT_1PA01.h>

LT_1PA01 lt;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    lt.getAddr_LT_1PA01(LT_1PA01_DEFAULT_ADDRESS);           // 0x44

    // The Proximity/Ambient Light Sensor
    // Proximity Enable/Disable, Proximity Sleep Time Select, IRDR (VCSEL) Current
    // Interrupt Algorithm, Proximity Offset Compensation
    // Ambient Light Sensing Enable/Disable and Ambient Light Sensor Range Select
    // can be changed via functions

    lt.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // LT_1PA01 Proximity/Ambient Light Sensor Address
    address = lt.lt_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Proximity and ALS Data Readings from LT_1PA01");
        Serial.println(" ");
        // Set up for Proximity/Ambient Light Sensor
        lt.setUpALSProximity();
        delay(500);

        // Display the results for ALS and Proximity Values
        lt.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Proximity of the Sensor: ");
        Serial.println(lt.lt_alsproxData.P);
        Serial.print("Ambient Light Illuminance: ");
        Serial.print(lt.lt_alsproxData.L);
        Serial.println(" lx");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("LT-1PA01 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
