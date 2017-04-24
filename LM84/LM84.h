/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LM84
        This code is designed to work with the LM84_I2CS I2C Mini Module available from ControlEverything.com.
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
/*
                                                        Address Select              LM84 SMBus
                                                        Pin State                   Slave Address
                                                            ADD0        ADD1        A6:A0 binary
*/
    #define LM84_DEFAULT_ADDRESS                (0x18)  // 0            0           001 1000
    #define LM84_ADDRESS_ADD0ADD1_0TRI          (0x19)  // 0            TRI-LEVEL   001 1001
    #define LM84_ADDRESS_ADD0ADD1_01            (0x1A)  // 0            1           001 1010
    #define LM84_ADDRESS_ADD0ADD1_TRI0          (0x29)  // TRI-LEVEL    0           010 1001
    #define LM84_ADDRESS_ADD0ADD1_TRITRI        (0x2A)  // TRI-LEVEL    TRI-LEVEL   010 1010
    #define LM84_ADDRESS_ADD0ADD1_TRI1          (0x2B)  // TRI-LEVEL    1           010 1011
    #define LM84_ADDRESS_ADD0ADD1_10            (0x4C)  // 1            0           100 1100
    #define LM84_ADDRESS_ADD0ADD1_1TRI          (0x4D)  // 1            TRI-LEVEL   100 1101
    #define LM84_ADDRESS_ADD0ADD1_11            (0x4E)  // 1            1           100 1110
    #define LM84_REG_MANUFID                    (0x00)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define LM84_CONVERSIONDELAY                (100)

/**************************************************************************
    COMMAND REGISTER
**************************************************************************/
    #define LM84_REG_COMMAND_RLT                (0x00)  // Read Local Temperature
    #define LM84_REG_COMMAND_RRT                (0x01)  // Read Remote Temperature
    #define LM84_REG_COMMAND_RS                 (0x02)  // Read Status
    #define LM84_REG_COMMAND_RC                 (0x03)  // Read Configuration
    #define LM84_REG_COMMAND_RMID               (0x04)  // Manufacturers ID
    #define LM84_REG_COMMAND_RLCS               (0x05)  // Read Local T_CRIT Setpoint
    #define LM84_REG_COMMAND_RRCS               (0x07)  // Read Remote T_CRIT Setpoint
    #define LM84_REG_COMMAND_WC                 (0x09)  // Write Configuration
    #define LM84_REG_COMMAND_WLCS               (0x0B)  // Write Local T_CRIT Setpoint
    #define LM84_REG_COMMAND_WRCS               (0x0D)  // Write Remote T_CRIT Setpoint

/**************************************************************************
    CONFIGURATION REGISTER
 **************************************************************************/
    #define LM84_REG_COMMAND_WC_T_CRIT_A_MASK   (0x80)    // T_CRIT_A Interrupts
    #define LM84_REG_COMMAND_WC_T_CRIT_A_0      (0x00)    // T_CRIT_A Interrupts are not Masked
    #define LM84_REG_COMMAND_WC_T_CRIT_A_1      (0x80)    // T_CRIT_A Interrupts are Masked


typedef enum
{
    T_CRIT_A_NOT_MASKED     = LM84_REG_COMMAND_WC_T_CRIT_A_0,
    T_CRIT_A_MASKED         = LM84_REG_COMMAND_WC_T_CRIT_A_1
} lmTCritA_t;

typedef struct
{
    float LT;
    float RT;
    
} lmSensorData_t;


class LM84
{
    protected:
        // Instance-specific properties
        uint8_t lm_conversionDelay;
        lmTCritA_t lm_tcrita;

    public:
        uint8_t lm_i2cAddress;
        lmSensorData_t lm_tempData;
        void getAddr_LM84(uint8_t i2cAddress);
        bool begin(void);
        void setUpConfig(void);
        void Measure_Temp(void);
        void setTCritA(lmTCritA_t tcrita);
        lmTCritA_t getTCritA(void);

    private:
};
