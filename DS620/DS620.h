/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        DS620
        This code is designed to work with the DS620_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=DS620_I2CS#tabs-0-product_tabset-2
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
                                                            //         A2  A1  A0
    #define DS620_DEFAULT_ADDRESS               (0x48)      // 1001    0    0   0
    #define DS620_ADDRESS_A2A1A0_001            (0x49)      // 1001    0    0   1
    #define DS620_ADDRESS_A2A1A0_010            (0x4A)      // 1001    0    1   0
    #define DS620_ADDRESS_A2A1A0_011            (0x4B)      // 1001    0    1   1
    #define DS620_ADDRESS_A2A1A0_100            (0x4C)      // 1001    1    0   0
    #define DS620_ADDRESS_A2A1A0_101            (0x4D)      // 1001    1    0   1
    #define DS620_ADDRESS_A2A1A0_110            (0x4E)      // 1001    1    1   0
    #define DS620_ADDRESS_A2A1A0_111            (0x4F)      // 1001    1    1   1


/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define DS620_CONVERSIONDELAY               (100)

/**************************************************************************
    POINTER REGISTER
**************************************************************************/
    #define DS620_REG_TH_MSB                    (0xA0)
    #define DS620_REG_TH_LSB                    (0xA1)
    #define DS620_REG_TL_MSB                    (0xA2)
    #define DS620_REG_TL_LSB                    (0xA3)
    #define DS620_REG_TEMP_MSB                  (0xAA)
    #define DS620_REG_TEMP_LSB                  (0xAB)
    #define DS620_REG_CONFIG_MSB                (0xAC)
    #define DS620_REG_CONFIG_LSB                (0xAD)

/**************************************************************************
    CONFIGURATION REGISTER
**************************************************************************/
    #define DS620_REG_CONFIG_DONE_MASK          (0x80)    // At Power-Up, DONE = 1. (Unless AUTOC = 1)
    #define DS620_REG_CONFIG_DONE_0             (0x00)    // Temperature Conversion is in Progress
    #define DS620_REG_CONFIG_DONE_1             (0x80)    // Temperature Conversion is Complete

    #define DS620_REG_CONFIG_NVB_MASK           (0x40)    // At Power-Up, NVB = 1
    #define DS620_REG_CONFIG_NVB_0              (0x00)    // Write to an E2 Memory Cell is in Progress
    #define DS620_REG_CONFIG_NVB_1              (0x40)    // Nonvolatile Memory is not Busy

    #define DS620_REG_CONFIG_THF_MASK           (0x20)    // At Power-Up, THF = 1
    #define DS620_REG_CONFIG_THF_0              (0x00)    // The Measured Temperature has Reached or Exceeded the Value Stored in the TH Register at Anytime Since Power-Up or Since the Bit was Last Cleared
    #define DS620_REG_CONFIG_THF_1              (0x20)    // THF Remains a 1 until the Power is Cycled or it is Overwritten with a 0 by the User

    #define DS620_REG_CONFIG_TLF_MASK           (0x10)    // At Power-Up, TLF = 1
    #define DS620_REG_CONFIG_TLF_0              (0x00)    // The Measured Temperature has Met or Fallen the Value Stored in the TL Register at Anytime Since Power-Up or Since the Bit was Last Cleared
    #define DS620_REG_CONFIG_TLF_1              (0x10)    // TLF Remains a 1 until the Power is Cycled or it is Overwritten with a 0 by the User

    #define DS620_REG_CONFIG_RES_MASK           (0x0C)    // Set Conversion Resolution
    #define DS620_REG_CONFIG_RES_10BIT          (0x00)    // Resolution: 10-bit
    #define DS620_REG_CONFIG_RES_11BIT          (0x04)    // Resolution: 11-bit
    #define DS620_REG_CONFIG_RES_12BIT          (0x08)    // Resolution: 12-bit
    #define DS620_REG_CONFIG_RES_13BIT          (0x0C)    // Resolution: 13-bit

    #define DS620_REG_CONFIG_AUTOC_MASK         (0x02)    // Determines Whether the DS620 Powers up Idle or Converting
    #define DS620_REG_CONFIG_AUTOC_0            (0x00)    // DS620 Powers-up Idle
    #define DS620_REG_CONFIG_AUTOC_1            (0x02)    // DS620 Powers-up Converting Temperature

    #define DS620_REG_CONFIG_1SHOT_MASK         (0x01)    // Configures Temperature Conversion Mode
    #define DS620_REG_CONFIG_1SHOT_0            (0x00)    // Continuous Conversion Mode
    #define DS620_REG_CONFIG_1SHOT_1            (0x01)    // One-Shot Mode


    #define DS620_REG_CONFIG_RPO_MASK           (0xC0)    // PO Configuration
    #define DS620_REG_CONFIG_RPO_00             (0x00)    // Force PO Low
    #define DS620_REG_CONFIG_RPO_01             (0x40)    // Force PO Low
    #define DS620_REG_CONFIG_RPO_10             (0x80)    // Thermostat Output (PO-Low)
    #define DS620_REG_CONFIG_RPO_11             (0xC0)    // Thermostat Output (PO-High)


typedef enum
{
    RESOLUTION_10BIT         = DS620_REG_CONFIG_RES_10BIT,
    RESOLUTION_11BIT         = DS620_REG_CONFIG_RES_11BIT,
    RESOLUTION_12BIT         = DS620_REG_CONFIG_RES_12BIT,
    RESOLUTION_13BIT         = DS620_REG_CONFIG_RES_13BIT
    
} dsResolution_t;

class DS620
{
    protected:
        // Instance-specific properties
        uint8_t ds_conversionDelay;
        dsResolution_t ds_resolution;

    public:
        uint8_t ds_i2cAddress;
        void getAddr_DS620(uint8_t i2cAddress);
        void begin(void);
        int16_t Measure_Temp(void);
        void setResolution(dsResolution_t resolution);
        dsResolution_t getResolution(void);
    
    private:
};
