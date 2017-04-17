/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ISL29125
        This code is designed to work with the ISL29125_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/isl29125-digital-rgb-color-light-sensor-with-ir-blocking-filter
*/
/**************************************************************************/

#include <Wire.h>
#include <ISL29125.h>

ISL29125 isl;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    isl.getAddr_ISL29125(ISL29125_DEFAULT_ADDRESS);           // 0x44, 1000100


    // The ADC Resolution, RGB Data Sensing Range and, RGB Data Sensing Range
    // can be changed via the following functions
    /*
    // Also some more parameters can be changed using the following functions
    // SYNC Mode, Active Infrared (IR) Compensation Offset/Adjust
    // RGB Conversion Done to INT Control, Interrupt Persist Control, Interrupt Threshold Assignment
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */


    isl.setResolution(RESOLUTION_16);       // 16 Bits
    // isl.setResolution(RESOLUTION_12);    // 12 Bits

    isl.setRange(RANGE_10KLUX);             // 10,000 lux
    // isl.setRange(RANGE_375LUX);          // 375 lux

    isl.setMode(MODE_RGB);                  // GREEN/RED/BLUE
    // isl.setMode(MODE_POWERDOWN);         // Power Down (ADC conversion)
    // isl.setMode(MODE_GREEN);             // GREEN Only
    // isl.setMode(MODE_RED);               // RED Only
    // isl.setMode(MODE_BLUE);              // BLUE Only
    // isl.setMode(MODE_STANDBY);           // Stand by (No ADC conversion)
    // isl.setMode(MODE_RG);                // GREEN/RED
    // isl.setMode(MODE_GB);                // GREEN/BLUE

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
        Serial.println("Getting Color Data Readings from ISL29125");
        Serial.println(" ");
        // Set up for Digital RGB Color Light Sensor with IR Blocking Filter
        isl.setUpColor();
        delay(500);

        // Display the results for Color Values
        isl.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Green Color Illuminance: ");
        Serial.print(isl.isl_colorData.G);
        Serial.println(" lx");
        Serial.print("Red Color Illuminance: ");
        Serial.print(isl.isl_colorData.R);
        Serial.println(" lx");
        Serial.print("Blue Color Illuminance: ");
        Serial.print(isl.isl_colorData.B);
        Serial.println(" lx");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("ISL29125 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
