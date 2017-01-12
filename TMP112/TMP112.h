/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMP112
        This code is designed to work with the TMP112_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=TMP112_I2CS#tabs-0-product_tabset-2
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

/**************************************************************************
 I2C ADDRESS/BITS
 **************************************************************************/
    #define TMP112_DEFAULT_ADDRESS              (0x48)    // 1001000 (ADDR = GND)
    #define TMP112_ADDRESS_VPLUS                (0x49)    // 1001001 (ADDR = V+)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TMP112_CONVERSIONDELAY              (100)

/**************************************************************************
    POINTER REGISTER
**************************************************************************/
    #define TMP112_REG_POINTER_MASK             (0x03)
    #define TMP112_REG_POINTER_TEMP             (0x00)
    #define TMP112_REG_POINTER_CONFIG           (0x01)
    #define TMP112_REG_POINTER_TLOW             (0x02)
    #define TMP112_REG_POINTER_THIGH            (0x03)

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/

    #define TMP112_REG_CONFIG_OS_MASK           (0x8000)    // OS/ALERT
    #define TMP112_REG_CONFIG_OS_DISABLED       (0x0000)    // Disabled
    #define TMP112_REG_CONFIG_OS_ENABLED        (0x8000)    // Enabled

    #define TMP112_REG_CONFIG_RES_MASK          (0x6000)    // Converter Resolution, Read-Only
    #define TMP112_REG_CONFIG_RES_12BIT         (0x6000)    // 12 bit (0.0625°C)

    #define TMP112_REG_CONFIG_FAULTQUE_MASK     (0x1800)    // Fault Queue
    #define TMP112_REG_CONFIG_FAULTQUE_1        (0x0000)    // 1 Consecutive Fault
    #define TMP112_REG_CONFIG_FAULTQUE_2        (0x0800)    // 2 Consecutive Faults
    #define TMP112_REG_CONFIG_FAULTQUE_4        (0x1000)    // 4 Consecutive Faults
    #define TMP112_REG_CONFIG_FAULTQUE_6        (0x1800)    // 6 Consecutive Faults

    #define TMP112_REG_CONFIG_ALERTPOL_MASK     (0x0400)    // Polarity
    #define TMP112_REG_CONFIG_ALERTPOL_LOW      (0x0000)    // Active-high
    #define TMP112_REG_CONFIG_ALERTPOL_HIGH     (0x0400)    // Active-Low

    #define TMP112_REG_CONFIG_TM_MASK           (0x0200)    // Thermostat Mode
    #define TMP112_REG_CONFIG_TM_COMP           (0x0000)    // Comparator Mode
    #define TMP112_REG_CONFIG_TM_INTR           (0x0200)    // Interrupt Mode

    #define TMP112_REG_CONFIG_SD_MASK           (0x0100)    // Shutdown Mode
    #define TMP112_REG_CONFIG_SD_DISABLE        (0x0000)    // Disable/Continous Conversion
    #define TMP112_REG_CONFIG_SD_ENABLE         (0x0100)    // Enable Shutdown Mode

    #define TMP112_REG_CONFIG_CR_MASK           (0x00C0)    // Continuos-Conversion Mode
    #define TMP112_REG_CONFIG_CR_0_25           (0x0000)    // 0.25 Hz Conversion Rate
    #define TMP112_REG_CONFIG_CR_1              (0x0040)    // 1 Hz Conversion Rate
    #define TMP112_REG_CONFIG_CR_4              (0x0080)    // 4 Hz Conversion Rate (Default)
    #define TMP112_REG_CONFIG_CR_8              (0x00C0)    // 8 Hz Conversion Rate

    #define TMP112_REG_CONFIG_ALERT_MASK        (0x0020)    // Converter Resolution, Read-Only

    #define TMP112_REG_CONFIG_EM_MASK           (0x0001)    // Extended Mode
    #define TMP112_REG_CONFIG_EM_NORMAL         (0x0000)    // Normal Mode Operation
    #define TMP112_REG_CONFIG_EM_EXTENDED       (0x0001)    // Extended Mode Operation


typedef enum
{
    ONESHOT_DISABLED    = TMP112_REG_CONFIG_OS_DISABLED,
    ONESHOT_ENABLED     = TMP112_REG_CONFIG_OS_ENABLED

} tmpOneShot_t;

typedef enum
{
    FAULTQUE_1          = TMP112_REG_CONFIG_FAULTQUE_1,
    FAULTQUE_2          = TMP112_REG_CONFIG_FAULTQUE_2,
    FAULTQUE_4          = TMP112_REG_CONFIG_FAULTQUE_4,
    FAULTQUE_6          = TMP112_REG_CONFIG_FAULTQUE_6
    
} tmpFaultQue_t;

typedef enum
{
    ALERTPOL_LOW        = TMP112_REG_CONFIG_ALERTPOL_LOW,
    ALERTPOL_HIGH       = TMP112_REG_CONFIG_ALERTPOL_HIGH
    
} tmpAlertPol_t;

typedef enum
{
    THERMOD_COMP        = TMP112_REG_CONFIG_TM_COMP,
    THERMOD_INTR        = TMP112_REG_CONFIG_TM_INTR

} tmpTherMod_t;

typedef enum
{
    SHUTDOWN_DISABLE    = TMP112_REG_CONFIG_SD_DISABLE,
    SHUTDOWN_ENABLE     = TMP112_REG_CONFIG_SD_ENABLE
    
} tmpShutdown_t;

typedef enum
{
    CONVRATE_0_25       = TMP112_REG_CONFIG_CR_0_25,
    CONVRATE_1          = TMP112_REG_CONFIG_CR_1,
    CONVRATE_4          = TMP112_REG_CONFIG_CR_4,
    CONVRATE_8          = TMP112_REG_CONFIG_CR_8
    
} tmpConvRate_t;

typedef enum
{
    EXMOD_NORM          = TMP112_REG_CONFIG_EM_NORMAL,
    EXMOD_EXT           = TMP112_REG_CONFIG_EM_EXTENDED
    
} tmpExMod_t;


class TMP112
{
    protected:
        // Instance-specific properties
        uint8_t tmp_conversionDelay;
        tmpOneShot_t tmp_oneshot;
        tmpFaultQue_t tmp_faultqueue;
        tmpAlertPol_t tmp_alertpol;
        tmpTherMod_t tmp_thermod;
        tmpShutdown_t tmp_shutdown;
        tmpConvRate_t tmp_convrate;
        tmpExMod_t tmp_exmod;
    
    public:
        uint8_t tmp_i2cAddress;
        void getAddr_TMP112(uint8_t i2cAddress);
        void begin(void);
        int16_t Measure_Temp(void);
        void setOneShot(tmpOneShot_t oneshot);
        tmpOneShot_t getOneShot(void);
        void setFaultQue(tmpFaultQue_t faultque);
        tmpFaultQue_t getFaultQue(void);
        void setAlertPol(tmpAlertPol_t alertpol);
        tmpAlertPol_t getAlertPol(void);
        void setTherMod(tmpTherMod_t thermod);
        tmpTherMod_t getTherMod(void);
        void setShutdown(tmpShutdown_t shutdown);
        tmpShutdown_t getShutdown(void);
        void setConvRate(tmpConvRate_t convrate);
        tmpConvRate_t getConvRate(void);
        void setExMod(tmpExMod_t exmod);
        tmpExMod_t getExMod(void);
  
    private:
};
