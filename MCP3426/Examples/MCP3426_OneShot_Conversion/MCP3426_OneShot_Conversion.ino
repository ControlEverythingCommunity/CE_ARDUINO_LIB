/****************************************************************************
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MCP3426
        This code is designed to work with the MCP3426_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Analog-Digital-Converters?sku=MCP3426_I2CADC#tabs-0-product_tabset-2
*/
/****************************************************************************/

#include <MCP3426.h>
#include <Wire.h>

MCP3426 MCP(0); // Declaration of MCP3426: A2, A1, A0 bits (000, 0x68)

void setup()
{
    // Start serial communication and set baud rate = 9600
    Serial.begin(9600);
    Serial.println("MCP3426 Analog to Digital Converter");
    Serial.println("Getting ADC Readings from Channel 2");
    Serial.println("        ************************************            ");
    Serial.println("        ");
    delay(500);
}

void loop()
{
    byte error;
    int8_t address;

    address = MCP.devAddr;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        long Raw_ADC;

        // MCP3426 is configured to channel 2 with 16 bits resolution, one shot mode and gain defined to 1
        // This arrangement of the mentioned paarmeters can be changed as per convenience
        MCP.SetConfiguration(2,16,0,1);

        // Note that the library waits for a complete conversion
        Raw_ADC = MCP.readADC();
        // raw_adc = raw_adc * LSB(1 mV)/PGA for PGA = 1;       // 12-bit Resolution
        // raw_adc = raw_adc * LSB(250 µV)/PGA for PGA = 1;     // 14-bit Resolution
        // raw_adc = raw_adc * LSB(62.5 µV)/PGA for PGA = 1;    // 16-bit Resolution

        // Output to the Screen
        Serial.print("Digital value of Analog Input at Channel 2: ");
        Serial.println(Raw_ADC);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("MCP3426 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
