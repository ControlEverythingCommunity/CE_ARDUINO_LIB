/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ISL29001
        This code is designed to work with the ISL29001_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <ISL29001.h>

ISL29001 isl;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    isl.getAddr_ISL29001(ISL29001_DEFAULT_ADDRESS);                // 0x44 (1000100)

    // The Primary Commands used to control ADC can altered via the following function
    isl.begin(ADC_DIODE_1_INT);             // ADC converts Diode 1’s current (IDIODE1) with internal integration timing
    // isl.begin(ADC_POWER_DOWN);           // ADC is Powered-Down
    // isl.begin(ADC_RESET);                // ADC is Reset
    // isl.begin(ADC_DIODE_2_INT);          // ADC converts Diode 2’s current (IDIODE2) with internal integration timing
    // isl.begin(ADC_DIODE_DIFF_INT);       // ADC converts IDIODE1-IDIODE2 with internal integration timing
    // isl.begin(ADC_DIODE_1_EXT);          // ADC converts Diode 1’s current (IDIODE1) with external integration timing
    // isl.begin(ADC_DIODE_2_EXT);          // ADC converts Diode 2’s current (IDIODE2) with external integration timing
    // isl.begin(ADC_DIODE_DIFF_EXT);       // ADC converts IDIODE1-IDIODE2 with external integration timing
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
        uint16_t light;
        float luminance;

        Serial.println("Getting Ambient Light Readings from ISL29001");
        Serial.println(" ");
        // Read and print out the Integrated Ambient Light data
        light = isl.Measure_Light();
        /*
            E (lux) = 1/32768 . 10,000 lux / (REXT / 100 kΩ) . Data 1

                REXT    INTEGRATION         LUX RANGE       RESOLUTION,
                (kΩ)        TIME(ms)        (lux)           COUNTS/LUX
                50 (Min)    50              20,000              1.6
                100         100             10,000              3
            Recommended
                200         200             5,000               6
                500 (Max)   500             2,000               16
        */
        luminance = light * (10000.0 / 32768.0);

        // Output data to screen
        Serial.print("Ambient Light Luminance: ");
        Serial.print(luminance);
        Serial.println(" lux");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("ISL29001 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
