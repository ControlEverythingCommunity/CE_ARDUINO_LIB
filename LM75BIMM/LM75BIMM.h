/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LM75BIMM
        This code is designed to work with the LM75BIMM_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=LM75BIMM_I2CS#tabs-0-product_tabset-2
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
//                                                              SLAVE ADDRESS
    #define LM75BIMM_DEFAULT_ADDRESS            (0x48)      //      1001000
    #define LM75BIMM_0_0_1_ADDRESS              (0x49)      //      1001001
    #define LM75BIMM_0_1_0_ADDRESS              (0x4A)      //      1001010
    #define LM75BIMM_0_1_1_ADDRESS              (0x4B)      //      1001011
    #define LM75BIMM_1_0_0_ADDRESS              (0x4C)      //      1001100
    #define LM75BIMM_1_0_1_ADDRESS              (0x4D)      //      1001101
    #define LM75BIMM_1_1_0_ADDRESS              (0x4E)      //      1001011
    #define LM75BIMM_1_1_1_ADDRESS              (0x4F)      //      1001111

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define LM75BIMM_CONVERSIONDELAY            (100)

/**************************************************************************
    POINTER REGISTER
**************************************************************************/
    #define LM75BIMM_REG_POINTER_MASK           (0x03)
    #define LM75BIMM_REG_POINTER_TEMP           (0x00)
    #define LM75BIMM_REG_POINTER_CONFIG         (0x01)
    #define LM75BIMM_REG_POINTER_THYST          (0x02)
    #define LM75BIMM_REG_POINTER_TOS            (0x03)

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/
    #define LM75BIMM_REG_CONFIG_FAULTQUE_MASK   (0x18)      // Fault Queue bit
    #define LM75BIMM_REG_CONFIG_FAULTQUE_1      (0x00)      // 1
    #define LM75BIMM_REG_CONFIG_FAULTQUE_2      (0x08)      // 2
    #define LM75BIMM_REG_CONFIG_FAULTQUE_4      (0x10)      // 4
    #define LM75BIMM_REG_CONFIG_FAULTQUE_6      (0x18)      // 6

    #define LM75BIMM_REG_CONFIG_OSPOL_MASK      (0x04)      // OS Polarity bit
    #define LM75BIMM_REG_CONFIG_OSPOL_HIGH      (0x04)      // Active-High
    #define LM75BIMM_REG_CONFIG_OSPOL_LOW       (0x00)      // Active-Low

    #define LM75BIMM_REG_CONFIG_COMPINT_MASK    (0x02)      // Comp/Int Bit
    #define LM75BIMM_REG_CONFIG_COMPINT_INTR    (0x02)      // Interrupt Output
    #define LM75BIMM_REG_CONFIG_COMPINT_COMP    (0x00)      // Comparator Output

    #define LM75BIMM_REG_CONFIG_SHDN_MASK       (0x01)      // Shutdown Bit
    #define LM75BIMM_REG_CONFIG_SHDN_ENABLE     (0x01)      // Enable
    #define LM75BIMM_REG_CONFIG_SHDN_DISABLE    (0x00)      // Disable


typedef enum
{
    FAULTQUE_1          = LM75BIMM_REG_CONFIG_FAULTQUE_1,
    FAULTQUE_2          = LM75BIMM_REG_CONFIG_FAULTQUE_2,
    FAULTQUE_4          = LM75BIMM_REG_CONFIG_FAULTQUE_4,
    FAULTQUE_6          = LM75BIMM_REG_CONFIG_FAULTQUE_6
    
} lmFaultQue_t;

typedef enum
{
    OSPOL_HIGH          = LM75BIMM_REG_CONFIG_OSPOL_HIGH,
    OSPOL_LOW           = LM75BIMM_REG_CONFIG_OSPOL_LOW
    
} lmOSPol_t;

typedef enum
{
    MOD_INTR            = LM75BIMM_REG_CONFIG_COMPINT_INTR,
    MOD_COMP            = LM75BIMM_REG_CONFIG_COMPINT_COMP

} lmMod_t;

typedef enum
{
    SHUTDOWN_ENABLE     = LM75BIMM_REG_CONFIG_SHDN_ENABLE,
    SHUTDOWN_DISABLE    = LM75BIMM_REG_CONFIG_SHDN_DISABLE
    
} lmShutdown_t;


class LM75BIMM
{
    protected:
        // Instance-specific properties
        uint8_t lm_conversionDelay;
        lmFaultQue_t lm_faultqueue;
        lmOSPol_t lm_ospol;
        lmMod_t lm_mod;
        lmShutdown_t lm_shutdown;

    public:
        uint8_t lm_i2cAddress;
        void getAddr_LM75BIMM(uint8_t i2cAddress);
        void begin(void);
        int16_t Measure_Temp(void);
        void setFaultQue(lmFaultQue_t faultque);
        lmFaultQue_t getFaultQue(void);
        void setOSPol(lmOSPol_t ospol);
        lmOSPol_t getOSPol(void);
        void setMod(lmMod_t mod);
        lmMod_t getMod(void);
        void setShutdown(lmShutdown_t shutdown);
        lmShutdown_t getShutdown(void);
  
    private:
};
