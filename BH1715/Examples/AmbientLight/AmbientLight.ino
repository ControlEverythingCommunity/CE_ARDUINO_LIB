/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BH1715
        This code is designed to work with the BH1715_I2CADC I2C Mini Module available from ControlEverything.com.
        hhttps://www.controleverything.com/content/Light?sku=BH1715_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <BH1715.h>

BH1715 bh;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    bh.getAddr_BH1715(BH1715_DEFAULT_ADDRESS);          // 0x23, 0100011
    // bh.getAddr_BH1715(BH1715_ADDR_H_ADDRESS);        // 0x5C, 1011100


    // The Measurement Mode and Resolution Value
    // can be changed via the following functions

    bh.setResMode(CONTINOUS_HIGH);          // Measure Ambient Light, Continous High Resolution Mode
    // bh.setResMode(CONTINOUS_LOW);        // Measure Ambient Light, Continous Low Resolution Mode
    // bh.setResMode(ONE_HIGH);             // Measure Ambient Light, One Time High Resolution Mode
    // bh.setResMode(ONE_LOW);              // Measure Ambient Light, One Time Low Resolution Mode

    bh.setResolution(RESOLUTION_DEFAULT);   // MTreg value: 0100_0101 (MTreg: 69), lx / count: 0.83
    // bh.setResolution(RESOLUTION_MIN);    // MTreg value: 0001_1111 (MTreg: 31), lx / count: 1.85
    // bh.setResolution(RESOLUTION_MAX);    // MTreg value: 1111_1110 (MTreg: 254), lx / count: 0.23

    bh.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = bh.bh_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        uint16_t raw_lux;
        float luminance;

        Serial.println("Getting  Ambient Light Readings from BH1715");
        Serial.println(" ");
        // Read and print out the Ambient Light Luminance
        /*
            Illuminance per 1 count (lx / count) = (1 / 1.2) * ( 69 / X )
            1.2 : Measurement accuracy
            69 : Default value of MTreg (dec)
            X : MTreg value
        */
        raw_lux = bh.Measure_Lux();
        luminance = 0.83 * raw_lux;

        // Output data to screen
        Serial.print("Ambient Light Luminance is: ");
        Serial.print(luminance);
        Serial.println("lux");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("BH1715 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
