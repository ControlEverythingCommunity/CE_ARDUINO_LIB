/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MXC6232xM
        This code is designed to work with the MXC6232xM_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/low-power-2g-dual-axis-accelerometer?variant=25687720907#tabs-0-product_tabset-2
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
    #define MXC6232xM_DEFAULT_ADDRESS_ACCEL                 (0x10)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define MXC6232xM_CONVERSIONDELAY                       (100)

/**************************************************************************
    ACCELEROMETER REGISTERS
**************************************************************************/
    #define MXC6232xM_REG_ACCEL_DATA                        (0x00)

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define MXC6232xM_REG_ACCEL_IR_TOEN_MASK                (0x08)      // Temperature Out Status
    #define MXC6232xM_REG_ACCEL_IR_TOEN_DISABLE             (0x00)      // Temperature Out Disable
    #define MXC6232xM_REG_ACCEL_IR_TOEN_ENABLE              (0x08)      // Temperature Out Enable

    #define MXC6232xM_REG_ACCEL_IR_BGTST_MASK               (0x04)      // BandGap Test Status
    #define MXC6232xM_REG_ACCEL_IR_BGTST_NORMAL             (0x00)      // Normal Test
    #define MXC6232xM_REG_ACCEL_IR_BGTST_BANDGAP            (0x04)      // Bandga Test

    #define MXC6232xM_REG_ACCEL_IR_ST_MASK                  (0x02)      // Self Test Status
    #define MXC6232xM_REG_ACCEL_IR_ST_OFF                   (0x00)      // Self Test Off
    #define MXC6232xM_REG_ACCEL_IR_ST_ON                    (0x02)      // Self Test On

    #define MXC6232xM_REG_ACCEL_IR_PD_MASK                  (0x01)      // Power Status
    #define MXC6232xM_REG_ACCEL_IR_PD_ON                    (0x00)      // Power On
    #define MXC6232xM_REG_ACCEL_IR_PD_DOWN                  (0x01)      // Power Down


typedef enum
{
    ACCEL_TEMP_OUT_DISABLE              = MXC6232xM_REG_ACCEL_IR_TOEN_DISABLE,
    ACCEL_TEMP_OUT_ENABLE               = MXC6232xM_REG_ACCEL_IR_TOEN_ENABLE
    
} mxcAccelTempOut_t;

typedef enum
{
    ACCEL_BANDGAP_TEST                  = MXC6232xM_REG_ACCEL_IR_BGTST_BANDGAP,
    ACCEL_NORMAL_TEST                   = MXC6232xM_REG_ACCEL_IR_BGTST_NORMAL
    
} mxcAccelBandgap_t;

typedef enum
{
    ACCEL_SELF_TEST_OFF                 = MXC6232xM_REG_ACCEL_IR_ST_OFF,
    ACCEL_SELF_TEST_ON                  = MXC6232xM_REG_ACCEL_IR_ST_ON
    
} mxcAccelSelfTest_t;

typedef enum
{
    ACCEL_POWER_ON                      = MXC6232xM_REG_ACCEL_IR_PD_ON,
    ACCEL_POWER_DOWN                    = MXC6232xM_REG_ACCEL_IR_PD_DOWN
    
} mxcAccelPower_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    
} mxcSensorData_t;


class MXC6232xM
{
    protected:
        // Instance-specific properties
        uint8_t mxc_conversionDelay;
        mxcAccelTempOut_t mxc_acceltempout;
        mxcAccelBandgap_t mxc_accelbandgap;
        mxcAccelSelfTest_t mxc_accelselftest;
        mxcAccelPower_t mxc_accelpower;
    
    public:
        uint8_t mxc_i2cAddress;
        mxcSensorData_t mxc_accelData;
        void getAddr_MXC6232xM(uint8_t i2cAddress);
        bool begin(void);
        void setUpAccelerometer(void);
        void Measure_Accelerometer(void);
        void setAccelTempOut(mxcAccelTempOut_t acceltempout);
        mxcAccelTempOut_t getAccelTempOut(void);
        void setAccelBandgap(mxcAccelBandgap_t accelbandgap);
        mxcAccelBandgap_t getAccelBandgap(void);
        void setAccelSelfTest(mxcAccelSelfTest_t accelselftest);
        mxcAccelSelfTest_t getAccelSelfTest(void);
        void setAccelPower(mxcAccelPower_t accelpower);
        mxcAccelPower_t getAccelPower(void);
    
    private:
};
