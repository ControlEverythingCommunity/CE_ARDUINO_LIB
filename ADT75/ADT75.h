/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADT75
        This code is designed to work with the ADT75_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=ADT75_I2CS#tabs-0-product_tabset-2
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
    #define ADT75_DEFAULT_ADDRESS           (0x48)      // 1001    0    0   0
    #define ADT75_ADDRESS_A2A1A0_001        (0x49)      // 1001    0    0   1
    #define ADT75_ADDRESS_A2A1A0_010        (0x4A)      // 1001    0    1   0
    #define ADT75_ADDRESS_A2A1A0_011        (0x4B)      // 1001    0    1   1
    #define ADT75_ADDRESS_A2A1A0_100        (0x4C)      // 1001    1    0   0
    #define ADT75_ADDRESS_A2A1A0_101        (0x4D)      // 1001    1    0   1
    #define ADT75_ADDRESS_A2A1A0_110        (0x4E)      // 1001    1    1   0
    #define ADT75_ADDRESS_A2A1A0_111        (0x4F)      // 1001    1    1   1


/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define ADT75_CONVERSIONDELAY           (100)

/**************************************************************************
    POINTER REGISTER
**************************************************************************/
    #define ADT75_REG_POINTER_MASK          (0x04)
    #define ADT75_REG_POINTER_TEMP          (0x00)
    #define ADT75_REG_POINTER_CONFIG        (0x01)
    #define ADT75_REG_POINTER_THYST         (0x02)
    #define ADT75_REG_POINTER_TSET          (0x03)
    #define ADT75_REG_POINTER_TOS           (0x04)

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/
    #define ADT75_REG_CONFIG_SM_MASK        (0x80)    // OS/SMBus Alert Mode
    #define ADT75_REG_CONFIG_SM_DISABLED    (0x00)    // Disable SMBus Alert Function
    #define ADT75_REG_CONFIG_SM_ENABLED     (0x80)    // Enable SMBus Alert Function

    #define ADT75_REG_CONFIG_OS_MASK        (0x60)    // One-Shot Mode
    #define ADT75_REG_CONFIG_OS_NORMAL      (0x00)    // Normal Mode
    #define ADT75_REG_CONFIG_OS_ONESHOT     (0x60)    // One-Shot Mode

    #define ADT75_REG_CONFIG_FAULTQUE_MASK  (0x18)    // Overtemperature Fault Queue
    #define ADT75_REG_CONFIG_FAULTQUE_1     (0x00)    // 1 Overtemperature Fault
    #define ADT75_REG_CONFIG_FAULTQUE_2     (0x08)    // 2 Overtemperature Faults
    #define ADT75_REG_CONFIG_FAULTQUE_4     (0x10)    // 4 Overtemperature Faults
    #define ADT75_REG_CONFIG_FAULTQUE_6     (0x18)    // 6 Overtemperature Faults

    #define ADT75_REG_CONFIG_ALERTPOL_MASK  (0x04)    // OS/Alert Pin Polarity
    #define ADT75_REG_CONFIG_ALERTPOL_LOW   (0x00)    // Active-Low
    #define ADT75_REG_CONFIG_ALERTPOL_HIGH  (0x04)    // Active-High

    #define ADT75_REG_CONFIG_TMPMOD_MASK    (0x02)    // Over Temperature Interrupt Modes
    #define ADT75_REG_CONFIG_TMPMOD_COMP    (0x00)    // Comparator Output
    #define ADT75_REG_CONFIG_TMPMOD_INTR    (0x02)    // Interrupt Output

    #define ADT75_REG_CONFIG_SHDN_MASK      (0x01)    // Shutdown Bit
    #define ADT75_REG_CONFIG_SHDN_DISABLE   (0x00)    // Disable
    #define ADT75_REG_CONFIG_SHDN_ENABLE    (0x01)    // Enable


typedef enum
{
    SM_DISABLED         = ADT75_REG_CONFIG_SM_DISABLED,
    SM_ENABLED          = ADT75_REG_CONFIG_SM_ENABLED
    
} adtSM_t;

typedef enum
{
    ONESHOT_NORMAL      = ADT75_REG_CONFIG_OS_NORMAL,
    ONESHOT_ENABLED     = ADT75_REG_CONFIG_OS_ONESHOT
    
} adtOneShot_t;

typedef enum
{
    FAULTQUE_1          = ADT75_REG_CONFIG_FAULTQUE_1,
    FAULTQUE_2          = ADT75_REG_CONFIG_FAULTQUE_2,
    FAULTQUE_4          = ADT75_REG_CONFIG_FAULTQUE_4,
    FAULTQUE_6          = ADT75_REG_CONFIG_FAULTQUE_6
    
} adtFaultQue_t;

typedef enum
{
    ALERTPOL_LOW        = ADT75_REG_CONFIG_ALERTPOL_LOW,
    ALERTPOL_HIGH       = ADT75_REG_CONFIG_ALERTPOL_HIGH
    
} adtAlertPol_t;

typedef enum
{
    MOD_COMP            = ADT75_REG_CONFIG_TMPMOD_COMP,
    MOD_INTR            = ADT75_REG_CONFIG_TMPMOD_INTR
    
} adtMod_t;

typedef enum
{
    SHUTDOWN_DISABLE    = ADT75_REG_CONFIG_SHDN_DISABLE,
    SHUTDOWN_ENABLE     = ADT75_REG_CONFIG_SHDN_ENABLE
    
} adtShutdown_t;


class ADT75
{
    protected:
        // Instance-specific properties
        uint8_t adt_conversionDelay;
        adtSM_t adt_sm;
        adtOneShot_t adt_oneshot;
        adtFaultQue_t adt_faultqueue;
        adtAlertPol_t adt_alertpol;
        adtMod_t adt_mod;
        adtShutdown_t adt_shutdown;
    
    public:
        uint8_t adt_i2cAddress;
        void getAddr_ADT75(uint8_t i2cAddress);
        void begin(void);
        int16_t Measure_Temp(void);
        void setSM(adtSM_t sm);
        adtSM_t getSM(void);
        void setOneShot(adtOneShot_t oneshot);
        adtOneShot_t getOneShot(void);
        void setFaultQue(adtFaultQue_t faultque);
        adtFaultQue_t getFaultQue(void);
        void setAlertPol(adtAlertPol_t alertpol);
        adtAlertPol_t getAlertPol(void);
        void setMod(adtMod_t mod);
        adtMod_t getMod(void);
        void setShutdown(adtShutdown_t shutdown);
        adtShutdown_t getShutdown(void);
    
    private:
};
