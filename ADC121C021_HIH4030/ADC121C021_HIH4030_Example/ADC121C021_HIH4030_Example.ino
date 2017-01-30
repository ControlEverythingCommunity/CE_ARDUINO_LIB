/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADC121C021_HIH-4030
        This code is designed to work with the ADC121C021_I2CS_HIH-4030 I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <ADC121C021_HIH4030.h>

ADC121C021_HIH4030 adc;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    adc.getAddr_ADC121C021_HIH4030(ADC121C021_HIH4030_DEFAULT_ADDRESS);         // 0x50, 1010000
    // adc.getAddr_ADC121C021_HIH4030(ADC121C021_HIH4030_ADDRESS_FLGND);        // 0x51, 1010001
    // adc.getAddr_ADC121C021_HIH4030(ADC121C021_HIH4030_ADDRESS_FLVA);         // 0x52, 1010010
    // adc.getAddr_ADC121C021_HIH4030(ADC121C021_HIH4030_ADDRESS_GNDFL);        // 0x54, 1010100
    // adc.getAddr_ADC121C021_HIH4030(ADC121C021_HIH4030_ADDRESS_GNDGND);       // 0x55, 1010101
    // adc.getAddr_ADC121C021_HIH4030(ADC121C021_HIH4030_ADDRESS_GNDVA);        // 0x56, 1010110
    // adc.getAddr_ADC121C021_HIH4030(ADC121C021_HIH4030_ADDRESS_VAFL);         // 0x58, 1011000
    // adc.getAddr_ADC121C021_HIH4030(ADC121C021_HIH4030_ADDRESS_VAGND);        // 0x59, 1011001
    // adc.getAddr_ADC121C021_HIH4030(ADC121C021_HIH4030_ADDRESS_VAVA);         // 0x5A, 1011010

    adc.begin();
}

void loop(void) 
{
    byte error;
    int8_t address;

    address = adc.adc_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        float sensorRH, trueRH;

        Serial.println("Getting Relative Humidity Readings from ADC121C021_HIH-4030");
        Serial.println(" ");
        sensorRH = adc.getSensorRH();
        trueRH = adc.getTrueRH();
        Serial.print("Relative Humidity: ");
        Serial.print(sensorRH);
        Serial.println(" %RH");
        Serial.print("True Relative Humidity: ");
        Serial.print(trueRH);
        Serial.println(" %RH");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("ADC121C021_HIH-4030 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
