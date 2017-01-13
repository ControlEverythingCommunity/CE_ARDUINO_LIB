/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ISL29003
        This code is designed to work with the ISL29003_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=ISL29003_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <ISL29003.h>

ISL29003 isl;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    isl.getAddr_ISL29003(ISL29003_DEFAULT_ADDRESS);           // 0x44, 1000100


    // The ADC Core Output, ADC Power Down Mode and ADC Work Mode,
    // The ADC Gain Range and Interrupt Persist Time
    // can be changed via the following functions

    isl.setADCCore(ADC_ENABLE);             // Enable ADC-Core
    // isl.setADCCore(ADC_DISABLE);         // Disable ADC-Core

    isl.setADCPD(ADCPD_NORMAL);             // ADC Normal Operation
    // isl.setADCPD(ADCPD_PDOWN);           // Power-Down Mode

    isl.setADCMode(ADCMODE_DIODE1);         // Diode1’s current to unsigned 16-bit data
    // isl.setADCMode(ADCMODE_DIODE2);      // Diode2’s current to unsigned 16-bit data
    // isl.setADCMode(ADCMODE_DIFF);        // Difference between diodes (I1 - I2) to signed 15-bit data
    // isl.setADCMode(ADCMODE_RES);         // Reserved

    isl.setADCGain(GAIN_64K);               // 0 to 64000 lux
    // isl.setADCGain(GAIN_1K);             // 0 to 1000 lux
    // isl.setADCGain(GAIN_4K);             // 0 to 4000 lux
    // isl.setADCGain(GAIN_16K);            // 0 to 16000 lux

    isl.setIntrCycle(INTRCYCLE_1);          // 1 Integration Cycle
    // isl.setIntrCycle(INTRCYCLE_4);       // 4 Integration Cycles
    // isl.setIntrCycle(INTRCYCLE_8);       // 8 Integration Cycles
    // isl.setIntrCycle(INTRCYCLE_16);      // 16 Integration Cycles

    isl.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = isl.isl_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        uint16_t luminance;
        Serial.println("Getting Integrated Light Readings from ISL29003");
        Serial.println(" ");
        // Read and print out the integrated light value in lux
        luminance = isl.Measure_Light();

        // Output data to screen
        Serial.print("Ambient Light Luminance: ");
        Serial.print(luminance);
        Serial.println(" lux");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("ISL29003 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
