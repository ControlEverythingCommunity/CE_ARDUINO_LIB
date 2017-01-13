/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MAX44009
        This code is designed to work with the MAX44009_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.Controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <MAX44009.h>

MAX44009 max;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    max.getAddr_MAX44009(MAX44009_DEFAULT_ADDRESS);           // 0x4A, 1001010
    // max.getAddr_MAX44009(MAX44009_VCC_ADDRESS);           // 0x4B, 1001011


    // The Continuous Mode, Manual Configuration Mode
    // Current Division Ratio (CDR) and Integration Timer Bits
    // can be changed via the following functions

    max.setContMode(CONTMODE_CONTIN);               // Continuous mode. The IC continuously measures lux intensity
    // max.setContMode(CONTMODE_DEFAULT);           // Default mode. The IC measures lux intensity only once every 800ms regardless of integration time.

    max.setManualMode(MANUAL_MODEON);               // Manual mode of configuration is used for the IC
    // max.setManualMode(MANUAL_DEFAULT);           // Default mode of configuration is used for the IC

    max.setCDR(CDR_NODIVIDED);                      // Current not divided. All of the photodiode current goes to the ADC
    // max.setCDR(CDR_DIVIDED);                     // Current divided by 8. Only 1/8 of the photodiode current goes to the ADC. This mode is used in high-brightness situations.

    max.setIntrTimer(INTRTIMER_800);                // This is a preferred mode for boosting low-light sensitivity
    // max.setIntrTimer(INTRTIMER_400);             // -
    // max.setIntrTimer(INTRTIMER_200);             // -
    // max.setIntrTimer(INTRTIMER_100);             // This is a preferred mode for high-brightness applications
    // max.setIntrTimer(INTRTIMER_50);              // Manual Mode only
    // max.setIntrTimer(INTRTIMER_25);              // Manual Mode only
    // max.setIntrTimer(INTRTIMER_12_5);            // Manual Mode only
    // max.setIntrTimer(INTRTIMER_6_25);            // Manual Mode only

    max.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = max.max_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        float luminance;
        Serial.println("Getting Ambient Light Readings from MAX44009");
        Serial.println(" ");
        // Read and print out the ambient light value in lux
        luminance = max.Measure_Light();

        // Output data to screen
        Serial.print("Ambient Light Luminance: ");
        Serial.print(luminance);
        Serial.println(" lux");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("MAX44009 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
