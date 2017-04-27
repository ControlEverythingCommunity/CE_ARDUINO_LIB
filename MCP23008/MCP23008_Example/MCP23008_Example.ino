/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MCP23008
        This code is designed to work with the MCP23008_I2CR8G5LE_10A_D I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/8-channel-relay-controller-i2c#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <MCP23008.h>

MCP23008 mcp;

void setup(void) 
{
    Serial.begin(9600);
    mcp.begin();

    // The address can be changed making the option of connecting multiple devices
    mcp.getAddr_MCP23008(MCP23008_DEFAULT_ADDRESS);             // 0x20
    // mcp.getAddr_MCP23008(MCP23008_ADDRESS_A2A1A0_001);       // 0x21
    // mcp.getAddr_MCP23008(MCP23008_ADDRESS_A2A1A0_010);       // 0x22
    // mcp.getAddr_MCP23008(MCP23008_ADDRESS_A2A1A0_011);       // 0x23
    // mcp.getAddr_MCP23008(MCP23008_ADDRESS_A2A1A0_100);       // 0x24
    // mcp.getAddr_MCP23008(MCP23008_ADDRESS_A2A1A0_101);       // 0x25
    // mcp.getAddr_MCP23008(MCP23008_ADDRESS_A2A1A0_110);       // 0x26
    // mcp.getAddr_MCP23008(MCP23008_ADDRESS_A2A1A0_111);       // 0x27


    // The I/O Pin Direction and Mode of the Corresponding Pin
    // can be changed via the following function

    mcp.pinMode(4, OUTPUT);         // Pin 0 and Output Mode
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
        uint8_t result;

        Serial.println("Getting Relay Response from MCP23008");
        Serial.println(" ");
        delay(500);
        mcp.digitalWrite(4, HIGH);
        Serial.println("Turning Relay '5' ON");
        Serial.println(" ");
        delay(500);
        mcp.digitalWrite(4, LOW);
        Serial.println("Turning Relay '5' OFF");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("MCP23008 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
