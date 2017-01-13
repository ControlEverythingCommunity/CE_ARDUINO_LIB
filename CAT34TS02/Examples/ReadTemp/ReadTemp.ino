/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        CAT34TS02
        This code is designed to work with the CAT34TS02_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <CAT34TS02.h>

CAT34TS02 cat;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    cat.getAddr_CAT34TS02(CAT34TS02_DEFAULT_ADDRESS);           // 0x18, 0011 000
    // cat.getAddr_CAT34TS02(CAT34TS02_ADDRESS_A2A1A0_001);     // 0x19, 0011 001
    // cat.getAddr_CAT34TS02(CAT34TS02_ADDRESS_A2A1A0_010);     // 0x1A, 0011 010
    // cat.getAddr_CAT34TS02(CAT34TS02_ADDRESS_A2A1A0_011);     // 0x1B, 0011 011
    // cat.getAddr_CAT34TS02(CAT34TS02_ADDRESS_A2A1A0_100);     // 0x1C, 0011 100
    // cat.getAddr_CAT34TS02(CAT34TS02_ADDRESS_A2A1A0_101);     // 0x1D, 0011 101
    // cat.getAddr_CAT34TS02(CAT34TS02_ADDRESS_A2A1A0_110);     // 0x1E, 0011 110
    // cat.getAddr_CAT34TS02(CAT34TS02_ADDRESS_A2A1A0_111);     // 0x1F, 0011 111


    // The Conversion Resolution and Temperature Range,
    // The TUPPER and TLOWER Limit Hysteresis and Shutdown Mode
    // can be changed via the following functions

    cat.setTRes(RES_12BIT);             // Conversion Resolution: 0.0625°C
    // cat.setTRes(RES_9BIT);           // Conversion Resolution: 0.50°C
    // cat.setTRes(RES_10BIT);          // Conversion Resolution: 0.25°C
    // cat.setTRes(RES_11BIT);          // Conversion Resolution: 0.125°C

    cat.setTRange(TEMPRANGE_POSTIVE);   // Positive Temperature Only
    cat.setTRange(TEMPRANGE_BOTH);      // Positive and Negative Temperature

    cat.setTHyst(THYST_DISABLE);        // Disable Hysteresis
    // cat.setTHyst(THYST_1_5C);        // Set hysteresis at 1.5°C
    // cat.setTHyst(THYST_3C);          // Set hysteresis at 3.0°C
    // cat.setTHyst(THYST_6C);          // Set hysteresis at 6.0°C

    cat.setTShdn(SHDN_ENABLE);          // Thermal Sensor is Enabled
    // cat.setTShdn(SHDN_SHUT);         // Thermal Sensor is Shut Down

    cat.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = cat.cat_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        int16_t temp;
        Serial.println("Getting Temperature Readings from CAT34TS02");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        temp = cat.Measure_Temp();
        float cTemp = temp * 0.0625;    // resolution is set to 0.0625
        float fTemp = cTemp * 1.8 + 32;

        // Output data to screen
        Serial.print("Temperature in Celsius: ");
        Serial.print(cTemp);
        Serial.println(" C");
        Serial.print("Temperature in Fahrenheit: ");
        Serial.print(fTemp);
        Serial.println(" F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("CAT34TS02 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
