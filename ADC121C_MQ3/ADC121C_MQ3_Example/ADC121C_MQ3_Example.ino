/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADC121C_MQ3
        This code is designed to work with the ADC121C_I2CGAS_MQ3 I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/alcohol-gas-sensor
*/
/**************************************************************************/

#include <Wire.h>
#include <ADC121C_MQ3.h>

ADC121C_MQ3 mq3;


void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    mq3.getAddr_ADC121C_MQ3(ADC121C_MQ3_DEFAULT_ADDRESS);         // 0x50, 1010000
    // mq3.getAddr_ADC121C_MQ3(ADC121C_MQ3_ADDRESS_FLGND);        // 0x51, 1010001
    // mq3.getAddr_ADC121C_MQ3(ADC121C_MQ3_ADDRESS_FLVA);         // 0x52, 1010010
    // mq3.getAddr_ADC121C_MQ3(ADC121C_MQ3_ADDRESS_GNDFL);        // 0x54, 1010100
    // mq3.getAddr_ADC121C_MQ3(ADC121C_MQ3_ADDRESS_GNDGND);       // 0x55, 1010101
    // mq3.getAddr_ADC121C_MQ3(ADC121C_MQ3_ADDRESS_GNDVA);        // 0x56, 1010110
    // mq3.getAddr_ADC121C_MQ3(ADC121C_MQ3_ADDRESS_VAFL);         // 0x58, 1011000
    // mq3.getAddr_ADC121C_MQ3(ADC121C_MQ3_ADDRESS_VAGND);        // 0x59, 1011001
    // mq3.getAddr_ADC121C_MQ3(ADC121C_MQ3_ADDRESS_VAVA);         // 0x5A, 1011010


    // The Automatic Conversion Mode, Alert Hold, Alert Flag Enable,
    // Alert Pin Enable and ALERT pin Polarity
    // can be changed via the following functions

    mq3.setCycleTime(CYCLE_TIME_32);                    // Tconvert x 32, 27 ksps
    // mq3.setCycleTime(AUTOMATIC_MODE_DISABLED);       // Automatic Mode Disabled, 0 ksps
    // mq3.setCycleTime(CYCLE_TIME_64);                 // Tconvert x 64, 13.5 ksps
    // mq3.setCycleTime(CYCLE_TIME_128);                // Tconvert x 128, 6.7 ksps
    // mq3.setCycleTime(CYCLE_TIME_256);                // Tconvert x 256, 3.4 ksps
    // mq3.setCycleTime(CYCLE_TIME_512);                // Tconvert x 512, 1.7 ksps
    // mq3.setCycleTime(CYCLE_TIME_1024);               // Tconvert x 1024, 0.9 ksps
    // mq3.setCycleTime(CYCLE_TIME_2048);               // Tconvert x 2048, 0.4 ksps

    mq3.setAlertHold(ALERT_HOLD_CLEAR);                 // Alerts will self-clear
    // mq3.setAlertHold(ALERT_HOLD_NOCLEAR);            // Alerts will not self-clear

    mq3.setAlertFlag(ALERT_FLAG_DISABLE);               // Disables alert status bit in the Conversion Result register
    // mq3.setAlertFlag(ALERT_FLAG_ENABLE);             // Enables alert status bit in the Conversion Result register

    mq3.setAlertPin(ALERT_PIN_DISABLE);                 // Disables the ALERT output pin
    // mq3.setAlertPin(ALERT_PIN_ENABLE);               // Enables the ALERT output pin

    mq3.setPolarity(POLARITY_LOW);                      // Sets the ALERT pin to active low
    // mq3.setPolarity(POLARITY_HIGH);                  // Sets the ALERT pin to active high

    mq3.begin();

    // Calibrate your Sensor to Obtain Precise Value
    mq3.Calibration();
    // We Deduce Resistance of the Sensor in Clean Air (Ro) in the Serial Port
    // Serial.print("Ro = ");
    // Serial.println(mq3.Measure_Ro());
}

void loop(void)
{
    byte error;
    int8_t address;

    address = mq3.ADC121C_MQ3_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        uint16_t result;

        Serial.println("Getting Gas Readings from ADC121C_MQ3");
        Serial.println(" ");
        // Serial.print("Rs/Ro Ratio: ");
        // Serial.println(mq3.Measure_RatioRsRo());

        // The Graph at fig of datasheet (sensitivity characteristics of the MQ-3)
        // seems a power function y = a*x^b
        // so ppm = a*(Rs/Ro)^b
        // Using Power Regression, you can obtain Slope (a), and Logarithmic Value of mg/L (b) for Alcohol!
        // From the graph, we get points for Alcohol: (0.1, 2.3), (10, 0.12)

        Serial.print("Concentration of Alcohol is: ");
        float MgLAlcohol = mq3.Measure_AlcoholMgL(-0.64, -1.0);
        Serial.print(MgLAlcohol, 3);
        Serial.println(" MgL");
        delay(500);
        Serial.print("Concentration of Alcohol is: ");
        float ppmAlcohol = mq3.Measure_Alcoholppm(-0.64, -1.0) * 2.2;
        Serial.print(ppmAlcohol, 3);
        Serial.println(" ppm ");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("ADC121C_MQ3 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
