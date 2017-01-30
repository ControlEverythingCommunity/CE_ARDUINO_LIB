/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BH1745NUC
        This code is designed to work with the BH1745NUC_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=BH1745NUC_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <BH1745NUC.h>

BH1745NUC bh;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    bh.getAddr_BH1745NUC(BH1745NUC_DEFAULT_ADDRESS);    // 0x38, 0111000
    // bh.getAddr_BH1745NUC(BH1745NUC_ADDRESS_HIGH);    // 0x39, 0111001

    // The Intial Reset Mode, Interrupt Pin Status,
    // RGBC Measurement Time,
    // RGBC Measurement Enable, Validity of the RGBC Data Update and Gain Setting for RGBC Measurement,
    // Interrupt Status of RGBC, Interrupt Pin Latch Status,
    // Interrupt Source Select and Interrupt Pin Status,
    // and Interrupt Persistence Function Status
    // can be changed via the following functions

    bh.setSWReset(SW_RESET_NOT_START);                  // Initial reset is not started
    // bh.setSWReset(SW_RESET_START);                   // Initial reset is started

    bh.setMeasTime(MEAS_TIME_160);                      // 000 : 160 msec
    // bh.setMeasTime(MEAS_TIME_320);                   // 001 : 320 msec
    // bh.setMeasTime(MEAS_TIME_640);                   // 010 : 640 msec
    // bh.setMeasTime(MEAS_TIME_1280);                  // 011 : 1280 msec
    // bh.setMeasTime(MEAS_TIME_2560);                  // 100 : 2560 msec
    // bh.setMeasTime(MEAS_TIME_5120);                  // 101 : 5120 msec

    bh.setRGBCValid(RGBC_VALID_UPDATE);                 // RGBC data is updated after last writing MODE_CONTROL1,2 register or last reading MODE_CONTROL2 register
    // bh.setRGBCValid(RGBC_VALID_NOT_UPDATE);          // RGBC data is not updated after last writing MODE_CONTROL1,2 register or last reading MODE_CONTROL2 register

    bh.setRGBCEnable(RGBC_EN_ACTIVE);                   // RGBC measurement is active
    // bh.setRGBCEnable(RGBC_EN_INACTIVE);              // RGBC measurement is inactive and becomes power down

    bh.setADCGain(ADC_GAIN_1X);                         // 00 : 1X
    // bh.setADCGain(ADC_GAIN_2X);                      // 00 : 2X
    // bh.setADCGain(ADC_GAIN_16X);                     // 00 : 16X

    bh.setINTStatus(STATUS_INACTIVE);                   // Interrupt signal is inactive
    // bh.setINTStatus(STATUS_ACTIVE);                  // Interrupt signal is active

    bh.setINTLatch(INT_LATCH_LATCHED);                  // INTERRUPT pin is latched until INTERRUPT register is read or initialized
    // bh.setINTLatch(INT_LATCH_UPDATED);               // INTERRUPT pin is updated after each measurement

    bh.setINTEnable(INT_ENABLE_DISABLE);                // INTERRUPT pin disable
    // bh.setINTEnable(INT_ENABLE_ENABLE);              // INTERRUPT pin ensable

    bh.setPersistance(PERSISTENCE_UPDATED_EACH);        // Interrupt status is updated at each measurement end
    // bh.setPersistance(PERSISTENCE_UPDATED_EACH);     // Interrupt status is toggled at each measurement end
    // bh.setPersistance(PERSISTENCE_UPDATED_EACH);     // Interrupt status is updated if 4 consecutive threshold judgments are the same
    // bh.setPersistance(PERSISTENCE_UPDATED_EACH);     // Interrupt status is updated if 8 consecutive threshold judgments are the same

    // Initialize the sensor for the color measurements
    bh.Initialize();

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
        uint16_t red, green, blue, clear;

        Serial.println("Getting Color Readings from BH1745NUC");
        Serial.println(" ");
        // Set Up the Color Sensor
        bh.Initialize();
        delay(500);
        // Read and print out the colors in lux values
        red = bh.getRedColor();
        green = bh.getGreenColor();
        blue = bh.getBlueColor();
        clear = bh.getClearColor();

        // Output data to screen
        Serial.print("Red Color Illuminance: ");
        Serial.println(red);
        Serial.print("Green Color Illuminance: ");
        Serial.println(green);
        Serial.print("Blue Color Illuminance: ");
        Serial.println(blue);
        Serial.print("Clear Color Illuminance: ");
        Serial.println(clear);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("BH1745NUC Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
