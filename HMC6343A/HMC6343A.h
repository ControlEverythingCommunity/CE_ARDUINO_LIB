/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HMC6343A
        This code is designed to work with the HMC6343A_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=HMC6343A_I2CS#tabs-0-product_tabset-2
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
    #define HMC6343A_DEFAULT_ADDRESS                    (0x19)

/**************************************************************************
    CONVERSION DELAY (in mS)x
**************************************************************************/
    #define HMC6343A_CONVERSIONDELAY                    (100)

/**************************************************************************
    REGISTERS DESCRIPTION
**************************************************************************/
    #define HMC6343A_REG_ACCELACCELMAG_SW_VERSION       (0x02)      // Software Version Number Register
    #define HMC6343A_REG_ACCELMAG_OP_MODE1              (0x04)      // Operational Mode Register 1
    #define HMC6343A_REG_ACCELMAG_OP_MODE2              (0x05)      // Operational Mode Register 2
    #define HMC6343A_REG_ACCELMAG_SN_LSB                (0x06)      // Device Serial Number Register
    #define HMC6343A_REG_ACCELMAG_SN_MSB                (0x07)      // Device Serial Number Register
    #define HMC6343A_REG_ACCELMAG_DEVIATION_LSB         (0x0A)      // Deviation Angle (±1800) in Tenths of a Degree Register
    #define HMC6343A_REG_ACCELMAG_DEVIATION_MSB         (0x0B)      // Deviation Angle (±1800) in Tenths of a Degree Register
    #define HMC6343A_REG_ACCELMAG_VARIATION_LSB         (0x0C)      // Variation Angle (±1800) in Tenths of a Degree Register
    #define HMC6343A_REG_ACCELMAG_VARIATION_MSB         (0x0D)      // Variation Angle (±1800) in Tenths of a Degree Register
    #define HMC6343A_REG_ACCELMAG_XOFFSET_LSB           (0x0E)      // Hard-Iron Calibration Offset for the X-Axis Register
    #define HMC6343A_REG_ACCELMAG_XOFFSET_MSB           (0x0F)      // Hard-Iron Calibration Offset for the X-Axis Register
    #define HMC6343A_REG_ACCELMAG_YOFFSET_LSB           (0x10)      // Hard-Iron Calibration Offset for the Y-Axis Register
    #define HMC6343A_REG_ACCELMAG_YOFFSET_MSB           (0x11)      // Hard-Iron Calibration Offset for the Y-Axis Register
    #define HMC6343A_REG_ACCELMAG_ZOFFSET_LSB           (0x12)      // Hard-Iron Calibration Offset for the Z-Axis Register
    #define HMC6343A_REG_ACCELMAG_ZOFFSET_MSB           (0x13)      // Hard-Iron Calibration Offset for the Z-Axis Register
    #define HMC6343A_REG_ACCELMAG_FILTER_LSB            (0x14)      // Heading IIR Filter (0x00 to 0x0F typical) Register
    #define HMC6343A_REG_ACCELMAG_FILTER_MSB            (0x15)      // Heading IIR Filter (set at zero) Register
    #define HMC6343A_REG_ACCELMAG_POST_ACCEL            (0x40)      // 6 Binary Data Bytes. AxMSB, AxLSB, AyMSB, AyLSB, AzMSB, AzLSB
    #define HMC6343A_REG_ACCELMAG_POST_MAG              (0x45)      // 6 Binary Data Bytes. MxMSB, MxLSB, MyMSB, MyLSB, MzMSB, MzLSB
    #define HMC6343A_REG_ACCELMAG_POST_HEADING          (0x50)      // 6 Binary Data Bytes. HeadMSB, HeadLSB, PitchMSB, PitchLSB, RollMSB, RollLSB
    #define HMC6343A_REG_ACCELMAG_POST_TILT             (0x55)      // 6 Binary Data Bytes. PitchMSB, PitchLSB, RollMSB, RollLSB, TempMSB, TempLSB
    #define HMC6343A_REG_ACCELMAG_POST_OP_MODE1         (0x65)      // OP Mode 1 Register
    #define HMC6343A_REG_ACCELMAG_USER_CALIB_MODE_ENTER (0x71)      // No Response Data
    #define HMC6343A_REG_ACCELMAG_LEVEL_ORIENTATION     (0x72)      // (X=forward, +Z=up) (default) No Response Data
    #define HMC6343A_REG_ACCELMAG_UPRIGHT_SIDE_ORIENT   (0x73)      // (X=forward, Y=up) No Response Data
    #define HMC6343A_REG_ACCELMAG_UPRIGHT_FLAT_ORIENT   (0x74)      // (Z=forward, -X=up) No Response Data
    #define HMC6343A_REG_ACCELMAG_RUM_MODE              (0x75)      // No Response Data
    #define HMC6343A_REG_ACCELMAG_STANDBY_MODE          (0x76)      // No Response Data
    #define HMC6343A_REG_ACCELMAG_USER_CALIB_MODE_EXIT  (0x7E)      // Identification Register A
    #define HMC6343A_REG_ACCELMAG_RESET_PROCESSOR       (0x82)      // No Response Data
    #define HMC6343A_REG_ACCELMAG_SLEEP_MODE_ENTER      (0x83)      // No Response Data
    #define HMC6343A_REG_ACCELMAG_SLEEP_MODE_EXIT       (0x84)      // No Response Data
    #define HMC6343A_REG_ACCELMAG_READ_EEPROM           (0xE1)      // 1 Binary Data Byte
    #define HMC6343A_REG_ACCELMAG_WRITE_EEPROM          (0xF1)      // No Response Data. Data Settling Time

