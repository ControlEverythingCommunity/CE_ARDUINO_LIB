/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        VEML6070
        This code is designed to work with the VEML6070_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <VEML6070.h>

VEML6070 veml;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    veml.getAddr_VEML6070(VEML6070_ADDRESS_LOW);                // 0x38 (0111000x)
    // veml.getAddr_VEML6070(VEML6070_ADDRESS_HIGH);            // 0x39 (0111001x)

    // The Integration Time Setting can be changed via the following function
    veml.begin(VEML6070_HALF_T);    // 1/2T
    // veml.begin(VEML6070_1_T);    // 1T
    // veml.begin(VEML6070_2_T);    // 2T
    // veml.begin(VEML6070_4_T);    // 4T
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = veml.veml_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        uint16_t UVlight;

        Serial.println("Getting UV Light Readings from VEML6070");
        Serial.println(" ");
        // Read and print out the UV data
        UVlight = veml.ReadUV();

        // Output data to screen
        Serial.print("UV Light Level: ");
        Serial.println(UVlight);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("VEML6070 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
