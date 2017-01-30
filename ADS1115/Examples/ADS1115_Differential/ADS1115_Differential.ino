/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADS1115
        This code is designed to work with the ADS1115_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADS1115_I2CADC#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <ADS1115.h>

ADS1115 ads;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    ads.getAddr_ADS1115(ADS1115_DEFAULT_ADDRESS);   // 0x48, 1001 000 (ADDR = GND)
    // ads.getAddr_ADS1115(ADS1115_VDD_ADDRESS);    // 0x49, 1001 001 (ADDR = VDD)
    // ads.getAddr_ADS1115(ADS1115_SDA_ADDRESS);    // 0x4A, 1001 010 (ADDR = SDA)
    // ads.getAddr_ADS1115(ADS1115_SCL_ADDRESS);    // 0x4B, 1001 011 (ADDR = SCL)

    // The ADC gain (PGA), Device operating mode, Data rate
    // can be changed via the following functions

    ads.setGain(GAIN_TWO);          // 2x gain   +/- 2.048V  1 bit = 0.0625mV (default)
    //ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 0.1875mV
    // ads.setGain(GAIN_ONE);       // 1x gain   +/- 4.096V  1 bit = 0.125mV
    // ads.setGain(GAIN_FOUR);      // 4x gain   +/- 1.024V  1 bit = 0.03125mV
    // ads.setGain(GAIN_EIGHT);     // 8x gain   +/- 0.512V  1 bit = 0.015625mV
    // ads.setGain(GAIN_SIXTEEN);   // 16x gain  +/- 0.256V  1 bit = 0.0078125mV

    ads.setMode(MODE_CONTIN);       // Continuous conversion mode
    // ads.setMode(MODE_SINGLE);    // Power-down single-shot mode (default)

    ads.setRate(RATE_128);          // 128SPS (default)
    // ads.setRate(RATE_8);         // 8SPS
    // ads.setRate(RATE_16);        // 16SPS
    // ads.setRate(RATE_32);        // 32SPS
    // ads.setRate(RATE_64);        // 64SPS
    // ads.setRate(RATE_250);       // 250SPS
    // ads.setRate(RATE_475);       // 475SPS
    // ads.setRate(RATE_860);       // 860SPS

    ads.setOSMode(OSMODE_SINGLE);   // Set to start a single-conversion

    ads.begin();
}

void loop(void)
{
    byte error;
    int8_t address;

    address = ads.ads_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        int16_t result01, result03, result13, result23;

        Serial.println("Getting Differential Readings from AIN01, AIN03, AIN13 and AIN23");
        Serial.println(" ");
        result01 = ads.Measure_Differential(01);
        Serial.print("Digital Value of Analog Input between Channel 0 and 1: ");
        Serial.println(result01);
        result03 = ads.Measure_Differential(03);
        Serial.print("Digital Value of Analog Input between Channel 0 and 3: ");
        Serial.println(result03);
        result13 = ads.Measure_Differential(13);
        Serial.print("Digital Value of Analog Input between Channel 1 and 3: ");
        Serial.println(result13);
        result23 = ads.Measure_Differential(23);
        Serial.print("Digital Value of Analog Input between Channel 2 and 3: ");
        Serial.println(result23);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("ADS1115 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