/**************************************************************************
    OPERATIONAL MODE REGISTER 1 CONFIGURATION
**************************************************************************/
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_COMP_MASK    (0x80)  // Calculating Compass Data if Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_COMP_NOSET   (0x00)  // No Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_COMP_SET     (0x80)  // Calculating Compass Data

    #define HMC6343A_REG_ACCELMAG_OP_MODE1_CAL_MASK     (0x40)  // Calculating Calibration Offsets if set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_CAL_NOSET    (0x00)  // No Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_CAL_SET      (0x40)  // Calculating Calibration Offsets

    #define HMC6343A_REG_ACCELMAG_OP_MODE1_FILTER_MASK  (0x20)  // IIR Heading Filter Used if Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_FILTER_NOSET (0x00)  // No Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_FILTER_SET   (0x20)  // IIR Heading Filter Used

    #define HMC6343A_REG_ACCELMAG_OP_MODE1_RUN_MASK     (0x10)  // Run Mode if Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_RUN_NOSET    (0x00)  // No Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_RUN_SET      (0x10)  // Run Mode

    #define HMC6343A_REG_ACCELMAG_OP_MODE1_STANDBY_MASK (0x08)  // Standby Mode if Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_STANDBY_NOSET (0x00)  // No Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_STANDBY_SET  (0x08)  // Standby Mode

    #define HMC6343A_REG_ACCELMAG_OP_MODE1_UF_MASK      (0x04)  // Upright Front Orientation if Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_UF_NOSET     (0x00)  // No Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_UF_SET       (0x04)  // Upright Front Orientation

    #define HMC6343A_REG_ACCELMAG_OP_MODE1_UE_MASK      (0x02)  // Upright Edge Orientation if Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_UE_NOSET     (0x00)  // No Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_UE_SET       (0x02)  // Upright Edge Orientation

    #define HMC6343A_REG_ACCELMAG_OP_MODE1_LEVEL_MASK   (0x01)  // Level Orientation if Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_LEVEL_NOSET  (0x00)  // No Set
    #define HMC6343A_REG_ACCELMAG_OP_MODE1_LEVEL_SET    (0x01)  // Level Orientation

/**************************************************************************
    OPERATIONAL MODE REGISTER 2 CONFIGURATION
**************************************************************************/
    #define HMC6343A_REG_ACCELMAG_OP_MODE2_MR_MASK      (0x03)  // Measurement Rate
    #define HMC6343A_REG_ACCELMAG_OP_MODE2_MR_1HZ       (0x00)  // 0,0 = 1Hz
    #define HMC6343A_REG_ACCELMAG_OP_MODE2_MR_5HZ       (0x01)  // 0,1 = 5Hz
    #define HMC6343A_REG_ACCELMAG_OP_MODE2_MR_10HZ      (0x02)  // 1,0 = 10Hz


typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
} hmcSensorData_t;


class HMC6343A
{
    protected:
        // Instance-specific properties
        uint8_t hmc_ConversionDelay;
    
    public:
        uint8_t hmc_i2cAddress;
        hmcSensorData_t hmc_accelData;
        hmcSensorData_t hmc_magData;
        void getAddr_HMC6343A(uint8_t i2cAddress);
        bool begin(void);
        void setUpSensor(void);
        void Measure_Sensor();
        void Measure_Accelerometer(void);
        void Measure_Magnetometer(void);
    
    private:
};
