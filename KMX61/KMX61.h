/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        KMX61
        This code is designed to work with the KMX61_I2CS I2C Mini Module available from ControlEverything.com.
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
    #define KMX61_DEFAULT_ADDRESS                   (0x0E)      // 0011001b

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define KMX61_CONVERSIONDELAY                   (100)

/**************************************************************************
    ACCELEROMETER REGISTERS
**************************************************************************/
    #define KMX61_REG_WHO_AM_I                      (0x00)      // Who Am I Register
    #define KMX61_REG_INS1                          (0x01)      // Interrupt Source Register 1
    #define KMX61_REG_INS2                          (0x02)      // Interrupt Source Register 2
    #define KMX61_REG_STATUS                        (0x03)      // Status Register
    #define KMX61_REG_ACCEL_XOUT_L                  (0x0A)      // X-Axis Acceleration Data Low Register
    #define KMX61_REG_ACCEL_XOUT_H                  (0x0B)      // X-Axis Acceleration Data High Register
    #define KMX61_REG_ACCEL_YOUT_L                  (0x0C)      // Y-Axis Acceleration Data Low Register
    #define KMX61_REG_ACCEL_YOUT_H                  (0x0D)      // Y-Axis Acceleration Data High Register
    #define KMX61_REG_ACCEL_ZOUT_L                  (0x0E)      // Z-Axis Acceleration Data Low Register
    #define KMX61_REG_ACCEL_ZOUT_H                  (0x0F)      // Z-Axis Acceleration Data High Register
    #define KMX61_REG_TEMP_OUT_L                    (0x10)      // Temperature Data Low Output Register
    #define KMX61_REG_TEMP_OUT_H                    (0x11)      // Temperature Data High Output Register
    #define KMX61_REG_MAG_XOUT_L                    (0x12)      // X-Axis Magnetic Data Low Register
    #define KMX61_REG_MAG_XOUT_H                    (0x13)      // X-Axis Magnetic Data High Register
    #define KMX61_REG_MAG_YOUT_L                    (0x14)      // Y-Axis Magnetic Data Low Register
    #define KMX61_REG_MAG_YOUT_H                    (0x15)      // Y-Axis Magnetic Data High Register
    #define KMX61_REG_MAG_ZOUT_L                    (0x16)      // Z-Axis Magnetic Data Low Register
    #define KMX61_REG_MAG_ZOUT_H                    (0x17)      // Z-Axis Magnetic Data High Register
    #define KMX61_REG_XOUT_HPF_L                    (0x18)      // High-Pass Filtered X-Axis Acceleration Data Low Register
    #define KMX61_REG_XOUT_HPF_H                    (0x19)      // High-Pass Filtered X-Axis Acceleration Data High Register
    #define KMX61_REG_YOUT_HPF_L                    (0x1A)      // High-Pass Filtered Y-Axis Acceleration Data Low Register
    #define KMX61_REG_YOUT_HPF_H                    (0x1B)      // High-Pass Filtered Y-Axis Acceleration Data High Register
    #define KMX61_REG_ZOUT_HPF_L                    (0x1C)      // High-Pass Filtered Z-Axis Acceleration Data Low Register
    #define KMX61_REG_ZOUT_HPF_H                    (0x1D)      // High-Pass Filtered Z-Axis Acceleration Data High Register
    #define KMX61_REG_SN_1                          (0x24)      // Individual Identification (Serial Number) Register 1
    #define KMX61_REG_SN_2                          (0x25)      // Individual Identification (Serial Number) Register 2
    #define KMX61_REG_SN_3                          (0x26)      // Individual Identification (Serial Number) Register 3
    #define KMX61_REG_SN_4                          (0x27)      // Individual Identification (Serial Number) Register 4
    #define KMX61_REG_INL                           (0x28)      // Interrupt Latch Release Register
    #define KMX61_REG_STBY_REG                      (0x29)      // Stand-by Status Register
    #define KMX61_REG_CNTL1                         (0x2A)      // Control Register 1
    #define KMX61_REG_CNTL2                         (0x2B)      // Control Register 2
    #define KMX61_REG_ODCNTL                        (0x2C)      // Output Data Control Register
    #define KMX61_REG_INC1                          (0x2D)      // Magnetic Field Sensing B Register
    #define KMX61_REG_INC2                          (0x2E)      // Magnetic Mode Select Register
    #define KMX61_REG_INC3                          (0x2F)      // X-Axis Magnetic Data High Register
    #define KMX61_REG_COTR                          (0x3C)      // X-Axis Magnetic Data Low Register
    #define KMX61_REG_WUFTH                         (0x3D)      // Y-Axis Magnetic Data High Register
    #define KMX61_REG_WUFC                          (0x3E)      // Y-Axis Magnetic Data Low Register
    #define KMX61_REG_BTH                           (0x3F)      // Z-Axis Magnetic Data High Register
    #define KMX61_REG_BTSC                          (0x40)      // Z-Axis Magnetic Data Low Register
    #define KMX61_REG_TEMP_EN_CNTL                  (0x4C)      // Data Status Ready Register
    #define KMX61_REG_SELF_TEST                     (0x60)      // Interrupt Request/Recognition A Register
    #define KMX61_REG_BUF_THRESH_H                  (0x76)      // Interrupt Request/Recognition B Register
    #define KMX61_REG_BUF_THRESH_L                  (0x77)      // Interrupt Request/Recognition C Register
    #define KMX61_REG_BUF_CTRL1                     (0x78)      // Y-Axis Magnetic Data High Register
    #define KMX61_REG_BUF_CTRL2                     (0x79)      // Y-Axis Magnetic Data Low Register
    #define KMX61_REG_BUF_CLEAR                     (0x7A)      // Z-Axis Magnetic Data High Register
    #define KMX61_REG_BUF_STATUS_REG                (0x7B)      // Z-Axis Magnetic Data Low Register
    #define KMX61_REG_BUF_STATUS_H                  (0x7C)      // Data Status Ready Register
    #define KMX61_REG_BUF_STATUS_L                  (0x7D)      // Interrupt Request/Recognition A Register
    #define KMX61_REG_BUF_READ                      (0x7E)      // Interrupt Request/Recognition B Register

