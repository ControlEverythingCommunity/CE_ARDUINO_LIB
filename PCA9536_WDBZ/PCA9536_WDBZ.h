/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        PCA9536_WDBZ
        This code is designed to work with the PCA9536_WDBZ_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/water-detect-sensor-with-buzzer
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
    #define PCA9536_WDBZ_DEFAULT_ADDRESS                         (0x41)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define PCA9536_WDBZ_CONVERSIONDELAY                         (100)

/**************************************************************************
    DIGITAL COLOR, PROXIMITY MODULE REGISTERS
**************************************************************************/
    #define PCA9536_WDBZ_NUM_BYTES                               (0x01)      // No of Bytes
    #define PCA9536_WDBZ_ALL_INPUT                               (0xFF)      // All Pins are Inputs
    #define PCA9536_WDBZ_ALL_OUTPUT                              (0x00)      // All Pins are Outputs
    #define PCA9536_WDBZ_ALL_LOW                                 (0x00)      // All Pins are Low
    #define PCA9536_WDBZ_ALL_HIGH                                (0xFF)      // All Pins are High
    #define PCA9536_WDBZ_ALL_NON_INVERTED                        (0x00)      // All Pins are Non-Inverted
    #define PCA9536_WDBZ_ALL_INVERTED                            (0xFF)      // All Pins are Inverted

typedef enum
{
    PCA9536_WDBZ_REG_INPUT           = 0,
    PCA9536_WDBZ_REG_OUTPUT          = 1,
    PCA9536_WDBZ_REG_POLARITY        = 2,
    PCA9536_WDBZ_REG_CONFIG          = 3
    
} pcaCommand_t;

typedef enum
{
    PCA9536_WDBZ_OUTPUT_PIN0         = 0,
    PCA9536_WDBZ_OUTPUT_PIN1         = 1,
    PCA9536_WDBZ_OUTPUT_PIN2         = 2,
    PCA9536_WDBZ_OUTPUT_PIN3         = 3
    
} pcaPin_t;

typedef enum
{
    PCA9536_WDBZ_MODE_OUTPUT         = 0,
    PCA9536_WDBZ_MODE_INPUT          = 1
    
} pcaMode_t;

typedef enum
{
    PCA9536_WDBZ_STATE_LOW           = 0,
    PCA9536_WDBZ_STATE_HIGH          = 1
    
} pcaState_t;

typedef enum
{
    PCA9536_WDBZ_STATE_NON_INVERTED  = 0,
    PCA9536_WDBZ_STATE_INVERTED      = 1
    
} pcaPolarity_t;


class PCA9536_WDBZ
{
    protected:
        // Instance-specific properties
        uint8_t pca_conversionDelay;

    public:
        PCA9536_WDBZ();
        ~PCA9536_WDBZ();
        uint8_t pca_i2cAddress;
        void getAddr_PCA9536_WDBZ(uint8_t i2cAddress);
        uint8_t begin();
        void Reset();
        uint8_t getPinMode(pcaPin_t pin);
        uint8_t getPinState(pcaPin_t pin);
        uint8_t getPinPolarity(pcaPin_t pin);
        void setPinMode(pcaPin_t pin, pcaMode_t newMode);
        void setPinMode(pcaMode_t newMode);
        void setPinState(pcaPin_t pin, pcaState_t newState);
        void setPinState(pcaState_t newState);
        void setPinPolarity(pcaPin_t pin, pcaPolarity_t newPolarity);
        void setPinPolarity(pcaPolarity_t newPolarity);
        uint8_t getComResult();
    
    private:
        uint8_t _comBuffer;
        uint8_t getCommand(pcaCommand_t command);
        uint8_t getPin(pcaPin_t pin, pcaCommand_t command);
        void setCommand(pcaCommand_t ptr, uint8_t newSetting);
        void setPin(pcaPin_t pin, pcaCommand_t command, uint8_t newSetting);
        void initCall(pcaCommand_t command);
        void EndCall();
};
