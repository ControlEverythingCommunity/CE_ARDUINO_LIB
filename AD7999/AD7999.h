/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AD7999
        This code is designed to work with the AD7999_I2CADC I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/ad7999-4-channel-8-10-12-bit-analog-to-digital-converter
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
    #define AD7999_DEFAULT_ADDRESS                  (0x29)      // 010 1001

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define AD7999_CONVERSIONDELAY                  (100)       // define the time interal(in ms) between each conversion

/**************************************************************************
    CONFIGURATION REGISTER
**************************************************************************/
    #define AD7999_REG_CONFIG_CYCLE_CH_MASK         (0xF0)      // Select the Analog Input Channel(s) to be Converted
    #define AD7999_REG_CONFIG_CYCLE_CH_NO           (0x00)      // No channel Selected
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN0         (0x10)      // Convert on VIN0
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN1         (0x20)      // Convert on VIN1
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN0_1       (0x30)      // Sequence between VIN0 and VIN1
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN2         (0x40)      // Convert on VIN2
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN0_2       (0x50)      // Sequence between VIN0 and VIN2
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN1_2       (0x60)      // Sequence between VIN1 and VIN2
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN0_1_2     (0x70)      // Sequence among VIN0, VIN1, and VIN2
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN3         (0x80)      // Convert on VIN3
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN0_3       (0x90)      // Sequence between VIN0 and VIN3
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN1_3       (0xA0)      // Sequence between VIN1 and VIN3
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN0_1_3     (0xB0)      // Sequence among VIN0, VIN1, and VIN3
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN2_3       (0xC0)      // Sequence between VIN2 and VIN3
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN0_2_3     (0xD0)      // Sequence among VIN0, VIN2, and VIN3
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN1_2_3     (0xE0)      // Sequence among VIN1, VIN2, and VIN3
    #define AD7999_REG_CONFIG_CYCLE_CH_VIN0_1_2_3   (0xF0)      // Sequence among VIN0, VIN1, VIN2, and VIN3

    #define AD7999_REG_CONFIG_REF_SEL_MASK          (0x08)      // Select the Supply Voltage as the Reference or Choose to use an External Reference
    #define AD7999_REG_CONFIG_REF_SEL_SUPPLY        (0x00)      // The supply is Used as the Reference, and the Device acts as a 4-Channel Input Part
    #define AD7999_REG_CONFIG_REF_SEL_EXT_REF       (0x08)      // An External Reference must be Used and Applied to the VIN3/VREF Pin, and the Device acts as a 3-Channel Input Part

    #define AD7999_REG_CONFIG_FLTR_MASK             (0x04)      // Determines Whether the Filtering on SDA and SCL is Enabled or Bypassed
    #define AD7999_REG_CONFIG_FLTR_ENABLED          (0x00)      // Filtering on SDA and SCL is Enabled
    #define AD7999_REG_CONFIG_FLTR_BYPASSED         (0x04)      // Filtering on SDA and SCL is Bypassed

    #define AD7999_REG_CONFIG_BTD_MASK              (0x02)      // Bit Trial Delay
    #define AD7999_REG_CONFIG_BTD_ON                (0x00)      // Bit Trial Delay Mechanism is Implemented
    #define AD7999_REG_CONFIG_BTD_OFF               (0x02)      // Bit Trial Delay Mechanism is not Implemented

    #define AD7999_REG_CONFIG_SD_MASK               (0x01)      // Sample Delay
    #define AD7999_REG_CONFIG_SD_ON                 (0x00)      // Sample Delay Mechanism is Implemented
    #define AD7999_REG_CONFIG_SD_OFF                (0x01)      // Sample Delay Mechanism is not Implemented


typedef enum
{
    NO_CHANNEL                  = AD7999_REG_CONFIG_CYCLE_CH_NO,
    CHANNEL_VIN0                = AD7999_REG_CONFIG_CYCLE_CH_VIN0,
    CHANNEL_VIN1                = AD7999_REG_CONFIG_CYCLE_CH_VIN1,
    CHANNEL_VIN0_1              = AD7999_REG_CONFIG_CYCLE_CH_VIN1,
    CHANNEL_VIN2                = AD7999_REG_CONFIG_CYCLE_CH_VIN0_1,
    CHANNEL_VIN0_2              = AD7999_REG_CONFIG_CYCLE_CH_VIN2,
    CHANNEL_VIN1_2              = AD7999_REG_CONFIG_CYCLE_CH_VIN0_2,
    CHANNEL_VIN0_1_2            = AD7999_REG_CONFIG_CYCLE_CH_VIN1_2,
    CHANNEL_VIN3                = AD7999_REG_CONFIG_CYCLE_CH_VIN3,
    CHANNEL_VIN0_3              = AD7999_REG_CONFIG_CYCLE_CH_VIN0_3,
    CHANNEL_VIN1_3              = AD7999_REG_CONFIG_CYCLE_CH_VIN1_3,
    CHANNEL_VIN0_1_3            = AD7999_REG_CONFIG_CYCLE_CH_VIN0_1_3,
    CHANNEL_VIN2_3              = AD7999_REG_CONFIG_CYCLE_CH_VIN2_3,
    CHANNEL_VIN0_2_3            = AD7999_REG_CONFIG_CYCLE_CH_VIN0_2_3,
    CHANNEL_VIN1_2_3            = AD7999_REG_CONFIG_CYCLE_CH_VIN1_2_3,
    CHANNEL_VIN0_1_2_3          = AD7999_REG_CONFIG_CYCLE_CH_VIN0_1_2_3
    
}adChannel_t;

typedef enum
{
    SELECT_SUPPLY               = AD7999_REG_CONFIG_REF_SEL_SUPPLY,
    EXTERNAL_REFERENCE          = AD7999_REG_CONFIG_REF_SEL_EXT_REF
    
}adSupply_t;

typedef enum
{
    FILTERING_ENABLED           = AD7999_REG_CONFIG_FLTR_ENABLED,
    FILTERING_BYPASSED          = AD7999_REG_CONFIG_FLTR_BYPASSED
    
}adFiltering_t;

typedef enum
{
    BIT_TRIAL_DELAY_ON          = AD7999_REG_CONFIG_BTD_ON,
    BIT_TRIAL_DELAY_OFF         = AD7999_REG_CONFIG_BTD_OFF
    
}adBitTrialDelay_t;

typedef enum
{
    SAMPLE_DELAY_ON             = AD7999_REG_CONFIG_SD_ON,
    SAMPLE_DELAY_OFF            = AD7999_REG_CONFIG_SD_OFF
    
}adSampleDelay_t;


class AD7999
{
    protected:
        // Instance-specific properties
        uint8_t ad_conversionDelay;
        adChannel_t ad_channel;
        adSupply_t ad_supply;
        adFiltering_t ad_filtering;
        adBitTrialDelay_t ad_bittrialdelay;
        adSampleDelay_t ad_sampledelay;
    
    public:
        uint8_t ad_i2cAddress;
        void getAddr_AD7999(uint8_t i2cAddress);
        bool begin(void);
        uint8_t Measure_Voltage(void);
        void setChannel(adChannel_t channel);
        adChannel_t getChannel(void);
        void setSupply(adSupply_t supply);
        adSupply_t getSupply(void);
        void setFiltering(adFiltering_t filtering);
        adFiltering_t getFiltering(void);
        void setBitTrialDelay(adBitTrialDelay_t bittrialdelay);
        adBitTrialDelay_t getBitTrialDelay(void);
        void setSampleDelay(adSampleDelay_t sampledelay);
        adSampleDelay_t getSampleDelay(void);


    private:
};