/**************************************************************************
    STAND-BY STATUS REGISTER DESCRIPTION
**************************************************************************/
    #define KMX61_REG_STBY_REG_ACT_STBY_MASK        (0x80)      // Allows the ASIC_AL to Control the Operating Mode of all of the Connected Devices based on the Activity State of the Device
    #define KMX61_REG_STBY_REG_ACT_STBY_0           (0x00)      // Feature Disabled
    #define KMX61_REG_STBY_REG_ACT_STBY_1           (0x80)      // Feature Enabled

    #define KMX61_REG_STBY_REG_MAG_STBY_MASK        (0x02)      // Controls the Operating Mode of the ASIC_AL’s Magnetometer
    #define KMX61_REG_STBY_REG_MAG_STBY_0           (0x00)      // Operating Mode
    #define KMX61_REG_STBY_REG_MAG_STBY_1           (0x02)      // Stand-by Mode

    #define KMX61_REG_STBY_REG_ACCEL_STBY_MASK      (0x01)      // Controls the Operating Mode of the ASIC_AL’s Accelerometer
    #define KMX61_REG_STBY_REG_ACCEL_STBY_0         (0x00)      // Operating Mode
    #define KMX61_REG_STBY_REG_ACCEL_STBY_1         (0x01)      // Stand-by Mode

