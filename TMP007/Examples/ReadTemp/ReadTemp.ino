/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMP007
        This code is designed to work with the TMP007_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=TMP007_I2CS#tabs-0-product_tabset-2
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <TMP007.h>

TMP007 tmp;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    tmp.getAddr_TMP007(TMP007_DEFAULT_ADDRESS);         // 0x40, 1000111
    // tmp.getAddr_TMP007(TMP007_0_1_ADDRESS);          // 0x41, 1000000
    // tmp.getAddr_TMP007(TMP007_0_SDA_ADDRESS);        // 0x42, 1000001
    // tmp.getAddr_TMP007(TMP007_0_SCL_ADDRESS);        // 0x43, 1000010
    // tmp.getAddr_TMP007(TMP007_1_0_ADDRESS);          // 0x44, 1000011
    // tmp.getAddr_TMP007(TMP007_1_1_ADDRESS);          // 0x45, 1000100
    // tmp.getAddr_TMP007(TMP007_1_SDA_ADDRESS);        // 0x46, 1000101
    // tmp.getAddr_TMP007(TMP007_1_SCL_ADDRESS);        // 0x47, 1000110

    /*
    // There are parameters that can be changed using the following functions
    // Software Reset, Conversion Mode, Conversion Rate/Averaging Mode, Alert Pin Enable
    // Cumulative Alert Flag and Transient Correction Enable, INT/COMP Bit Controls
    // Status Cumulative Alert Flag and Conversion Ready Flag Enable
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    tmp.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = tmp.tmp_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        float TDieC, TDieF, TObjC, TObjF;

        Serial.println("Getting Temperature Readings from TMP007");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        TDieC = tmp.Measure_DieTemp();
        TDieF = TDieC * 1.8 + 32;

        TObjC = tmp.Measure_ObjTemp();
        TObjF = TObjC * 1.8 + 32;

        // Output data to screen
        Serial.print("Die Temperature Reading in Celsius: ");
        Serial.print(TDieC);
        Serial.println(" °C");
        Serial.print("Die Temperature Reading in Fahrenheit: ");
        Serial.print(TDieF);
        Serial.println(" °F");
        Serial.print("Object Temperature Reading in Celsius: ");
        Serial.print(TObjC);
        Serial.println(" °C");
        Serial.print("Object Temperature Reading in Fahrenheit: ");
        Serial.print(TObjF);
        Serial.println(" °F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("TMP007 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
