/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADS7828
        This code is designed to work with the ADS7828_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADS7828_I2CADC#tabs-0-product_tabset-2
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <ADS7828.h>

ADS7828 ads;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    ads.getAddr_ADS7828(ADS7828_DEFAULT_ADDRESS);   // 0x48, 1001 000 (ADDR = GND)
    // ads.getAddr_ADS7828(ADS7828_VDD_ADDRESS);    // 0x49, 1001 001 (ADDR = VDD)
    // ads.getAddr_ADS7828(ADS7828_SDA_ADDRESS);    // 0x4A, 1001 010 (ADDR = SDA)
    // ads.getAddr_ADS7828(ADS7828_SCL_ADDRESS);    // 0x4B, 1001 011 (ADDR = SCL)

    // The Device operating and Power-Down mode
    // can be changed via the following functions

    ads.setSDMode(SDMODE_SINGLE);       // Single-Ended Inputs
    //ads.setSDMode(SDMODE_DIFF);       // Differential Inputs

    ads.setPDMode(PDIROFF_ADON);        // Internal Reference OFF and A/D Converter ON
    // ads.setPDMode(PDADCONV);         // Power Down Between A/D Converter Conversions
    // ads.setPDMode(PDIRON_ADOFF);     // Internal Reference ON and A/D Converter OFF
    // ads.setPDMode(PDIRON_ADON);      // Internal Reference ON and A/D Converter ON

    ads.begin();
}

void loop(void) 
{
    int16_t adc0, adc1, adc2, adc3, adc4, adc5, adc6, adc7;

    Serial.println("Getting single-ended readings from AIN0..7");
    adc0 = ads.Measure_SingleEnded(0);
    Serial.print("Digital Value of Analog Input at Channel 0: ");
    Serial.println(adc0);
    adc1 = ads.Measure_SingleEnded(1);
    Serial.print("Digital Value of Analog Input at Channel 1: ");
    Serial.println(adc1);
    adc2 = ads.Measure_SingleEnded(2);
    Serial.print("Digital Value of Analog Input at Channel 2: ");
    Serial.println(adc2);
    adc3 = ads.Measure_SingleEnded(3);
    Serial.print("Digital Value of Analog Input at Channel 3: ");
    Serial.println(adc3);
    adc4 = ads.Measure_SingleEnded(4);
    Serial.print("Digital Value of Analog Input at Channel 4: ");
    Serial.println(adc4);
    adc5 = ads.Measure_SingleEnded(5);
    Serial.print("Digital Value of Analog Input at Channel 5: ");
    Serial.println(adc5);
    adc6 = ads.Measure_SingleEnded(6);
    Serial.print("Digital Value of Analog Input at Channel 6: ");
    Serial.println(adc6);
    adc7 = ads.Measure_SingleEnded(7);
    Serial.print("Digital Value of Analog Input at Channel 7: ");
    Serial.println(adc7);

    Serial.println("        ****************************************        ");
    Serial.println(" ");

    delay(1000);
}
