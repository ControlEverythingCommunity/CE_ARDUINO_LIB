/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AT30TS74
        This code is designed to work with the AT30TS74_I2CS I2C Mini Module available from ControlEverything.com.
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
                                                            //         A2  A1  A0
    #define AT30TS74_DEFAULT_ADDRESS            (0x48)      // 1001    0    0   0
    #define AT30TS74_ADDRESS_A2A1A0_001         (0x49)      // 1001    0    0   1
    #define AT30TS74_ADDRESS_A2A1A0_010         (0x4A)      // 1001    0    1   0
    #define AT30TS74_ADDRESS_A2A1A0_011         (0x4B)      // 1001    0    1   1
    #define AT30TS74_ADDRESS_A2A1A0_100         (0x4C)      // 1001    1    0   0
    #define AT30TS74_ADDRESS_A2A1A0_101         (0x4D)      // 1001    1    0   1
    #define AT30TS74_ADDRESS_A2A1A0_110         (0x4E)      // 1001    1    1   0
    #define AT30TS74_ADDRESS_A2A1A0_111         (0x4F)      // 1001    1    1   1

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define AT30TS74_CONVERSIONDELAY            (100)

/**************************************************************************
    POINTER REGISTER
**************************************************************************/
    #define AT30TS74_REG_POINTER_MASK           (0x03)
    #define AT30TS74_REG_POINTER_TEMP           (0x00)
    #define AT30TS74_REG_POINTER_CONFIG         (0x01)
    #define AT30TS74_REG_POINTER_TLOW           (0x02)
    #define AT30TS74_REG_POINTER_THIGH          (0x03)

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/

    #define AT30TS74_REG_CONFIG_OS_MASK         (0x8000)    // One-Shot Mode
    #define AT30TS74_REG_CONFIG_OS_ENABLED      (0x0000)    // Normal Operation (Default)
    #define AT30TS74_REG_CONFIG_OS_DISABLED     (0x8000)    // Perform One-Shot Measurement

    #define AT30TS74_REG_CONFIG_RES_MASK        (0x6000)    // Conversion Resolution
    #define AT30TS74_REG_CONFIG_RES_9BIT        (0x0000)    // 9 bits (Default)
    #define AT30TS74_REG_CONFIG_RES_10BIT       (0x2000)    // 10 bits
    #define AT30TS74_REG_CONFIG_RES_11BIT       (0x4000)    // 11 bits
    #define AT30TS74_REG_CONFIG_RES_12BIT       (0x6000)    // 12 bits

    #define AT30TS74_REG_CONFIG_FAULTQUE_MASK   (0x1800)    // Fault Tolerance Queue
    #define AT30TS74_REG_CONFIG_FAULTQUE_1      (0x0000)    // Alarm after 1 Fault (Default)
    #define AT30TS74_REG_CONFIG_FAULTQUE_2      (0x0800)    // Alarm after 2 Consecutive Faults
    #define AT30TS74_REG_CONFIG_FAULTQUE_4      (0x1000)    // Alarm after 4 Consecutive Faults
    #define AT30TS74_REG_CONFIG_FAULTQUE_6      (0x1800)    // Alarm after 6 Consecutive Faults

    #define AT30TS74_REG_CONFIG_ALERTPOL_MASK   (0x0400)    // ALERT Pin Polarity
    #define AT30TS74_REG_CONFIG_ALERTPOL_HIGH   (0x0000)    // ALERT pin is Active Low (Default)
    #define AT30TS74_REG_CONFIG_ALERTPOL_LOW    (0x0400)    // ALERT pin is Active High

    #define AT30TS74_REG_CONFIG_CMPINT_MASK     (0x0200)    // Alarm Thermostat Mode
    #define AT30TS74_REG_CONFIG_CMPINT_INTR     (0x0000)    // Comparator Mode (Default)
    #define AT30TS74_REG_CONFIG_CMPINT_COMP     (0x0200)    // Interrupt Mode

    #define AT30TS74_REG_CONFIG_SD_MASK         (0x0100)    // Shutdown Mode
    #define AT30TS74_REG_CONFIG_SD_ENABLE       (0x0000)    // Temperature Sensor Performing Active Measurements (Default)
    #define AT30TS74_REG_CONFIG_SD_DISABLE      (0x0100)    // Temperature Sensor Disabled and Device in Shutdown Mode


typedef enum
{
    ONESHOT_ENABLED     = AT30TS74_REG_CONFIG_OS_ENABLED,
    ONESHOT_DISABLED    = AT30TS74_REG_CONFIG_OS_DISABLED
    
} atOneShot_t;

typedef enum
{
    RESOLUTION_9        = AT30TS74_REG_CONFIG_RES_9BIT,
    RESOLUTION_10       = AT30TS74_REG_CONFIG_RES_10BIT,
    RESOLUTION_11       = AT30TS74_REG_CONFIG_RES_11BIT,
    RESOLUTION_12       = AT30TS74_REG_CONFIG_RES_12BIT
    
} atResolution_t;

typedef enum
{
    FAULTQUE_1          = AT30TS74_REG_CONFIG_FAULTQUE_1,
    FAULTQUE_2          = AT30TS74_REG_CONFIG_FAULTQUE_2,
    FAULTQUE_4          = AT30TS74_REG_CONFIG_FAULTQUE_4,
    FAULTQUE_6          = AT30TS74_REG_CONFIG_FAULTQUE_6
    
} atFaultQue_t;

typedef enum
{
    ALERTPOL_LOW        = AT30TS74_REG_CONFIG_ALERTPOL_LOW,
    ALERTPOL_HIGH       = AT30TS74_REG_CONFIG_ALERTPOL_HIGH
    
} atAlertPol_t;

typedef enum
{
    MOD_COMP            = AT30TS74_REG_CONFIG_CMPINT_COMP,
    MOD_INTR            = AT30TS74_REG_CONFIG_CMPINT_INTR
    
} atMod_t;

typedef enum
{
    SHUTDOWN_ENABLE     = AT30TS74_REG_CONFIG_SD_ENABLE,
    SHUTDOWN_DISABLE    = AT30TS74_REG_CONFIG_SD_DISABLE

} atShutdown_t;


class AT30TS74
{
    protected:
        // Instance-specific properties
        uint8_t at_conversionDelay;
        atOneShot_t at_oneshot;
        atResolution_t at_resolution;
        atFaultQue_t at_faultqueue;
        atAlertPol_t at_alertpol;
        atMod_t at_mod;
        atShutdown_t at_shutdown;
    
    public:
        uint8_t at_i2cAddress;
        void getAddr_AT30TS74(uint8_t i2cAddress);
        void begin(void);
        int16_t Measure_Temp(void);
        void setOneShot(atOneShot_t oneshot);
        atOneShot_t getOneShot(void);
        void setResolution(atResolution_t resolution);
        atResolution_t getResolution(void);
        void setFaultQue(atFaultQue_t faultque);
        atFaultQue_t getFaultQue(void);
        void setAlertPol(atAlertPol_t alertpol);
        atAlertPol_t getAlertPol(void);
        void setMod(atMod_t mod);
        atMod_t getMod(void);
        void setShutdown(atShutdown_t shutdown);
        atShutdown_t getShutdown(void);
    
    private:
};
