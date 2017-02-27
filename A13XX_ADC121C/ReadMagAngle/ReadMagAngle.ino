/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        A13XX_ADC121C
        This code is designed to work with the A13XX_ADC121C_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/hall-effect-sensor-0-5-mv-g-with-adc121c-12-bit-resolution?variant=25687579211#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <A13XX_ADC121C.h>

A13XX_ADC121C adc;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    adc.getAddr_A13XX_ADC121C(A13XX_ADC121C_DEFAULT_ADDRESS);         // 0x50, 1010000
    // adc.getAddr_A13XX_ADC121C(A13XX_ADC121C_ADDRESS_FLGND);        // 0x51, 1010001
    // adc.getAddr_A13XX_ADC121C(A13XX_ADC121C_ADDRESS_FLVA);         // 0x52, 1010010
    // adc.getAddr_A13XX_ADC121C(A13XX_ADC121C_ADDRESS_GNDFL);        // 0x54, 1010100
    // adc.getAddr_A13XX_ADC121C(A13XX_ADC121C_ADDRESS_GNDGND);       // 0x55, 1010101
    // adc.getAddr_A13XX_ADC121C(A13XX_ADC121C_ADDRESS_GNDVA);        // 0x56, 1010110
    // adc.getAddr_A13XX_ADC121C(A13XX_ADC121C_ADDRESS_VAFL);         // 0x58, 1011000
    // adc.getAddr_A13XX_ADC121C(A13XX_ADC121C_ADDRESS_VAGND);        // 0x59, 1011001
    // adc.getAddr_A13XX_ADC121C(A13XX_ADC121C_ADDRESS_VAVA);         // 0x5A, 1011010

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
        adc.readRegister(address);
        float magAngle;

        Serial.println("Getting Hall Effect Rotational Angle Readings from A1332");
        Serial.println(" ");
        // Read and print out the Magnetic Angle, convert it to degrees
        magAngle = adc.Measure_HallSensor();

        // Output data to screen
        Serial.print("Magnetic Angle Value: ");
        Serial.print(magAngle);
        Serial.println(" degrees");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("A13XX_ADC121C Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
