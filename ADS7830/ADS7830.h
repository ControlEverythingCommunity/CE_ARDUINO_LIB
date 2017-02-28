/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADS7830
        This code is designed to work with the ADS7830_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADS7830_I2CADC#tabs-0-product_tabset-2
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
    #define ADS7830_DEFAULT_ADDRESS             (0x48)      // 1001 000 (ADDR = GND)
    #define ADS7830_VDD_ADDRESS                 (0x49)      // 1001 001 (ADDR = VDD)
    #define ADS7830_SDA_ADDRESS                 (0x4A)      // 1001 010 (ADDR = SDA)
    #define ADS7830_SCL_ADDRESS                 (0x4B)      // 1001 011 (ADDR = SCL)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define ADS7830_CONVERSIONDELAY             (100)

/**************************************************************************
    COMMAND BYTE REGISTER
**************************************************************************/
    #define ADS7830_REG_COMMAND_SD_MASK         (0x80)      // Single-Ended/Differential Inputs
    #define ADS7830_REG_COMMAND_SD_DIFF         (0x00)      // Bit = 0, Differential Inputs
    #define ADS7830_REG_COMMAND_SD_SINGLE       (0x80)      // Bit = 1, Single-Ended Inputs

    #define ADS7830_REG_COMMAND_CH_MASK         (0x70)      // Input multiplexer COMMANDuration
    #define ADS7830_REG_COMMAND_CH_DIFF_0_1     (0x00)      // Differential P = CH0, N = CH1
    #define ADS7830_REG_COMMAND_CH_DIFF_2_3     (0x10)      // Differential P = CH2, N = CH3
    #define ADS7830_REG_COMMAND_CH_DIFF_4_5     (0x20)      // Differential P = CH4, N = CH5
    #define ADS7830_REG_COMMAND_CH_DIFF_6_7     (0x30)      // Differential P = CH6, N = CH7
    #define ADS7830_REG_COMMAND_CH_DIFF_1_0     (0x40)      // Differential P = CH1, N = CH0
    #define ADS7830_REG_COMMAND_CH_DIFF_3_2     (0x50)      // Differential P = CH3, N = CH2
    #define ADS7830_REG_COMMAND_CH_DIFF_5_4     (0x60)      // Differential P = CH5, N = CH4
    #define ADS7830_REG_COMMAND_CH_DIFF_7_6     (0x70)      // Differential P = CH7, N = CH6
    #define ADS7830_REG_COMMAND_CH_SINGLE_0     (0x00)      // Single-ended P = CH0, N = COM
    #define ADS7830_REG_COMMAND_CH_SINGLE_1     (0x10)      // Single-ended P = CH1, N = COM
    #define ADS7830_REG_COMMAND_CH_SINGLE_2     (0x20)      // Single-ended P = CH2, N = COM
    #define ADS7830_REG_COMMAND_CH_SINGLE_3     (0x30)      // Single-ended P = CH3, N = COM
    #define ADS7830_REG_COMMAND_CH_SINGLE_4     (0x40)      // Single-ended P = CH4, N = COM
    #define ADS7830_REG_COMMAND_CH_SINGLE_5     (0x50)      // Single-ended P = CH5, N = COM
    #define ADS7830_REG_COMMAND_CH_SINGLE_6     (0x60)      // Single-ended P = CH6, N = COM
    #define ADS7830_REG_COMMAND_CH_SINGLE_7     (0x70)      // Single-ended P = CH7, N = COM

    #define ADS7830_REG_COMMAND_PD_MASK         (0x0C)      // Power-Down Selection
    #define ADS7830_REG_COMMAND_PD_PDADCONV     (0x00)      // Power Down Between A/D Converter Conversions
    #define ADS7830_REG_COMMAND_PD_IROFF_ADON   (0x04)      // Internal Reference OFF and A/D Converter ON
    #define ADS7830_REG_COMMAND_PD_IRON_ADOFF   (0x08)      // Internal Reference ON and A/D Converter OFF
    #define ADS7830_REG_COMMAND_PD_IRON_ADON    (0x0C)      // Internal Reference ON and A/D Converter ON



typedef enum
{
    SDMODE_DIFF         = ADS7830_REG_COMMAND_SD_DIFF,
    SDMODE_SINGLE       = ADS7830_REG_COMMAND_SD_SINGLE
} adsSDMode_t;

typedef enum
{
    PDADCONV            = ADS7830_REG_COMMAND_PD_PDADCONV,
    PDIROFF_ADON        = ADS7830_REG_COMMAND_PD_IROFF_ADON,
    PDIRON_ADOFF        = ADS7830_REG_COMMAND_PD_IRON_ADOFF,
    PDIRON_ADON         = ADS7830_REG_COMMAND_PD_IRON_ADON
} adsPDMode_t;

class ADS7830
{
    protected:
        // Instance-specific properties
        uint8_t ads_conversionDelay;
        adsSDMode_t ads_sdmode;
        adsPDMode_t ads_pdmode;


    public:
        uint8_t ads_i2cAddress;
        void getAddr_ADS7830(uint8_t i2cAddress);
        void begin(void);
        uint8_t Measure_SingleEnded(uint8_t channel);
        int8_t Measure_Differential(uint8_t channel);
        int8_t getLastConversionResults();
        void setSDMode(adsSDMode_t sdmode);
        adsSDMode_t getSDMode(void);
        void setPDMode(adsPDMode_t pdmode);
        adsPDMode_t getPDMode(void);

    private:
};
