/****************************************************************************
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MCP3425
        This code is designed to work with the MCP3425_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Analog-Digital-Converters?sku=MCP3425_I2CADC#tabs-0-product_tabset-2
*/
/****************************************************************************/

#include <MCP3425.h>
#include <Wire.h>

MCP3425 MCP(0); // Declaration of MCP3425: A2, A1, A0 bits (000, 0x68)

void setup()
{
    // Start serial communication and set baud rate = 9600
    Serial.begin(9600);
    Serial.println("MCP3425 Analog to Digital Converter");
    Serial.println("Getting ADC Readings from Channel 1");
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

        // MCP3425 is configured to channel 1 with 14 bits resolution, one shot mode and gain defined to 1
        // This arrangement of the mentioned paarmeters can be changed as per convenience
        MCP.SetConfiguration(1,14,0,1);

        // Note that the library waits for a complete conversion
        Raw_ADC = MCP.readADC();
        // print result
        Serial.print("Digital value of Analog Input at Channel 1: ");
        Serial.println(Raw_ADC);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("MCP3425 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
