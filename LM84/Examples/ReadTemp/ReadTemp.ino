/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LM84
        This code is designed to work with the LM84_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <LM84.h>

LM84 lm;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    lm.getAddr_LM84(LM84_DEFAULT_ADDRESS);                  // 0x18, 001 1000
    // lm.getAddr_LM84(LM84_ADDRESS_ADD0ADD1_0TRI);         // 0x19, 001 1001
    // lm.getAddr_LM84(LM84_ADDRESS_ADD0ADD1_01);           // 0x1A, 001 1010
    // lm.getAddr_LM84(LM84_ADDRESS_ADD0ADD1_TRI0);         // 0x29, 010 1001
    // lm.getAddr_LM84(LM84_ADDRESS_ADD0ADD1_TRITRI);       // 0x2A, 010 1010
    // lm.getAddr_LM84(LM84_ADDRESS_ADD0ADD1_TRI1);         // 0x2B, 010 1011
    // lm.getAddr_LM84(LM84_ADDRESS_ADD0ADD1_10);           // 0x4C, 100 1100
    // lm.getAddr_LM84(LM84_ADDRESS_ADD0ADD1_1TRI);         // 0x4D, 100 1101
    // lm.getAddr_LM84(LM84_ADDRESS_ADD0ADD1_11);           // 0x4E, 100 1110


    // The T_CRIT_A Interrupts Mask
    // can be changed via the following function

    lm.setTCritA(T_CRIT_A_MASKED);                          // T_CRIT_A Interrupts are Masked
    // lm.setTCritA(T_CRIT_A_NOT_MASKED);                   // T_CRIT_A Interrupts are not Masked

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
        Serial.println("Getting Temperature Readings from LM84");
        Serial.println(" ");

        // Set up for Diode Input Digital Temperature Sensor
        lm.setUpConfig();
        delay(500);

        // Display the results for Local and Remote Temperature
        lm.Measure_Temp();

        // Output data to screen
        Serial.print("Local Temperature in Celsius: ");
        Serial.print(lm.lm_tempData.LT);
        Serial.println(" °C");
        Serial.print("Local Temperature in Fahrenheit: ");
        Serial.print(lm.lm_tempData.LT * 1.8 + 32);
        Serial.println(" °F");
        Serial.print("Remote Temperature in Celsius: ");
        Serial.print(lm.lm_tempData.RT);
        Serial.println(" °C");
        Serial.print("Remote Temperature in Fahrenheit: ");
        Serial.print(lm.lm_tempData.RT * 1.8 + 32);
        Serial.println(" °F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("LM84 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
