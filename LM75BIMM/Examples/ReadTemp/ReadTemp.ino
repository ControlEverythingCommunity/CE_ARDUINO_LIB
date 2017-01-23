/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LM75BIMM
        This code is designed to work with the LM75BIMM_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=LM75BIMM_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <LM75BIMM.h>

LM75BIMM lm;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    lm.getAddr_LM75BIMM(LM75BIMM_0_0_1_ADDRESS);            // 0x49, 1001001
    // lm.getAddr_LM75BIMM(LM75BIMM_DEFAULT_ADDRESS);       // 0x48, 1001000
    // lm.getAddr_LM75BIMM(LM75BIMM_0_1_0_ADDRESS);         // 0x4A, 1001010
    // lm.getAddr_LM75BIMM(LM75BIMM_0_1_1_ADDRESS);         // 0x4B, 1001011
    // lm.getAddr_LM75BIMM(LM75BIMM_1_0_0_ADDRESS);         // 0x4C, 1001100
    // lm.getAddr_LM75BIMM(LM75BIMM_1_0_1_ADDRESS);         // 0x4D, 1001101
    // lm.getAddr_LM75BIMM(LM75BIMM_1_1_0_ADDRESS);         // 0x4E, 1001110
    // lm.getAddr_LM75BIMM(LM75BIMM_1_1_1_ADDRESS);         // 0x4F, 1001111

    // The Shutdown Mode can be changed via the following function

    lm.setShutdown(SHUTDOWN_DISABLE);      // Disable (Power-up default)
    // lm.setShutdown(SHUTDOWN_ENABLE);    // Enable

    lm.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = lm.lm_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        int16_t temp;

        Serial.println("Getting Temperature Readings from LM75BIMM");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        temp = lm.Measure_Temp();
        // 9-bit resolution
        temp = temp >> 7;
        float cTemp = temp * 0.5;
        float fTemp = cTemp * 1.8 + 32;

        // Output data to screen
        Serial.print("Temperature Reading in Celsius : ");
        Serial.print(cTemp);
        Serial.println(" C");
        Serial.print("Temperature Reading in Fahrenheit : ");
        Serial.print(fTemp);
        Serial.println(" F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("LM75BIMM Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
