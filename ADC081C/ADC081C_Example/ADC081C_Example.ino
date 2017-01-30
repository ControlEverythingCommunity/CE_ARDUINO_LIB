/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADC081C
        This code is designed to work with the ADC081C_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADC081C_I2CADC#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <ADC081C.h>

ADC081C adc;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    adc.getAddr_ADC081C(ADC081C_DEFAULT_ADDRESS);         // 0x50, 1010000
    // adc.getAddr_ADC081C(ADC081C_ADDRESS_FLGND);        // 0x51, 1010001
    // adc.getAddr_ADC081C(ADC081C_ADDRESS_FLVA);         // 0x52, 1010010
    // adc.getAddr_ADC081C(ADC081C_ADDRESS_GNDFL);        // 0x54, 1010100
    // adc.getAddr_ADC081C(ADC081C_ADDRESS_GNDGND);       // 0x55, 1010101
    // adc.getAddr_ADC081C(ADC081C_ADDRESS_GNDVA);        // 0x56, 1010110
    // adc.getAddr_ADC081C(ADC081C_ADDRESS_VAFL);         // 0x58, 1011000
    // adc.getAddr_ADC081C(ADC081C_ADDRESS_VAGND);        // 0x59, 1011001
    // adc.getAddr_ADC081C(ADC081C_ADDRESS_VAVA);         // 0x5A, 1011010


    // The Automatic Conversion Mode, Alert Hold, Alert Flag Enable,
    // Alert Pin Enable and ALERT pin Polarity
    // can be changed via the following functions

    adc.setCycleTime(CYCLE_TIME_32);                    // Tconvert x 32, 27 ksps
    // adc.setCycleTime(AUTOMATIC_MODE_DISABLED);       // Automatic Mode Disabled, 0 ksps
    // adc.setCycleTime(CYCLE_TIME_64);                 // Tconvert x 64, 13.5 ksps
    // adc.setCycleTime(CYCLE_TIME_128);                // Tconvert x 128, 6.7 ksps
    // adc.setCycleTime(CYCLE_TIME_256);                // Tconvert x 256, 3.4 ksps
    // adc.setCycleTime(CYCLE_TIME_512);                // Tconvert x 512, 1.7 ksps
    // adc.setCycleTime(CYCLE_TIME_1024);               // Tconvert x 1024, 0.9 ksps
    // adc.setCycleTime(CYCLE_TIME_2048);               // Tconvert x 2048, 0.4 ksps

    adc.setAlertHold(ALERT_HOLD_CLEAR);                 // Alerts will self-clear
    // adc.setAlertHold(ALERT_HOLD_NOCLEAR);            // Alerts will not self-clear

    adc.setAlertFlag(ALERT_FLAG_DISABLE);               // Disables alert status bit in the Conversion Result register
    // adc.setAlertFlag(ALERT_FLAG_ENABLE);             // Enables alert status bit in the Conversion Result register

    adc.setAlertPin(ALERT_PIN_DISABLE);                 // Disables the ALERT output pin
    // adc.setAlertPin(ALERT_PIN_ENABLE);               // Enables the ALERT output pin

    adc.setPolarity(POLARITY_LOW);                      // Sets the ALERT pin to active low
    // adc.setPolarity(POLARITY_HIGH);                  // Sets the ALERT pin to active high

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
        uint8_t raw_adc;

        Serial.println("Getting Conversion Readings from ADC081C");
        Serial.println(" ");
        raw_adc = adc.Measure_Voltage();
        Serial.print("Digital Value of Analog Input: ");
        Serial.println(raw_adc);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("ADC081C Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
