/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        VCNL4010
        This code is designed to work with the VCNL4010_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=VCNL4010_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <VCNL4010.h>

VCNL4010 vcnl;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    vcnl.getAddr_VCNL4010(VCNL4010_DEFAULT_ADDRESS);           // 0x13

    // The Proximity and Ambient Light Sensor
    // The Proximity IR Test Signal Frequency
    // can be changed via the following function

    vcnl.setFrequency(FREQUENCY_390_625KHZ);                    // 390.625 kHz (DEFAULT)
    // vcnl.setFrequency(FREQUENCY_781_25KHZ);                  // 781.25 kHz
    // vcnl.setFrequency(FREQUENCY_1_5625MHZ);                  // 1.5625 MHz
    // vcnl.setFrequency(FREQUENCY_3_125MHZ);                   // 3.125 MHz

    vcnl.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // VCNL4010 Proximity and Ambient Light Sensor Address
    address = vcnl.vcnl_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        uint16_t als, proximity;
        Serial.println("Getting ALS and Proximity Data Readings from VCNL4010");
        Serial.println(" ");
        // Set Up and Display the results for Proximity and Ambient Light Sensor
        als = vcnl.Measure_ALS();
        proximity = vcnl.Measure_Proximity();
        delay(500);

        // Output Data to Screen
        Serial.print("Ambient Light Illuminance: ");
        Serial.println(als);
        Serial.print("Proximity of the Sensor: ");
        Serial.println(proximity);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("VCNL4010 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
