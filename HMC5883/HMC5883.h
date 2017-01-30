/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HMC5883
        This code is designed to work with the HMC5883_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Compass?sku=HMC5883_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

/**************************************************************************
    I2C ADDRESS/BITS
**************************************************************************/
    #define HMC5883_DEFAULT_ADDRESS             (0x1E)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define HMC5883_CONVERSIONDELAY             (100)

/**************************************************************************
    REGISTERS DESCRIPTION
**************************************************************************/
    #define HMC5883_REG_MAG_CONFIG_A            (0x00)      // Configuration Register A
    #define HMC5883_REG_MAG_CONFIG_B            (0x01)      // Configuration Register B
    #define HMC5883_REG_MAG_MODE                (0x02)      // COMPASS Control Register 1
    #define HMC5883_REG_MAG_OUT_X_MSB           (0x03)      // Data Output X MSB Register
    #define HMC5883_REG_MAG_OUT_X_LSB           (0x04)      // Data Output X LSB Register
    #define HMC5883_REG_MAG_OUT_Z_MSB           (0x05)      // Data Output Z MSB Register
    #define HMC5883_REG_MAG_OUT_Z_LSB           (0x06)      // Data Output Z LSB Register
    #define HMC5883_REG_MAG_OUT_Y_MSB           (0x07)      // Data Output Y MSB Register
    #define HMC5883_REG_MAG_OUT_Y_LSB           (0x08)      // Data Output Y LSB Register
    #define HMC5883_REG_MAG_STATUS              (0x09)      // Status Register
    #define HMC5883_REG_MAG_IDENTITY_A          (0x0A)      // Identification Register A
    #define HMC5883_REG_MAG_IDENTITY_B          (0x0B)      // Identification Register B
    #define HMC5883_REG_MAG_IDENTITY_C          (0x0C)      // Identification Register C

/**************************************************************************
    CONFIGURATION REGISTER A
**************************************************************************/
    #define HMC5883_REG_MAG_CRA_MA_MASK         (0x60)  // Select number of samples averaged (1 to 8) per measurement output
    #define HMC5883_REG_MAG_CRA_MA_1            (0x00)  // 00 = 1 (Default)
    #define HMC5883_REG_MAG_CRA_MA_2            (0x20)  // 01 = 2
    #define HMC5883_REG_MAG_CRA_MA_4            (0x40)  // 10 = 4
    #define HMC5883_REG_MAG_CRA_MA_8            (0x60)  // 11 = 8

    #define HMC5883_REG_MAG_CRA_DO_MASK         (0x1C)  // Data Output Rate Bits. These bits set the rate at which data is written to all three data output registers
    #define HMC5883_REG_MAG_CRA_DO_0_75         (0x00)  // Data Output Rate (Hz): 0.75
    #define HMC5883_REG_MAG_CRA_DO_1_5          (0x04)  // Data Output Rate (Hz): 1.5
    #define HMC5883_REG_MAG_CRA_DO_3            (0x08)  // Data Output Rate (Hz): 3
    #define HMC5883_REG_MAG_CRA_DO_7_5          (0x0C)  // Data Output Rate (Hz): 7.5
    #define HMC5883_REG_MAG_CRA_DO_15           (0x10)  // Data Output Rate (Hz): 15 (Default)
    #define HMC5883_REG_MAG_CRA_DO_30           (0x14)  // Data Output Rate (Hz): 30
    #define HMC5883_REG_MAG_CRA_DO_75           (0x18)  // Data Output Rate (Hz): 75

    #define HMC5883_REG_MAG_CRA_MS_MASK         (0x03)  // Measurement Mode
    #define HMC5883_REG_MAG_CRA_MS_NORMAL       (0x00)  // Normal measurement configuration (Default)
    #define HMC5883_REG_MAG_CRA_MS_POSITIVE     (0x01)  // Positive bias configuration for X, Y, and Z axes
    #define HMC5883_REG_MAG_CRA_MS_NEGATIVE     (0x02)  // Negative bias configuration for X, Y, and Z axes

/**************************************************************************
    CONFIGURATION REGISTER A
**************************************************************************/
    #define HMC5883_REG_MAG_CRB_GN_MASK         (0xE0)  // Gain Configuration Bits
    #define HMC5883_REG_MAG_CRB_GN_1_3          (0x20)  // Recommended Sensor Field Range: ± 1.3 Ga, Gain (LSb/Gauss): 1090 (Default), Digital Resolution (mG/LSb): 0.92, Output Range: 0xF800–0x07FF (-2048–2047)
    #define HMC5883_REG_MAG_CRB_GN_1_9          (0x40)  // Recommended Sensor Field Range: ± 1.9 Ga, Gain (LSb/Gauss): 820, Digital Resolution (mG/LSb): 1.22, Output Range: 0xF800–0x07FF (-2048–2047)
    #define HMC5883_REG_MAG_CRB_GN_2_5          (0x60)  // Recommended Sensor Field Range: ± 2.5 Ga, Gain (LSb/Gauss): 660, Digital Resolution (mG/LSb): 1.52, Output Range: 0xF800–0x07FF (-2048–2047)
    #define HMC5883_REG_MAG_CRB_GN_4_0          (0x80)  // Recommended Sensor Field Range: ± 4.0 Ga, Gain (LSb/Gauss): 440, Digital Resolution (mG/LSb): 2.27, Output Range: 0xF800–0x07FF (-2048–2047)
    #define HMC5883_REG_MAG_CRB_GN_4_7          (0xA0)  // Recommended Sensor Field Range: ± 4.7 Ga, Gain (LSb/Gauss): 390, Digital Resolution (mG/LSb): 2.56, Output Range: 0xF800–0x07FF (-2048–2047)
    #define HMC5883_REG_MAG_CRB_GN_5_6          (0xC0)  // Recommended Sensor Field Range: ± 5.6 Ga, Gain (LSb/Gauss): 330, Digital Resolution (mG/LSb): 3.03, Output Range: 0xF800–0x07FF (-2048–2047)
    #define HMC5883_REG_MAG_CRB_GN_8_1          (0xE0)  // Recommended Sensor Field Range: ± 8.1 Ga, Gain (LSb/Gauss): 230, Digital Resolution (mG/LSb): 4.35, Output Range: 0xF800–0x07FF (-2048–2047)

