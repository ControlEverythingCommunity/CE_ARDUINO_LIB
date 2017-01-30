/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AD7416ARZ
        This code is designed to work with the AD7416ARZ_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=AD7416ARZ_I2CS#tabs-0-product_tabset-2
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
    #define AD7416ARZ_DEFAULT_ADDRESS           (0x48)      // 1001    0    0   0
    #define AD7416ARZ_ADDRESS_A2A1A0_001        (0x49)      // 1001    0    0   1
    #define AD7416ARZ_ADDRESS_A2A1A0_010        (0x4A)      // 1001    0    1   0
    #define AD7416ARZ_ADDRESS_A2A1A0_011        (0x4B)      // 1001    0    1   1
    #define AD7416ARZ_ADDRESS_A2A1A0_100        (0x4C)      // 1001    1    0   0
    #define AD7416ARZ_ADDRESS_A2A1A0_101        (0x4D)      // 1001    1    0   1
    #define AD7416ARZ_ADDRESS_A2A1A0_110        (0x4E)      // 1001    1    1   0
    #define AD7416ARZ_ADDRESS_A2A1A0_111        (0x4F)      // 1001    1    1   1


/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define AD7416ARZ_CONVERSIONDELAY           (100)

/**************************************************************************
    POINTER REGISTER
**************************************************************************/
    #define AD7416ARZ_REG_POINTER_MASK          (0x05)
    #define AD7416ARZ_REG_POINTER_TEMP          (0x00)
    #define AD7416ARZ_REG_POINTER_CONFIG        (0x01)
    #define AD7416ARZ_REG_POINTER_THYST         (0x02)
    #define AD7416ARZ_REG_POINTER_TOTI          (0x03)
    #define AD7416ARZ_REG_POINTER_TADC          (0x04)
    #define AD7416ARZ_REG_POINTER_CONFIG2       (0x05)

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/
    #define AD7416ARZ_REG_CONFIG_CS_MASK        (0xE0)    // Channel Selection
    #define AD7416ARZ_REG_CONFIG_CS_CHANNEL0    (0x00)    // Temperature sensor (all parts), Channel 0
    #define AD7416ARZ_REG_CONFIG_CS_CHANNEL1    (0x20)    // AIN1, Channel 1
    #define AD7416ARZ_REG_CONFIG_CS_CHANNEL2    (0x40)    // AIN2, Channel 2
    #define AD7416ARZ_REG_CONFIG_CS_CHANNEL3    (0x60)    // AIN3, Channel 3
    #define AD7416ARZ_REG_CONFIG_CS_CHANNEL4    (0x80)    // AIN4, Channel 4

    #define AD7416ARZ_REG_CONFIG_FAULTQUE_MASK  (0x18)    // Fault Queue Settings
    #define AD7416ARZ_REG_CONFIG_FAULTQUE_1     (0x00)    // 1  Fault
    #define AD7416ARZ_REG_CONFIG_FAULTQUE_2     (0x08)    // 2  Faults
    #define AD7416ARZ_REG_CONFIG_FAULTQUE_4     (0x10)    // 4  Faults
    #define AD7416ARZ_REG_CONFIG_FAULTQUE_6     (0x18)    // 6  Faults

    #define AD7416ARZ_REG_CONFIG_OTI_MASK       (0x04)    // OTI Output
    #define AD7416ARZ_REG_CONFIG_OTI_LOW        (0x00)    // Active-Low
    #define AD7416ARZ_REG_CONFIG_OTI_HIGH       (0x04)    // Active-High

    #define AD7416ARZ_REG_CONFIG_MOD_MASK       (0x02)    // Mode of Operation
    #define AD7416ARZ_REG_CONFIG_MOD_COMP       (0x00)    // Comparator Output
    #define AD7416ARZ_REG_CONFIG_MOD_INTR       (0x02)    // Interrupt Output

    #define AD7416ARZ_REG_CONFIG_SHDN_MASK      (0x01)    // Shutdown Bit
    #define AD7416ARZ_REG_CONFIG_SHDN_DISABLE   (0x00)    // Disable
    #define AD7416ARZ_REG_CONFIG_SHDN_ENABLE    (0x01)    // Enable


typedef enum
{
    CHANNEL_0           = AD7416ARZ_REG_CONFIG_CS_CHANNEL0,
    CHANNEL_1           = AD7416ARZ_REG_CONFIG_CS_CHANNEL1,
    CHANNEL_2           = AD7416ARZ_REG_CONFIG_CS_CHANNEL2,
    CHANNEL_3           = AD7416ARZ_REG_CONFIG_CS_CHANNEL3,
    CHANNEL_4           = AD7416ARZ_REG_CONFIG_CS_CHANNEL4
    
} ad7416arzChannel_t;

typedef enum
{
    FAULTQUE_1          = AD7416ARZ_REG_CONFIG_FAULTQUE_1,
    FAULTQUE_2          = AD7416ARZ_REG_CONFIG_FAULTQUE_2,
    FAULTQUE_4          = AD7416ARZ_REG_CONFIG_FAULTQUE_4,
    FAULTQUE_6          = AD7416ARZ_REG_CONFIG_FAULTQUE_6
    
} ad7416arzFaultQue_t;

typedef enum
{
    OTI_LOW             = AD7416ARZ_REG_CONFIG_OTI_LOW,
    OTI_HIGH            = AD7416ARZ_REG_CONFIG_OTI_HIGH
    
} ad7416arzOTI_t;

typedef enum
{
    MOD_COMP            = AD7416ARZ_REG_CONFIG_MOD_COMP,
    MOD_INTR            = AD7416ARZ_REG_CONFIG_MOD_INTR
    
} ad7416arzMod_t;

typedef enum
{
    SHUTDOWN_DISABLE    = AD7416ARZ_REG_CONFIG_SHDN_DISABLE,
    SHUTDOWN_ENABLE     = AD7416ARZ_REG_CONFIG_SHDN_ENABLE
    
} ad7416arzShutdown_t;


class AD7416ARZ
{
    protected:
        // Instance-specific properties
        uint8_t ad7416arz_conversionDelay;
        ad7416arzChannel_t ad7416arz_channel;
        ad7416arzFaultQue_t ad7416arz_faultqueue;
        ad7416arzOTI_t ad7416arz_OTI;
        ad7416arzMod_t ad7416arz_mod;
        ad7416arzShutdown_t ad7416arz_shutdown;
    
    public:
        uint8_t ad7416arz_i2cAddress;
        void getAddr_AD7416ARZ(uint8_t i2cAddress);
        void begin(void);
        int16_t Measure_Temp(void);
        void setChannel(ad7416arzChannel_t channel);
        ad7416arzChannel_t getChannel(void);
        void setFaultQue(ad7416arzFaultQue_t faultque);
        ad7416arzFaultQue_t getFaultQue(void);
        void setOTI(ad7416arzOTI_t oti);
        ad7416arzOTI_t getOTI(void);
        void setMod(ad7416arzMod_t mod);
        ad7416arzMod_t getMod(void);
        void setShutdown(ad7416arzShutdown_t shutdown);
        ad7416arzShutdown_t getShutdown(void);
    
    private:
};
