/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TCN75A
        This code is designed to work with the TCN75A_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=TCN75A_I2CS#tabs-0-product_tabset-2
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
    #define TCN75A_DEFAULT_ADDRESS              (0x48)      // 1001    0    0   0
    #define TCN75A_ADDRESS_A2A1A0_001           (0x49)      // 1001    0    0   1
    #define TCN75A_ADDRESS_A2A1A0_010           (0x4A)      // 1001    0    1   0
    #define TCN75A_ADDRESS_A2A1A0_011           (0x4B)      // 1001    0    1   1
    #define TCN75A_ADDRESS_A2A1A0_100           (0x4C)      // 1001    1    0   0
    #define TCN75A_ADDRESS_A2A1A0_101           (0x4D)      // 1001    1    0   1
    #define TCN75A_ADDRESS_A2A1A0_110           (0x4E)      // 1001    1    1   0
    #define TCN75A_ADDRESS_A2A1A0_111           (0x4F)      // 1001    1    1   1

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TCN75A_CONVERSIONDELAY                  (100)

/**************************************************************************
    POINTER REGISTER
**************************************************************************/
    #define TCN75A_REG_POINTER_MASK             (0x03)
    #define TCN75A_REG_POINTER_TEMP             (0x00)
    #define TCN75A_REG_POINTER_CONFIG           (0x01)
    #define TCN75A_REG_POINTER_THYST            (0x02)
    #define TCN75A_REG_POINTER_TLIMIT           (0x03)

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/

    #define TCN75A_REG_CONFIG_OS_MASK           (0x80)      // One-Shot bit
    #define TCN75A_REG_CONFIG_OS_ENABLED        (0x80)      // Enabled
    #define TCN75A_REG_CONFIG_OS_DISABLED       (0x00)      // Disabled (Power-up default)

    #define TCN75A_REG_CONFIG_RES_MASK          (0x60)      // ADC Resolution bit
    #define TCN75A_REG_CONFIG_RES_9BIT          (0x00)      // 9 bit (Power-up default)
    #define TCN75A_REG_CONFIG_RES_10BIT         (0x20)      // 10 bit
    #define TCN75A_REG_CONFIG_RES_11BIT         (0x40)      // 11 bit
    #define TCN75A_REG_CONFIG_RES_12BIT         (0x60)      // 12 bit

    #define TCN75A_REG_CONFIG_FAULTQUE_MASK     (0x18)      // Fault Queue bit
    #define TCN75A_REG_CONFIG_FAULTQUE_1        (0x00)      // 1 (Power-up default)
    #define TCN75A_REG_CONFIG_FAULTQUE_2        (0x08)      // 2
    #define TCN75A_REG_CONFIG_FAULTQUE_4        (0x10)      // 4
    #define TCN75A_REG_CONFIG_FAULTQUE_6        (0x18)      // 6

    #define TCN75A_REG_CONFIG_ALERTPOL_MASK     (0x04)      // Alert Polarity bit
    #define TCN75A_REG_CONFIG_ALERTPOL_HIGH     (0x04)      // Active-high
    #define TCN75A_REG_CONFIG_ALERTPOL_LOW      (0x00)      // Active-low (Power-up default)

    #define TCN75A_REG_CONFIG_COMPINT_MASK      (0x02)      // Comp/Int bit
    #define TCN75A_REG_CONFIG_COMPINT_INTR      (0x02)      // Interrupt output
    #define TCN75A_REG_CONFIG_COMPINT_COMP      (0x00)      // Comparator output (Power-up default)

    #define TCN75A_REG_CONFIG_SHDN_MASK         (0x01)      // Shutdown bit
    #define TCN75A_REG_CONFIG_SHDN_ENABLE       (0x01)      // Enable
    #define TCN75A_REG_CONFIG_SHDN_DISABLE      (0x00)      // Disable (Power-up default)


typedef enum
{
    ONESHOT_ENABLED     = TCN75A_REG_CONFIG_OS_ENABLED,
    ONESHOT_DISABLED    = TCN75A_REG_CONFIG_OS_DISABLED

} tcnOneShot_t;

typedef enum
{
    RESOLUTION_9        = TCN75A_REG_CONFIG_RES_9BIT,
    RESOLUTION_10       = TCN75A_REG_CONFIG_RES_10BIT,
    RESOLUTION_11       = TCN75A_REG_CONFIG_RES_11BIT,
    RESOLUTION_12       = TCN75A_REG_CONFIG_RES_12BIT
    
} tcnResolution_t;

typedef enum
{
    FAULTQUE_1          = TCN75A_REG_CONFIG_FAULTQUE_1,
    FAULTQUE_2          = TCN75A_REG_CONFIG_FAULTQUE_2,
    FAULTQUE_4          = TCN75A_REG_CONFIG_FAULTQUE_4,
    FAULTQUE_6          = TCN75A_REG_CONFIG_FAULTQUE_6
    
} tcnFaultQue_t;

typedef enum
{
    ALERTPOL_HIGH       = TCN75A_REG_CONFIG_ALERTPOL_HIGH,
    ALERTPOL_LOW        = TCN75A_REG_CONFIG_ALERTPOL_LOW
    
} tcnAlertPol_t;

typedef enum
{
    MOD_INTR            = TCN75A_REG_CONFIG_COMPINT_INTR,
    MOD_COMP            = TCN75A_REG_CONFIG_COMPINT_COMP

} tcnMod_t;

typedef enum
{
    SHUTDOWN_ENABLE     = TCN75A_REG_CONFIG_SHDN_ENABLE,
    SHUTDOWN_DISABLE    = TCN75A_REG_CONFIG_SHDN_DISABLE
    
} tcnShutdown_t;


class TCN75A
{
    protected:
        // Instance-specific properties
        uint8_t tcn_conversionDelay;
        tcnOneShot_t tcn_oneshot;
        tcnResolution_t tcn_resolution;
        tcnFaultQue_t tcn_faultqueue;
        tcnAlertPol_t tcn_alertpol;
        tcnMod_t tcn_mod;
        tcnShutdown_t tcn_shutdown;

    public:
        uint8_t tcn_i2cAddress;
        void getAddr_TCN75A(uint8_t i2cAddress);
        void begin(void);
        int16_t Measure_Temp(void);
        void setOneShot(tcnOneShot_t oneshot);
        tcnOneShot_t getOneShot(void);
        void setResolution(tcnResolution_t resolution);
        tcnResolution_t getResolution(void);
        void setFaultQue(tcnFaultQue_t faultque);
        tcnFaultQue_t getFaultQue(void);
        void setAlertPol(tcnAlertPol_t alertpol);
        tcnAlertPol_t getAlertPol(void);
        void setMod(tcnMod_t mod);
        tcnMod_t getMod(void);
        void setShutdown(tcnShutdown_t shutdown);
        tcnShutdown_t getShutdown(void);
  
    private:
};
