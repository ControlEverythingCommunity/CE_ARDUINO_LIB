/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AD7999
        This code is designed to work with the AD7999_I2CADC I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/ad7999-4-channel-8-10-12-bit-analog-to-digital-converter
*/
/**************************************************************************/

#include <Wire.h>
#include <AD7999.h>

AD7999 ad;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    ad.getAddr_AD7999(AD7999_DEFAULT_ADDRESS);         // 0x50, 1010000

    // The Selection of the Analog Input Channel(s) to be Converted
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Supply Voltage as the Reference or Choose to use an External Reference
    // Determines Whether the Filtering on SDA and SCL is Enabled or Bypassed
    // Bit Trial and Sample Delay
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    ad.setChannel(CHANNEL_VIN0);               // Convert on VIN0
    // ad.setChannel(NO_CHANNEL);              // No channel Selected
    // ad.setChannel(CHANNEL_VIN1);            // Convert on VIN1
    // ad.setChannel(CHANNEL_VIN0_1);          // Sequence between VIN0 and VIN1
    // ad.setChannel(CHANNEL_VIN2);            // Convert on VIN2
    // ad.setChannel(CHANNEL_VIN0_2);          // Sequence between VIN0 and VIN2
    // ad.setChannel(CHANNEL_VIN1_2);          // Sequence between VIN1 and VIN2
    // ad.setChannel(CHANNEL_VIN0_1_2);        // Sequence among VIN0, VIN1, and VIN2
    // ad.setChannel(CHANNEL_VIN3);            // Convert on VIN3
    // ad.setChannel(CHANNEL_VIN0_3);          // Sequence between VIN0 and VIN3
    // ad.setChannel(CHANNEL_VIN1_3);          // Sequence between VIN1 and VIN3
    // ad.setChannel(CHANNEL_VIN0_1_3);        // Sequence among VIN0, VIN1, and VIN3
    // ad.setChannel(CHANNEL_VIN2_3);          // Sequence between VIN2 and VIN3
    // ad.setChannel(CHANNEL_VIN0_2_3);        // Sequence among VIN0, VIN2, and VIN3
    // ad.setChannel(CHANNEL_VIN1_2_3);        // Sequence among VIN1, VIN2, and VIN3
    // ad.setChannel(CHANNEL_VIN0_1_2_3);      // Sequence among VIN0, VIN1, VIN2, and VIN3

    ad.begin();
}

void loop(void) 
{
    byte error;
    int8_t address;

    address = ad.ad_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        uint8_t raw_adc;

        Serial.println("Getting Conversion Readings from AD7999");
        Serial.println(" ");
        raw_adc = ad.Measure_Voltage();
        Serial.print("Digital Value of Analog Input at Channel 0: ");
        Serial.println(raw_adc);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("AD7999 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