/**************************************************************************
    MODE REGISTER
**************************************************************************/
    #define HMC5883_REG_MAG_MR_HS_MASK          (0x80)  // Select High Speed I2C, 3400kHz
    #define HMC5883_REG_MAG_MR_HS_DISABLE       (0x00)  // Disables High Speed I2C, 3400kHz
    #define HMC5883_REG_MAG_MR_HS_ENABLE        (0x80)  // Enables High Speed I2C, 3400kHz

    #define HMC5883_REG_MAG_MR_MD_MASK          (0x03)  // Operating Mode
    #define HMC5883_REG_MAG_MR_MD_CONT          (0x00)  // Continuous-Measurement Mode
    #define HMC5883_REG_MAG_MR_MD_SINGLE        (0x01)  // Single-Measurement Mode (Default)
    #define HMC5883_REG_MAG_MR_MD_IDLE          (0x03)  // Idle Mode. Device is placed in idle mode


typedef enum
{
    MA_SAMPLE1                      = HMC5883_REG_MAG_CRA_MA_1,
    MA_SAMPLE2                      = HMC5883_REG_MAG_CRA_MA_2,
    MA_SAMPLE4                      = HMC5883_REG_MAG_CRA_MA_4,
    MA_SAMPLE8                      = HMC5883_REG_MAG_CRA_MA_8

} hmcMASample_t;

typedef enum
{
    DATA_OUTPUT_RATE_0_75           = HMC5883_REG_MAG_CRA_DO_0_75,
    DATA_OUTPUT_RATE_1_5            = HMC5883_REG_MAG_CRA_DO_1_5,
    DATA_OUTPUT_RATE_3              = HMC5883_REG_MAG_CRA_DO_3,
    DATA_OUTPUT_RATE_7_5            = HMC5883_REG_MAG_CRA_DO_7_5,
    DATA_OUTPUT_RATE_15             = HMC5883_REG_MAG_CRA_DO_15,
    DATA_OUTPUT_RATE_30             = HMC5883_REG_MAG_CRA_DO_30,
    DATA_OUTPUT_RATE_75             = HMC5883_REG_MAG_CRA_DO_75
    
} hmcDataRate_t;

typedef enum
{
    NORMAL_MEASUREMENT              = HMC5883_REG_MAG_CRA_MS_NORMAL,
    POSITIVE_BIAS                   = HMC5883_REG_MAG_CRA_MS_POSITIVE,
    NEGATIVE_BIAS                   = HMC5883_REG_MAG_CRA_MS_NEGATIVE
    
} hmcMeasMode_t;

typedef enum
{
    MAGGAIN_1_3                     = HMC5883_REG_MAG_CRB_GN_1_3,
    MAGGAIN_1_9                     = HMC5883_REG_MAG_CRB_GN_1_9,
    MAGGAIN_2_5                     = HMC5883_REG_MAG_CRB_GN_2_5,
    MAGGAIN_4_0                     = HMC5883_REG_MAG_CRB_GN_4_0,
    MAGGAIN_4_7                     = HMC5883_REG_MAG_CRB_GN_4_7,
    MAGGAIN_5_6                     = HMC5883_REG_MAG_CRB_GN_5_6,
    MAGGAIN_8_1                     = HMC5883_REG_MAG_CRB_GN_8_1
} hmcMagGain_t;

typedef enum
{
    CONTINOUS_MEASUREMENT           = HMC5883_REG_MAG_MR_MD_CONT,
    SINGLE_MEASUREMENT              = HMC5883_REG_MAG_MR_MD_SINGLE,
    IDLE_MEASUREMENT                = HMC5883_REG_MAG_MR_MD_IDLE
    
} hmcOpMode_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
} hmcMagData_t;


class HMC5883
{
    protected:
        // Instance-specific properties
        uint8_t hmc_ConversionDelay;
        hmcMASample_t hmc_masample;
        hmcDataRate_t hmc_datarate;
        hmcMeasMode_t hmc_measmode;
        hmcMagGain_t hmc_maggain;
        hmcOpMode_t hmc_opmode;
    
    public:
        uint8_t hmc_i2cAddress;
        hmcMagData_t hmc_magdata;
        void getAddr_HMC5883(uint8_t i2cAddress);
        bool begin(void);
        void setUpMagnetometer(void);
        void Measure_Magnetometer(void);
        void setMASample(hmcMASample_t sample);
        hmcMASample_t getMASample(void);
        void setDataRate(hmcDataRate_t datarate);
        hmcDataRate_t getDataRate(void);
        void setMeasMode(hmcMeasMode_t measmode);
        hmcMeasMode_t getMeasMode(void);
        void setMagGain(hmcMagGain_t maggain);
        hmcMagGain_t getMagGain(void);
        void setOpMode(hmcOpMode_t opmode);
        hmcOpMode_t getOpMode(void);
    
    private:
};
