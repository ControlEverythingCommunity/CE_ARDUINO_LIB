/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        VEML6070
        This code is designed to work with the VEML6070_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
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
    #define VEML6070_ADDRESS_LOW            (0x38)    // 0111000x
    #define VEML6070_ADDRESS_HIGH           (0x39)    // 0111001x

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define VEML6070_CONVERSIONDELAY        (100)

/**************************************************************************
    COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define VEML6070_CMD_ACK_MASK           (0x20)  // Acknowledge Activity Setting
    #define VEML6070_CMD_ACK_DISABLE        (0x00)  // Disable
    #define VEML6070_CMD_ACK_ENABLE         (0x20)  // Enable

    #define VEML6070_CMD_ACK_THD_MASK       (0x10)  // Acknowledge Threshold wWindow sSetting for Byte Mode Usage
    #define VEML6070_CMD_ACK_THD_102        (0x00)  // 102 Steps
    #define VEML6070_CMD_ACK_THD_145        (0x10)  // 145 Steps

    #define VEML6070_CMD_IT_MASK            (0x0C)  // Integration Time Setting
    #define VEML6070_CMD_IT_1_2T            (0x00)  // (0 : 0) = 1/2T
    #define VEML6070_CMD_IT_1T              (0x04)  // (0 : 1) = 1T
    #define VEML6070_CMD_IT_2T              (0x08)  // (1 : 0) = 2T
    #define VEML6070_CMD_IT_4T              (0x0C)  // (1 : 1) = 4T

    #define VEML6070_CMD_SD_MASK            (0x01)  // Shutdown Mode Setting
    #define VEML6070_CMD_SD_DISABLE         (0x00)  // Disable
    #define VEML6070_CMD_SD_ENABLE          (0x01)  // Enable

typedef enum
{
    VEML6070_HALF_T     = VEML6070_CMD_IT_1_2T,
    VEML6070_1_T        = VEML6070_CMD_IT_1T,
    VEML6070_2_T        = VEML6070_CMD_IT_2T,
    VEML6070_4_T        = VEML6070_CMD_IT_4T
    
} veml_integrationtime_t;

class VEML6070
{
    protected:
        // Instance-specific properties
        uint8_t veml_conversionDelay;
    
    public:
        uint8_t veml_i2cAddress;
        VEML6070() {};
        void getAddr_VEML6070(uint8_t i2cAddress);
        void begin(veml_integrationtime_t itime);
        uint16_t ReadUV(void);
  
    private:
};