/**************************************************************************
    CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define KMX61_REG_CNTL1_BTSE_MASK               (0x80)      // Enables the Back to Sleep Engine
    #define KMX61_REG_CNTL1_BTSE_DISABLE            (0x00)      // Disable
    #define KMX61_REG_CNTL1_BTSE_ENABLE             (0x80)      // Enable

    #define KMX61_REG_CNTL1_WUFE_MASK               (0x40)      // Enables the Wake up Engine
    #define KMX61_REG_CNTL1_WUFE_DISABLE            (0x00)      // Disable
    #define KMX61_REG_CNTL1_WUFE_ENABLE             (0x40)      // Enable

    #define KMX61_REG_CNTL1_GSEL_MASK               (0x03)      // Selects the Acceleration Range of the Accelerometer Outputs
    #define KMX61_REG_CNTL1_GSEL_2G                 (0x00)      // +/-2g
    #define KMX61_REG_CNTL1_GSEL_4G                 (0x01)      // +/-4g
    #define KMX61_REG_CNTL1_GSEL_8G                 (0x02)      // +/-8g: 12-bit
    #define KMX61_REG_CNTL1_GSEL_8G_14BIT           (0x03)      // +/-8g: 14-bit

/**************************************************************************
    CONTROL REGISTER 2 DESCRIPTION
**************************************************************************/
    #define KMX61_REG_CNTL2_SRST_MASK               (0x80)      // Allows the ASIC_AL to Control the Operating Mode of all of the Connected Devices based on the Activity State of the Device
    #define KMX61_REG_CNTL2_SRST_0                  (0x00)      // Feature Disabled
    #define KMX61_REG_CNTL2_SRST_1                  (0x80)      // Feature Enabled

    #define KMX61_REG_CNTL2_STPOL_MASK              (0x40)      // ST Polarity Control
    #define KMX61_REG_CNTL2_STPOL_0                 (0x00)      // ST Polarity is Positive
    #define KMX61_REG_CNTL2_STPOL_1                 (0x40)      // ST Polarity is Negative

    #define KMX61_REG_CNTL2_COTC_MASK               (0x10)      // Enables the Command Test Function
    #define KMX61_REG_CNTL2_COTC_0                  (0x00)      // No Action
    #define KMX61_REG_CNTL2_COTC_1                  (0x10)      // Sets AAh to COTR Register, When the COTR Register is Read, COTC is Cleared and STR = 55h

    #define KMX61_REG_CNTL2_OWUF_MASK               (0x07)      // Output Data Rate at which the Wake Up (Motion Detection) performs its Function
    #define KMX61_REG_CNTL2_OWUF_0_781              (0x00)      // Output Data Rate (Hz): 0.781
    #define KMX61_REG_CNTL2_OWUF_1_563              (0x01)      // Output Data Rate (Hz): 1.563
    #define KMX61_REG_CNTL2_OWUF_3_125              (0x02)      // Output Data Rate (Hz): 3.125
    #define KMX61_REG_CNTL2_OWUF_6_25               (0x03)      // Output Data Rate (Hz): 6.25
    #define KMX61_REG_CNTL2_OWUF_12_5               (0x04)      // Output Data Rate (Hz): 12.5
    #define KMX61_REG_CNTL2_OWUF_25                 (0x05)      // Output Data Rate (Hz): 25
    #define KMX61_REG_CNTL2_OWUF_50                 (0x06)      // Output Data Rate (Hz): 50
    #define KMX61_REG_CNTL2_OWUF_100                (0x07)      // Output Data Rate (Hz): 100

