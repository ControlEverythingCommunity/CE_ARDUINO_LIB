/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADC121C_MQ131
        This code is designed to work with the ADC121C_I2CGAS_MQ131 I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/ozone-gas-sensor-12-bit
        NT.
*/
/**************************************************************************/

#include <Wire.h>
#include <ADC121C_MQ131.h>

ADC121C_MQ131 mq131;


void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    mq131.getAddr_ADC121C_MQ131(ADC121C_MQ131_DEFAULT_ADDRESS);         // 0x50, 1010000
    // mq131.getAddr_ADC121C_MQ131(ADC121C_MQ131_ADDRESS_FLGND);        // 0x51, 1010001
    // mq131.getAddr_ADC121C_MQ131(ADC121C_MQ131_ADDRESS_FLVA);         // 0x52, 1010010
    // mq131.getAddr_ADC121C_MQ131(ADC121C_MQ131_ADDRESS_GNDFL);        // 0x54, 1010100
    // mq131.getAddr_ADC121C_MQ131(ADC121C_MQ131_ADDRESS_GNDGND);       // 0x55, 1010101
    // mq131.getAddr_ADC121C_MQ131(ADC121C_MQ131_ADDRESS_GNDVA);        // 0x56, 1010110
    // mq131.getAddr_ADC121C_MQ131(ADC121C_MQ131_ADDRESS_VAFL);         // 0x58, 1011000
    // mq131.getAddr_ADC121C_MQ131(ADC121C_MQ131_ADDRESS_VAGND);        // 0x59, 1011001
    // mq131.getAddr_ADC121C_MQ131(ADC121C_MQ131_ADDRESS_VAVA);         // 0x5A, 1011010


    // The Automatic Conversion Mode, Alert Hold, Alert Flag Enable,
    // Alert Pin Enable and ALERT pin Polarity
    // can be changed via the following functions

    mq131.setCycleTime(CYCLE_TIME_32);                    // Tconvert x 32, 27 ksps
    // mq131.setCycleTime(AUTOMATIC_MODE_DISABLED);       // Automatic Mode Disabled, 0 ksps
    // mq131.setCycleTime(CYCLE_TIME_64);                 // Tconvert x 64, 13.5 ksps
    // mq131.setCycleTime(CYCLE_TIME_128);                // Tconvert x 128, 6.7 ksps
    // mq131.setCycleTime(CYCLE_TIME_256);                // Tconvert x 256, 3.4 ksps
    // mq131.setCycleTime(CYCLE_TIME_512);                // Tconvert x 512, 1.7 ksps
    // mq131.setCycleTime(CYCLE_TIME_1024);               // Tconvert x 1024, 0.9 ksps
    // mq131.setCycleTime(CYCLE_TIME_2048);               // Tconvert x 2048, 0.4 ksps

    mq131.setAlertHold(ALERT_HOLD_CLEAR);                 // Alerts will self-clear
    // mq131.setAlertHold(ALERT_HOLD_NOCLEAR);            // Alerts will not self-clear

    mq131.setAlertFlag(ALERT_FLAG_DISABLE);               // Disables alert status bit in the Conversion Result register
    // mq131.setAlertFlag(ALERT_FLAG_ENABLE);             // Enables alert status bit in the Conversion Result register

    mq131.setAlertPin(ALERT_PIN_DISABLE);                 // Disables the ALERT output pin
    // mq131.setAlertPin(ALERT_PIN_ENABLE);               // Enables the ALERT output pin

    mq131.setPolarity(POLARITY_LOW);                      // Sets the ALERT pin to active low
    // mq131.setPolarity(POLARITY_HIGH);                  // Sets the ALERT pin to active high

    mq131.begin();

    // Calibrate your Sensor to Obtain Precise Value
    mq131.Calibration();
    // We Deduce Resistance of the Sensor in Clean Air (Ro) in the Serial Port
    // Serial.print("Ro = ");
    // Serial.println(mq131.Measure_Ro());
}

void loop(void)
{
    byte error;
    int8_t address;

    address = mq131.ADC121C_MQ131_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        uint16_t result;

        Serial.println("Getting Gas Readings from ADC121C_MQ131");
        Serial.println(" ");
        // Serial.print("Rs/Ro Ratio: ");
        // Serial.println(mq131.Measure_RatioRsRo());

        // The Graph at fig of datasheet (sensitivity characteristics of the MQ-131)
        // seems a power function y = a*x^b
        // so ppm = a*(Rs/Ro)^b
        // Using Power Regression, you can obtain Slope (a), and Logarithmic Value of ppm (b) for Ozone (O3)!
        // From the graph, we get points for O3: (5.0, 6.0), (100.0, 0.4)

        Serial.print("Concentration of Ozone is: ");
        float ppmO3 = mq131.Measure_Ozone(-0.90, 0.69);
        Serial.print(ppmO3, 3);
        Serial.println(" ppm ");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("ADC121C_MQ131 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
