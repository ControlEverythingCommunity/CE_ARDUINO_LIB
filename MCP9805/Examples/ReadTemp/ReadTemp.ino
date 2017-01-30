/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MCP9805
        This code is designed to work with the MCP9805_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=MCP9805_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <MCP9805.h>

MCP9805 mcp;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    mcp.getAddr_MCP9805(MCP9805_DEFAULT_ADDRESS);           // 0x18, 0011 000
    // mcp.getAddr_MCP9805(MCP9805_ADDRESS_A2A1A0_001);     // 0x19, 0011 001
    // mcp.getAddr_MCP9805(MCP9805_ADDRESS_A2A1A0_010);     // 0x1A, 0011 010
    // mcp.getAddr_MCP9805(MCP9805_ADDRESS_A2A1A0_011);     // 0x1B, 0011 011
    // mcp.getAddr_MCP9805(MCP9805_ADDRESS_A2A1A0_100);     // 0x1C, 0011 100
    // mcp.getAddr_MCP9805(MCP9805_ADDRESS_A2A1A0_101);     // 0x1D, 0011 101
    // mcp.getAddr_MCP9805(MCP9805_ADDRESS_A2A1A0_110);     // 0x1E, 0011 110
    // mcp.getAddr_MCP9805(MCP9805_ADDRESS_A2A1A0_111);     // 0x1F, 0011 111

    // The TUPPER and TLOWER Limit Hysteresis,  Shutdown Mode and Resolution
    // can be changed via the following functions

    mcp.setTHyst(THYST_0C);         // 0°C (power-up default)
    // mcp.setTHyst(THYST_1_5C);    // +1.5°C
    // mcp.setTHyst(THYST_3C);      // +3.0°C
    // mcp.setTHyst(THYST_6C);      // +6.0°C

    mcp.setTShdn(SHDN_CONT);        // Continuous conversion (power-up default)
    // mcp.setTShdn(SHDN_SHUT);     // Shutdown (Low-Power mode)

    mcp.setRes(RES_0_0625);         // +0.0625°C (power-up default, tCONV = 250 ms typical)
    // mcp.setRes(RES_0_25);        // +0.5°C (tCONV = 30 ms typical)
    // mcp.setRes(RES_0_25);        // +0.25°C (tCONV = 65 ms typical)
    // mcp.setRes(RES_0_125);       // +0.125°C (tCONV = 130 ms typical)

    mcp.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = mcp.mcp_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        int16_t temp;
        Serial.println("Getting Temperature Readings from MCP9805");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        temp = mcp.Measure_Temp();
        float cTemp = temp * 0.0625;    // resolution is set to 0.0625
        float fTemp = cTemp * 1.8 + 32;

        // Output data to screen
        Serial.print("Temperature in Celsius : ");
        Serial.print(cTemp);
        Serial.println(" C");
        Serial.print("Temperature in Fahrenheit : ");
        Serial.print(fTemp);
        Serial.println(" F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("MCP9805 Disconnected !");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
