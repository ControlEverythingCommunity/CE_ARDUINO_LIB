/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TCS3414
        This code is designed to work with the TCS3414_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=TCS3414_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <TCS3414.h>

TCS3414 tcs;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tcs.getAddr_TCS3414(TCS3414_DEFAULT_ADDRESS);           // 0x39

    // The Digital Color Sensor
    // Preset Integration Time, NOMINAL INTEGRATION TIME,
    // Analog Gain Control and 6-bit Digital Prescaler and Divider
    // can be changed via the following functions

    /*
    // Also, some more parameters can be changed using the following functions
    // Digital Color Sensor:
    // Sync Pin Edge, SYNC IN PULSE COUNT, Stop ADC Integration on Interrupt,
    // INTR Control Select, INTR Control Select, Interrupt Source
    // Four-Channel RGBC ADC Enable to Begin Integration and Power ON.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    tcs.setIntegMode(INTEG_MODE_FREE);                      // In this Mode, the Integrator is Free-Running and One of the Three Internally-Generated Nominal Integration Times is Selected for Each Conversion
    // tcs.setIntegMode(INTEG_MODE_MANUAL);                 // Manually Start/Stop Integration through Serial Bus using ADC_EN Field in Control Register
    // tcs.setIntegMode(INTEG_MODE_SYN_SINGLE);             // Synchronize Exactly One Internally-Timed Integration Cycle as Specified in the NOMINAL INTEGRATION TIME Beginning 2.4 μs After Being Initiated by the SYNC IN Pin
    // tcs.setIntegMode(INTEG_MODE_SYN_MULTI);              // Integrate over Specified Number of Pulses on SYNC IN pin. Minimum width of sync pulse is 50 μs. SYNC IN must be low at least 3.6 μs

    tcs.setIntegParamTime(INTEG_PARAM_INTTIME_12MS);        // 12 ms
    // tcs.setIntegParamTime(INTEG_PARAM_INTTIME_100MS);    // 100 ms
    // tcs.setIntegParamTime(INTEG_PARAM_INTTIME_400MS);    // 400 ms

    tcs.setAnalogGain(GAIN_1X);                             // 1X
    // tcs.setAnalogGain(GAIN_4X);                          // 4X
    // tcs.setAnalogGain(GAIN_16X);                         // 16X
    // tcs.setAnalogGain(GAIN_64X);                         // 64X

    tcs.setPrescaler(PRESCALER_1);                          // Divide by 1
    // tcs.setPrescaler(PRESCALER_2);                       // Divide by 2
    // tcs.setPrescaler(PRESCALER_4);                       // Divide by 4
    // tcs.setPrescaler(PRESCALER_8);                       // Divide by 8
    // tcs.setPrescaler(PRESCALER_16);                      // Divide by 16
    // tcs.setPrescaler(PRESCALER_32);                      // Divide by 32
    // tcs.setPrescaler(PRESCALER_64);                      // Divide by 64

    tcs.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // TCS3414 Digital Color Sensor Address
    address = tcs.tcs_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Color Data Readings from TCS3414");
        Serial.println(" ");
        // Set up for Digital Color Sensor
        tcs.setUpColor();
        delay(500);

        // Display the results for Color and Proximity Values
        tcs.Measure_Color();

        // Output Data to Screen
        Serial.print("Green Color Illuminance: ");
        Serial.print(tcs.tcs_colorData.G);
        Serial.println(" lx");
        Serial.print("Red Color Illuminance: ");
        Serial.print(tcs.tcs_colorData.R);
        Serial.println(" lx");
        Serial.print("Blue Color Illuminance: ");
        Serial.print(tcs.tcs_colorData.B);
        Serial.println(" lx");
        Serial.print("Clear Data Illuminance: ");
        Serial.print(tcs.tcs_colorData.C);
        Serial.println(" lx");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("TCS3414 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
