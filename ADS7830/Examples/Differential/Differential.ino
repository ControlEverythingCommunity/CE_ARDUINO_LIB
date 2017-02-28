/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADS7830
        This code is designed to work with the ADS7830_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADS7830_I2CADC#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <ADS7830.h>

ADS7830 ads;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    ads.getAddr_ADS7830(ADS7830_DEFAULT_ADDRESS);   // 0x48, 1001 000 (ADDR = GND)
    // ads.getAddr_ADS7830(ADS7830_VDD_ADDRESS);    // 0x49, 1001 001 (ADDR = VDD)
    // ads.getAddr_ADS7830(ADS7830_SDA_ADDRESS);    // 0x4A, 1001 010 (ADDR = SDA)
    // ads.getAddr_ADS7830(ADS7830_SCL_ADDRESS);    // 0x4B, 1001 011 (ADDR = SCL)

    // The Device operating and Power-Down mode
    // can be changed via the following functions

    ads.setSDMode(SDMODE_DIFF);         // Differential Inputs
    // ads.setSDMode(SDMODE_SINGLE);    // Single-Ended Inputs


    ads.setPDMode(PDIROFF_ADON);        // Internal Reference OFF and A/D Converter ON
    // ads.setPDMode(PDADCONV);         // Power Down Between A/D Converter Conversions
    // ads.setPDMode(PDIRON_ADOFF);     // Internal Reference ON and A/D Converter OFF
    // ads.setPDMode(PDIRON_ADON);      // Internal Reference ON and A/D Converter ON

    ads.begin();
}

void loop(void)
{
    byte error;
    int8_t address;

    // ADS7830 Address
    address = ads.ads_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {

        int8_t result01, result10, result23, result32, result45, result54, result67, result76;

        Serial.println("Getting Differential Readings");
        result01 = ads.Measure_Differential(01);
        Serial.print("Digital Value of Analog Input between Channel 0 and 1: ");
        Serial.println(result01);
        result10 = ads.Measure_Differential(10);
        Serial.print("Digital Value of Analog Input between Channel 1 and 0: ");
        Serial.println(result10);
        result23 = ads.Measure_Differential(23);
        Serial.print("Digital Value of Analog Input between Channel 2 and 3: ");
        Serial.println(result23);
        result32 = ads.Measure_Differential(32);
        Serial.print("Digital Value of Analog Input between Channel 3 and 2: ");
        Serial.println(result32);
        result45 = ads.Measure_Differential(45);
        Serial.print("Digital Value of Analog Input between Channel 4 and 5: ");
        Serial.println(result45);
        result54 = ads.Measure_Differential(54);
        Serial.print("Digital Value of Analog Input between Channel 5 and 4: ");
        Serial.println(result54);
        result67 = ads.Measure_Differential(67);
        Serial.print("Digital Value of Analog Input between Channel 6 and 7: ");
        Serial.println(result67);
        result76 = ads.Measure_Differential(76);
        Serial.print("Digital Value of Analog Input between Channel 7 and 6: ");
        Serial.println(result76);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("ADS7830 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