/**************************************************************************
    OUTPUT DATA CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define KMX61_REG_ODCNTL_OSM_MASK               (0xF0)      // Rate at Which Data Samples from the Magnetometer will be Updated
    #define KMX61_REG_ODCNTL_OSM_12_5               (0x00)      // Output Data Rate (Hz): 12.5
    #define KMX61_REG_ODCNTL_OSM_25                 (0x10)      // Output Data Rate (Hz): 25
    #define KMX61_REG_ODCNTL_OSM_50                 (0x20)      // Output Data Rate (Hz): 50
    #define KMX61_REG_ODCNTL_OSM_100                (0x30)      // Output Data Rate (Hz): 100
    #define KMX61_REG_ODCNTL_OSM_200                (0x40)      // Output Data Rate (Hz): 200
    #define KMX61_REG_ODCNTL_OSM_400                (0x50)      // Output Data Rate (Hz): 400
    #define KMX61_REG_ODCNTL_OSM_800                (0x60)      // Output Data Rate (Hz): 800
    #define KMX61_REG_ODCNTL_OSM_1600               (0x70)      // Output Data Rate (Hz): 1600
    #define KMX61_REG_ODCNTL_OSM_0_781              (0x80)      // Output Data Rate (Hz): 0.781
    #define KMX61_REG_ODCNTL_OSM_1_583              (0x90)      // Output Data Rate (Hz): 1.583
    #define KMX61_REG_ODCNTL_OSM_3_125              (0xA0)      // Output Data Rate (Hz): 3.125
    #define KMX61_REG_ODCNTL_OSM_6_25               (0xB0)      // Output Data Rate (Hz): 6.25

    #define KMX61_REG_ODCNTL_OSA_MASK               (0x0F)      // Rate at Which Data Samples from the Accelerometer will be Updated
    #define KMX61_REG_ODCNTL_OSA_12_5               (0x00)      // Output Data Rate (Hz): 12.5
    #define KMX61_REG_ODCNTL_OSA_25                 (0x01)      // Output Data Rate (Hz): 25
    #define KMX61_REG_ODCNTL_OSA_50                 (0x02)      // Output Data Rate (Hz): 50
    #define KMX61_REG_ODCNTL_OSA_100                (0x03)      // Output Data Rate (Hz): 100
    #define KMX61_REG_ODCNTL_OSA_200                (0x04)      // Output Data Rate (Hz): 200
    #define KMX61_REG_ODCNTL_OSA_400                (0x05)      // Output Data Rate (Hz): 400
    #define KMX61_REG_ODCNTL_OSA_800                (0x06)      // Output Data Rate (Hz): 800
    #define KMX61_REG_ODCNTL_OSA_1600               (0x07)      // Output Data Rate (Hz): 1600
    #define KMX61_REG_ODCNTL_OSA_0_781              (0x08)      // Output Data Rate (Hz): 0.781
    #define KMX61_REG_ODCNTL_OSA_1_583              (0x09)      // Output Data Rate (Hz): 1.583
    #define KMX61_REG_ODCNTL_OSA_3_125              (0x0A)      // Output Data Rate (Hz): 3.125
    #define KMX61_REG_ODCNTL_OSA_6_25               (0x0B)      // Output Data Rate (Hz): 6.25




typedef enum
{
    ACT_OPERATING               = KMX61_REG_STBY_REG_ACT_STBY_0,
    ACT_STANDBY                 = KMX61_REG_STBY_REG_ACT_STBY_1
    
} kmxActStandby_t;

typedef enum
{
    MAG_OPERATING               = KMX61_REG_STBY_REG_MAG_STBY_0,
    MAG_STANDBY                 = KMX61_REG_STBY_REG_MAG_STBY_1
    
} kmxMagStandby_t;

typedef enum
{
    ACCEL_OPERATING             = KMX61_REG_STBY_REG_ACCEL_STBY_0,
    ACCEL_STANDBY               = KMX61_REG_STBY_REG_ACCEL_STBY_1
    
} kmxAccelStandby_t;

typedef enum
{
    BTSE_DISABLE                = KMX61_REG_CNTL1_BTSE_DISABLE,
    BTSE_ENABLE                 = KMX61_REG_CNTL1_BTSE_ENABLE
    
} kmxSleepEngine_t;

typedef enum
{
    WUFE_DISABLE                = KMX61_REG_CNTL1_WUFE_DISABLE,
    WUFE_ENABLE                 = KMX61_REG_CNTL1_WUFE_ENABLE
    
} kmxWakeEngine_t;

typedef enum
{
    ACCEL_RANGE_2G              = KMX61_REG_CNTL1_GSEL_2G,
    ACCEL_RANGE_4G              = KMX61_REG_CNTL1_GSEL_4G,
    ACCEL_RANGE_8G              = KMX61_REG_CNTL1_GSEL_8G,
    ACCEL_RANGE_8G_14BIT        = KMX61_REG_CNTL1_GSEL_8G_14BIT
    
} kmxAccelRange_t;

typedef enum
{
    MAG_DATA_RATE_12_5HZ        = KMX61_REG_ODCNTL_OSM_12_5,
    MAG_DATA_RATE_25HZ          = KMX61_REG_ODCNTL_OSM_25,
    MAG_DATA_RATE_50HZ          = KMX61_REG_ODCNTL_OSM_50,
    MAG_DATA_RATE_100HZ         = KMX61_REG_ODCNTL_OSM_100,
    MAG_DATA_RATE_200HZ         = KMX61_REG_ODCNTL_OSM_200,
    MAG_DATA_RATE_400HZ         = KMX61_REG_ODCNTL_OSM_400,
    MAG_DATA_RATE_800HZ         = KMX61_REG_ODCNTL_OSM_800,
    MAG_DATA_RATE_1600HZ        = KMX61_REG_ODCNTL_OSM_1600,
    MAG_DATA_RATE_0_781HZ       = KMX61_REG_ODCNTL_OSM_0_781,
    MAG_DATA_RATE_1_583HZ       = KMX61_REG_ODCNTL_OSM_1_583,
    MAG_DATA_RATE_3_125HZ       = KMX61_REG_ODCNTL_OSM_3_125,
    MAG_DATA_RATE_6_25HZ        = KMX61_REG_ODCNTL_OSM_6_25
    
} kmxMagDataRate_t;

typedef enum
{
    ACCEL_DATA_RATE_12_5HZ      = KMX61_REG_ODCNTL_OSA_12_5,
    ACCEL_DATA_RATE_25HZ        = KMX61_REG_ODCNTL_OSA_25,
    ACCEL_DATA_RATE_50HZ        = KMX61_REG_ODCNTL_OSA_50,
    ACCEL_DATA_RATE_100HZ       = KMX61_REG_ODCNTL_OSA_100,
    ACCEL_DATA_RATE_200HZ       = KMX61_REG_ODCNTL_OSA_200,
    ACCEL_DATA_RATE_400HZ       = KMX61_REG_ODCNTL_OSA_400,
    ACCEL_DATA_RATE_800HZ       = KMX61_REG_ODCNTL_OSA_800,
    ACCEL_DATA_RATE_1600HZ      = KMX61_REG_ODCNTL_OSA_1600,
    ACCEL_DATA_RATE_0_781HZ     = KMX61_REG_ODCNTL_OSA_0_781,
    ACCEL_DATA_RATE_1_583HZ     = KMX61_REG_ODCNTL_OSA_1_583,
    ACCEL_DATA_RATE_3_125HZ     = KMX61_REG_ODCNTL_OSA_3_125,
    ACCEL_DATA_RATE_6_25HZ      = KMX61_REG_ODCNTL_OSA_6_25
    
} kmxAccelDataRate_t;


typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    float   T;
    
} kmxSensorData_t;


class KMX61
{
    protected:
        // Instance-specific properties
        uint8_t kmx_conversionDelay;
        kmxActStandby_t kmx_actstandby;
        kmxMagStandby_t kmx_magstandby;
        kmxAccelStandby_t kmx_accelstandby;
        kmxSleepEngine_t kmx_sleepengine;
        kmxWakeEngine_t kmx_wakeengine;
        kmxAccelRange_t kmx_accelrange;
        kmxMagDataRate_t kmx_magdatarate;
        kmxAccelDataRate_t kmx_acceldatarate;

    public:
        uint8_t kmx_i2cAddress;
        kmxSensorData_t kmx_accelData;
        kmxSensorData_t kmx_magData;
        kmxSensorData_t kmx_tempData;
        void getAddr_KMX61(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpSensor(void);
        void Measure_Accelerometer(void);
        void Measure_Magnetometer(void);
        void setActStandby(kmxActStandby_t actstandby);
        kmxActStandby_t getActStandby(void);
        void setMagStandby(kmxMagStandby_t magstandby);
        kmxMagStandby_t getMagStandby(void);
        void setAccelStandby(kmxAccelStandby_t accelstandby);
        kmxAccelStandby_t getAccelStandby(void);
        void setSleepEngine(kmxSleepEngine_t sleepengine);
        kmxSleepEngine_t getSleepEngine(void);
        void setWakeEngine(kmxWakeEngine_t wakeengine);
        kmxWakeEngine_t getWakeEngine(void);
        void setAccelRange(kmxAccelRange_t accelrange);
        kmxAccelRange_t getAccelRange(void);
        void setMagDataRate(kmxMagDataRate_t magdatarate);
        kmxMagDataRate_t getMagDataRate(void);
        void setAccelDataRate(kmxAccelDataRate_t acceldatarate);
        kmxAccelDataRate_t getAccelDataRate(void);
    
    private:
};
