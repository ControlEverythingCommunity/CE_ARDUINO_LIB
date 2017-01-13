/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMP101NA
        This code is designed to work with the TMP101NA_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=TMP101NA_I2CS#tabs-0-product_tabset-2
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
//                                                                      ADD0     SLAVE ADDRESS
        #define TMP101NA_0_0_ADDRESS                (0x48)      //      0           1001000
        #define TMP101NA_DEFAULT_ADDRESS            (0x49)      //      Float       1001001
        #define TMP101NA_0_1_ADDRESS                (0x4A)      //      1           1001010

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TMP101NA_CONVERSIONDELAY                (100)

/**************************************************************************
    POINTER REGISTER
**************************************************************************/
    #define TMP101NA_REG_POINTER_MASK               (0x03)
    #define TMP101NA_REG_POINTER_TEMP               (0x00)
    #define TMP101NA_REG_POINTER_CONFIG             (0x01)
    #define TMP101NA_REG_POINTER_TLOW               (0x02)
    #define TMP101NA_REG_POINTER_THIGH              (0x03)

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/

    #define TMP101NA_REG_CONFIG_OS_MASK             (0x80)      // OS/ALERT
    #define TMP101NA_REG_CONFIG_OS_DISABLED         (0x00)      // Disabled
    #define TMP101NA_REG_CONFIG_OS_ENABLED          (0x80)      // Enabled

    #define TMP101NA_REG_CONFIG_RES_MASK            (0x60)      // Converter Resolution
    #define TMP101NA_REG_CONFIG_RES_9BIT            (0x00)      // 9 bits (0.5°C)
    #define TMP101NA_REG_CONFIG_RES_10BIT           (0x20)      // 10 bit (0.25°C)
    #define TMP101NA_REG_CONFIG_RES_11BIT           (0x40)      // 11 bit (0.125°C)
    #define TMP101NA_REG_CONFIG_RES_12BIT           (0x60)      // 12 bit (0.0625°C)

    #define TMP101NA_REG_CONFIG_FAULTQUE_MASK       (0x18)      // Fault Queue
    #define TMP101NA_REG_CONFIG_FAULTQUE_1          (0x00)      // 1 Consecutive Fault
    #define TMP101NA_REG_CONFIG_FAULTQUE_2          (0x08)      // 2 Consecutive Faults
    #define TMP101NA_REG_CONFIG_FAULTQUE_4          (0x10)      // 4 Consecutive Faults
    #define TMP101NA_REG_CONFIG_FAULTQUE_6          (0x18)      // 6 Consecutive Faults

    #define TMP101NA_REG_CONFIG_ALERTPOL_MASK       (0x04)      // Active-high
    #define TMP101NA_REG_CONFIG_ALERTPOL_LOW        (0x00)      // Active-low (Power-up default)
    #define TMP101NA_REG_CONFIG_ALERTPOL_HIGH       (0x04)      // Active-high

    #define TMP101NA_REG_CONFIG_TM_MASK             (0x02)      // Thermostat Mode
    #define TMP101NA_REG_CONFIG_TM_COMP             (0x00)      // Comparator Mode
    #define TMP101NA_REG_CONFIG_TM_INTR             (0x02)      // Interrupt Mode

    #define TMP101NA_REG_CONFIG_SD_MASK             (0x01)      // Shutdown Mode
    #define TMP101NA_REG_CONFIG_SD_DISABLE          (0x00)      // Disable/Continous Conversion
    #define TMP101NA_REG_CONFIG_SD_ENABLE           (0x01)      // Enable Shutdown Mode


typedef enum
{
    ONESHOT_DISABLED    = TMP101NA_REG_CONFIG_OS_DISABLED,
    ONESHOT_ENABLED     = TMP101NA_REG_CONFIG_OS_ENABLED

} tmpOneShot_t;

typedef enum
{
    RESOLUTION_9        = TMP101NA_REG_CONFIG_RES_9BIT,
    RESOLUTION_10       = TMP101NA_REG_CONFIG_RES_10BIT,
    RESOLUTION_11       = TMP101NA_REG_CONFIG_RES_11BIT,
    RESOLUTION_12       = TMP101NA_REG_CONFIG_RES_12BIT
    
} tmpResolution_t;

typedef enum
{
    FAULTQUE_1          = TMP101NA_REG_CONFIG_FAULTQUE_1,
    FAULTQUE_2          = TMP101NA_REG_CONFIG_FAULTQUE_2,
    FAULTQUE_4          = TMP101NA_REG_CONFIG_FAULTQUE_4,
    FAULTQUE_6          = TMP101NA_REG_CONFIG_FAULTQUE_6
    
} tmpFaultQue_t;

typedef enum
{
    ALERTPOL_LOW        = TMP101NA_REG_CONFIG_ALERTPOL_LOW,
    ALERTPOL_HIGH       = TMP101NA_REG_CONFIG_ALERTPOL_HIGH
    
} tmpAlertPol_t;

typedef enum
{
    MOD_COMP            = TMP101NA_REG_CONFIG_TM_COMP,
    MOD_INTR            = TMP101NA_REG_CONFIG_TM_INTR

} tmpMod_t;

typedef enum
{
    SHUTDOWN_DISABLE    = TMP101NA_REG_CONFIG_SD_DISABLE,
    SHUTDOWN_ENABLE     = TMP101NA_REG_CONFIG_SD_ENABLE
    
} tmpShutdown_t;


class TMP101NA
{
    protected:
        // Instance-specific properties
        uint8_t tmp_conversionDelay;
        tmpOneShot_t tmp_oneshot;
        tmpResolution_t tmp_resolution;
        tmpFaultQue_t tmp_faultqueue;
        tmpAlertPol_t tmp_alertpol;
        tmpMod_t tmp_mod;
        tmpShutdown_t tmp_shutdown;

    public:
        uint8_t tmp_i2cAddress;
        void getAddr_TMP101NA(uint8_t i2cAddress);
        void begin(void);
        int16_t Measure_Temp(void);
        void setOneShot(tmpOneShot_t oneshot);
        tmpOneShot_t getOneShot(void);
        void setResolution(tmpResolution_t resolution);
        tmpResolution_t getResolution(void);
        void setFaultQue(tmpFaultQue_t faultque);
        tmpFaultQue_t getFaultQue(void);
        void setAlertPol(tmpAlertPol_t alertpol);
        tmpAlertPol_t getAlertPol(void);
        void setMod(tmpMod_t mod);
        tmpMod_t getMod(void);
        void setShutdown(tmpShutdown_t shutdown);
        tmpShutdown_t getShutdown(void);
  
    private:
};
