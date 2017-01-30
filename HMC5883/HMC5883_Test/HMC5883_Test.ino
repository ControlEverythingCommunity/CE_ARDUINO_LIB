/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HMC5883
        This code is designed to work with the HMC5883_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Compass?sku=HMC5883_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/
#include <Wire.h>
#include <HMC5883.h>

HMC5883 hmc;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    hmc.getAddr_HMC5883(HMC5883_DEFAULT_ADDRESS);   // 0x1E

    // The Number of Samples Averaged (1 to 8) per Measurement Output,
    // Data Output Rate, Measurement Configuration/Mode, Gain Configuration,
    // Operating Mode of the Device
    // can be changed via the following functions

    hmc.setMASample(MA_SAMPLE8);                // No. of Samples Averaged per Measurement Output: 8
    // hmc.setMASample(MA_SAMPLE1);             // No. of Samples Averaged per Measurement Output: 1 (Default)
    // hmc.setMASample(MA_SAMPLE2);             // No. of Samples Averaged per Measurement Output: 2
    // hmc.setMASample(MA_SAMPLE4);             // No. of Samples Averaged per Measurement Output: 4

    hmc.setDataRate(DATA_OUTPUT_RATE_0_75);     // Data Output Rate (Hz): 0.75
    // hmc.setDataRate(DATA_OUTPUT_RATE_1_5);   // Data Output Rate (Hz): 1.5
    // hmc.setDataRate(DATA_OUTPUT_RATE_3);     // Data Output Rate (Hz): 3
    // hmc.setDataRate(DATA_OUTPUT_RATE_7_5);   // Data Output Rate (Hz): 7.5
    // hmc.setDataRate(DATA_OUTPUT_RATE_15);    // Data Output Rate (Hz): 15 (Default)
    // hmc.setDataRate(DATA_OUTPUT_RATE_30);    // Data Output Rate (Hz): 30
    // hmc.setDataRate(DATA_OUTPUT_RATE_75);    // Data Output Rate (Hz): 75

    hmc.setMeasMode(NORMAL_MEASUREMENT);        // Normal measurement configuration (Default)
    // hmc.setMeasMode(POSITIVE_BIAS);          // Positive bias configuration for X, Y, and Z axes
    // hmc.setMeasMode(NEGATIVE_BIAS);          // Negative bias configuration for X, Y, and Z axes

    hmc.setMagGain(MAGGAIN_1_3);                // Recommended Sensor Field Range: ± 1.3 Ga, Gain (LSb/Gauss): 1090 (Default), Digital Resolution (mG/LSb): 0.92, Output Range: 0xF800–0x07FF (-2048–2047)
    // hmc.setMagGain(MAGGAIN_1_9);             // Recommended Sensor Field Range: ± 1.9 Ga, Gain (LSb/Gauss): 820, Digital Resolution (mG/LSb): 1.22, Output Range: 0xF800–0x07FF (-2048–2047)
    // hmc.setMagGain(MAGGAIN_2_5);             // RRecommended Sensor Field Range: ± 2.5 Ga, Gain (LSb/Gauss): 660, Digital Resolution (mG/LSb): 1.52, Output Range: 0xF800–0x07FF (-2048–2047)
    // hmc.setMagGain(MAGGAIN_4_0);             // Recommended Sensor Field Range: ± 4.0 Ga, Gain (LSb/Gauss): 440, Digital Resolution (mG/LSb): 2.27, Output Range: 0xF800–0x07FF (-2048–2047)
    // hmc.setMagGain(MAGGAIN_4_7);             // Recommended Sensor Field Range: ± 4.7 Ga, Gain (LSb/Gauss): 390, Digital Resolution (mG/LSb): 2.56, Output Range: 0xF800–0x07FF (-2048–2047)
    // hmc.setMagGain(MAGGAIN_5_6);             // Recommended Sensor Field Range: ± 5.6 Ga, Gain (LSb/Gauss): 330, Digital Resolution (mG/LSb): 3.03, Output Range: 0xF800–0x07FF (-2048–2047)
    // hmc.setMagGain(MAGGAIN_8_1);             // Recommended Sensor Field Range: ± 8.1 Ga, Gain (LSb/Gauss): 230, Digital Resolution (mG/LSb): 4.35, Output Range: 0xF800–0x07FF (-2048–2047)

    hmc.setOpMode(CONTINOUS_MEASUREMENT);       // Continuous-Measurement Mode
    // hmc.setOpMode(SINGLE_MEASUREMENT);       // Single-Measurement Mode (Default)
    // hmc.setOpMode(IDLE_MEASUREMENT);         // Idle Mode. Device is placed in idle mode

    hmc.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = hmc.hmc_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Magnetic Field Readings from HMC5883");
        Serial.println(" ");
        // Display the results for magnetic vector values
        hmc.Measure_Magnetometer();

        // Output data to screen
        Serial.print("Magnetic Field in X-Axis: ");
        Serial.println(hmc.hmc_magdata.X);
        Serial.print("Magnetic Field in Y-Axis: ");
        Serial.println(hmc.hmc_magdata.Y);
        Serial.print("Magnetic Field in Z-Axis: ");
        Serial.println(hmc.hmc_magdata.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("HMC5883 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
