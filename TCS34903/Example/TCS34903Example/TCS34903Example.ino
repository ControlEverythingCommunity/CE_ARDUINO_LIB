/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TCS34903
        This code is designed to work with the TCS34903FN_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=TCS34903FN_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <TCS34903.h>

TCS34903 tcs;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tcs.getAddr_TCS34903(TCS34903_DEFAULT_ADDRESS);           // 0x39

    // The Color Light-to-Digital Converter
    // Internal Integration Time of the Color Channel ADC’s in 2.78 ms Increments,
    // Wait Time, Color Gain
    // can be changed via the following functions

    /*
    // Also, some more parameters can be changed using the following functions
    // Color Light-to-Digital Converter:
    // Color Interrupt, Wait, Color and Power Enable,
    // Color Gain Level, Wait Long.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    tcs.setATime(ATIME_712MS);               // Cycles: 256, Time: 712 ms Max Count: 65535
    // tcs.setATime(ATIME_2_78MS);                 // Cycles: 1, Time: 2.78 ms Max Count: 1024
    // tcs.setATime(ATIME_27_8MS);              // Cycles: 10, Time: 27.8 ms Max Count: 10240
    // tcs.setATime(ATIME_103MS);               // Cycles: 37, Time: 100 ms Max Count: 37888
    // tcs.setATime(ATIME_178MS);               // Cycles: 64, Time: 178 ms Max Count: 65535

    tcs.setWTime(WTIME_1);                      // Wait Time: 1, Time (WLONG = 0): 2.78 ms Time (WLONG = 1):  0.03 sec
    // tcs.setWTime(WTIME_85);                  // Wait Time: 74, Time (WLONG = 0): 236 ms Time (WLONG = 1):  2.84 sec
    // tcs.setWTime(WTIME_256);                 // Wait Time: 256, Time (WLONG = 0): 712 ms Time (WLONG = 1):  8.54 sec

    tcs.setColorGain(AGAIN_1X);                 // Color Gain: 1X
    // tcs.setColorGain(AGAIN_4X);              // Color Gain: 4X
    // tcs.setColorGain(AGAIN_16X);             // Color Gain: 16X
    // tcs.setColorGain(AGAIN_64X);             // Color Gain: 64X

    tcs.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // TCS34903 Color Light-to-Digital Converter Address
    address = tcs.tcs_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Color Data Readings from TCS34903");
        Serial.println(" ");
        // Set up for Color Light-to-Digital Converter
        tcs.setUpColor();
        delay(500);

        // Display the results for Color Values
        tcs.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Clear/IR Data Illuminance: ");
        Serial.print(tcs.tcs_colorData.C);
        Serial.println(" lx");
        Serial.print("Red Color Illuminance: ");
        Serial.print(tcs.tcs_colorData.R);
        Serial.println(" lx");
        Serial.print("Green Color Illuminance: ");
        Serial.print(tcs.tcs_colorData.G);
        Serial.println(" lx");
        Serial.print("Blue Color Illuminance: ");
        Serial.print(tcs.tcs_colorData.B);
        Serial.println(" lx");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("TCS34903 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
