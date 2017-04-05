/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HTS221
        This code is designed to work with the HTS221_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/?sku=HTS221_I2CS#tabs-0-product_tabset-2
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
    #define HTS221_DEFAULT_ADDRESS                          (0x5F)
    #define HTS221_HUMTEMP_DEV_ID                           (0xBC)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define HTS221_CONVERSIONDELAY                          (100)

/**************************************************************************
     HUMIDITY AND TEMPERATURE SENSOR REGISTERS
**************************************************************************/
    #define HTS221_REG_HUMTEMP_WHO_AM_I                     (0x0F)      // Device Identification Register
    #define HTS221_REG_HUMTEMP_RES_CONF                     (0x10)      // Relative Humidity and Temperature Average Mode
    #define HTS221_REG_HUMTEMP_CTRL_REG1                    (0x20)      // Control Register 1
    #define HTS221_REG_HUMTEMP_CTRL_REG2                    (0x21)      // Control Register 2
    #define HTS221_REG_HUMTEMP_CTRL_REG3                    (0x22)      // Control Register 3
    #define HTS221_REG_HUMTEMP_STATUS                       (0x27)      // Status Register
    #define HTS221_REG_HUMTEMP_HUMIDITY_OUT_L               (0x28)      // Humidity Output Value Low Register
    #define HTS221_REG_HUMTEMP_HUMIDITY_OUT_H               (0x29)      // Humidity Output Value High Register
    #define HTS221_REG_HUMTEMP_TEMP_OUT_L                   (0x2A)      // Temperature Output Value Low Register
    #define HTS221_REG_HUMTEMP_TEMP_OUT_H                   (0x2B)      // Temperature Output Value High Register
    #define HTS221_REG_HUMTEMP_H0_rH_x2                     (0x30)      // Calibration Register
    #define HTS221_REG_HUMTEMP_H1_rH_x2                     (0x31)      // Calibration Register
    #define HTS221_REG_HUMTEMP_T0_degC_x8                   (0x32)      // Calibration Register
    #define HTS221_REG_HUMTEMP_T1_degC_x8                   (0x33)      // Calibration Register
    #define HTS221_REG_HUMTEMP_T1_T0_msb                    (0x35)      // Calibration Register
    #define HTS221_REG_HUMTEMP_H0_T0_OUT_L                  (0x36)      // Calibration Register
    #define HTS221_REG_HUMTEMP_H0_T0_OUT_H                  (0x37)      // Calibration Register
    #define HTS221_REG_HUMTEMP_H1_T0_OUT_L                  (0x3A)      // Calibration Register
    #define HTS221_REG_HUMTEMP_H1_T0_OUT_H                  (0x3B)      // Calibration Register
    #define HTS221_REG_HUMTEMP_T0_OUT_L                     (0x3C)      // Calibration Register
    #define HTS221_REG_HUMTEMP_T0_OUT_H                     (0x3D)      // Calibration Register
    #define HTS221_REG_HUMTEMP_T1_OUT_L                     (0x3E)      // Calibration Register
    #define HTS221_REG_HUMTEMP_T1_OUT_H                     (0x3F)      // Calibration Register

/**************************************************************************
     HUMIDITY AND TEMPERATURE RESOLUTION MODE REGISTER DESCRIPTION
**************************************************************************/
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGT_MASK           (0x38)      // Numbers of Averaged Temperature Samples
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGT_2              (0x00)      // Temperature (AVGT): 2
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGT_4              (0x08)      // Temperature (AVGT): 4
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGT_8              (0x10)      // Temperature (AVGT): 8
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGT_16             (0x18)      // Temperature (AVGT): 16
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGT_32             (0x20)      // Temperature (AVGT): 32
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGT_64             (0x28)      // Temperature (AVGT): 64
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGT_128            (0x30)      // Temperature (AVGT): 128
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGT_256            (0x38)      // Temperature (AVGT): 256

    #define HTS221_REG_HUMTEMP_RES_CONF_AVGH_MASK           (0x07)      // Numbers of Averaged Humidity Samples
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGH_4              (0x00)      // Humidity (AVGH): 4
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGH_8              (0x01)      // Humidity (AVGH): 8
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGH_16             (0x02)      // Humidity (AVGH): 16
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGH_32             (0x03)      // Humidity (AVGH): 32
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGH_64             (0x04)      // Humidity (AVGH): 64
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGH_128            (0x05)      // Humidity (AVGH): 128
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGH_256            (0x06)      // Humidity (AVGH): 256
    #define HTS221_REG_HUMTEMP_RES_CONF_AVGH_512            (0x07)      // Humidity (AVGH): 512

/**************************************************************************
     HUMIDITY AND TEMPERATURE CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define HTS221_REG_HUMTEMP_CTRL_REG1_PD_MASK            (0x80)      // Power-Down Mode Enable
    #define HTS221_REG_HUMTEMP_CTRL_REG1_PD_DOWN            (0x00)      // Power-Down Mode
    #define HTS221_REG_HUMTEMP_CTRL_REG1_PD_ACTIVE          (0x80)      // Active Mode

    #define HTS221_REG_HUMTEMP_CTRL_REG1_BDU_MASK           (0x04)      // Block Data Update
    #define HTS221_REG_HUMTEMP_CTRL_REG1_BDU_CONTINUOUS     (0x00)      // Continuous Update
    #define HTS221_REG_HUMTEMP_CTRL_REG1_BDU_NOTUPDATE      (0x04)      // Output Registers Not Updated until MSB and LSB Read

    #define HTS221_REG_HUMTEMP_CTRL_REG1_ODR_MASK           (0x03)      // Output Data Rate Selection
    #define HTS221_REG_HUMTEMP_CTRL_REG1_ODR_ONE_SHOT       (0x00)      // One-Shot
    #define HTS221_REG_HUMTEMP_CTRL_REG1_ODR_1HZ            (0x01)      // Humidity (Hz) & Temperature (Hz): 1 Hz
    #define HTS221_REG_HUMTEMP_CTRL_REG1_ODR_7HZ            (0x02)      // Humidity (Hz) & Temperature (Hz): 7 Hz
    #define HTS221_REG_HUMTEMP_CTRL_REG1_ODR_12_5HZ         (0x03)      // Humidity (Hz) & Temperature (Hz): 12.5 Hz

/**************************************************************************
    HUMIDITY AND TEMPERATURE CONTROL REGISTER 2 DESCRIPTION
**************************************************************************/
    #define HTS221_REG_HUMTEMP_CTRL_REG2_BOOT_MASK          (0x80)      // Reboot Memory Content
    #define HTS221_REG_HUMTEMP_CTRL_REG2_BOOT_NORMAL        (0x00)      // Normal Mode
    #define HTS221_REG_HUMTEMP_CTRL_REG2_BOOT_REBOOT        (0x80)      // Reboot Memory Content

    #define HTS221_REG_HUMTEMP_CTRL_REG2_HEATER_MASK        (0x02)      // Heater
    #define HTS221_REG_HUMTEMP_CTRL_REG2_HEATER_DISABLE     (0x00)      // Disable Heater
    #define HTS221_REG_HUMTEMP_CTRL_REG2_HEATER_ENABLE      (0x02)      // Enable Heater

    #define HTS221_REG_HUMTEMP_CTRL_REG2_ONE_SHOT_MASK      (0x01)      // One-Shot Enable
    #define HTS221_REG_HUMTEMP_CTRL_REG2_ONE_SHOT_WAITING   (0x00)      // Waiting for Start of Conversion
    #define HTS221_REG_HUMTEMP_CTRL_REG2_ONE_SHOT_START     (0x01)      // Start for a New Dataset


typedef enum
{
    HTS221_AVGT_2                   = HTS221_REG_HUMTEMP_RES_CONF_AVGT_2,
    HTS221_AVGT_4                   = HTS221_REG_HUMTEMP_RES_CONF_AVGT_4,
    HTS221_AVGT_8                   = HTS221_REG_HUMTEMP_RES_CONF_AVGT_8,
    HTS221_AVGT_16                  = HTS221_REG_HUMTEMP_RES_CONF_AVGT_16,
    HTS221_AVGT_32                  = HTS221_REG_HUMTEMP_RES_CONF_AVGT_32,
    HTS221_AVGT_64                  = HTS221_REG_HUMTEMP_RES_CONF_AVGT_64,
    HTS221_AVGT_128                 = HTS221_REG_HUMTEMP_RES_CONF_AVGT_128,
    HTS221_AVGT_256                 = HTS221_REG_HUMTEMP_RES_CONF_AVGT_256
    
} htsTempAverage_t;

typedef enum
{
    HTS221_AVGH_4                   = HTS221_REG_HUMTEMP_RES_CONF_AVGH_4,
    HTS221_AVGH_8                   = HTS221_REG_HUMTEMP_RES_CONF_AVGH_8,
    HTS221_AVGH_16                  = HTS221_REG_HUMTEMP_RES_CONF_AVGH_16,
    HTS221_AVGH_32                  = HTS221_REG_HUMTEMP_RES_CONF_AVGH_32,
    HTS221_AVGH_64                  = HTS221_REG_HUMTEMP_RES_CONF_AVGH_64,
    HTS221_AVGH_128                 = HTS221_REG_HUMTEMP_RES_CONF_AVGH_128,
    HTS221_AVGH_256                 = HTS221_REG_HUMTEMP_RES_CONF_AVGH_256,
    HTS221_AVGH_512                 = HTS221_REG_HUMTEMP_RES_CONF_AVGH_512
    
} htsPresAverage_t;

typedef enum
{
    HUMTEMP_POWER_DOWN_MODE         = HTS221_REG_HUMTEMP_CTRL_REG1_PD_DOWN,
    HUMTEMP_ACTIVE_MODE             = HTS221_REG_HUMTEMP_CTRL_REG1_PD_ACTIVE
    
} htsMode_t;

typedef enum
{
    HUMTEMP_CONTINUOUS              = HTS221_REG_HUMTEMP_CTRL_REG1_BDU_CONTINUOUS,
    HUMTEMP_NOT_UPDATED             = HTS221_REG_HUMTEMP_CTRL_REG1_BDU_NOTUPDATE
    
} htsBlockData_t;

typedef enum
{
    HUMTEMP_DATARATE_ONE_SHOT       = HTS221_REG_HUMTEMP_CTRL_REG1_ODR_ONE_SHOT,
    HUMTEMP_DATARATE_1HZ            = HTS221_REG_HUMTEMP_CTRL_REG1_ODR_1HZ,
    HUMTEMP_DATARATE_7HZ            = HTS221_REG_HUMTEMP_CTRL_REG1_ODR_7HZ,
    HUMTEMP_DATARATE_12_5HZ         = HTS221_REG_HUMTEMP_CTRL_REG1_ODR_12_5HZ
    
} htsDataRate_t;

typedef enum
{
    HUMTEMP_BOOT_NORMAL             = HTS221_REG_HUMTEMP_CTRL_REG2_BOOT_NORMAL,
    HUMTEMP_BOOT_REBOOT             = HTS221_REG_HUMTEMP_CTRL_REG2_BOOT_REBOOT
    
} htsBoot_t;

typedef enum
{
    HUMTEMP_HEATER_DISABLE          = HTS221_REG_HUMTEMP_CTRL_REG2_HEATER_DISABLE,
    HUMTEMP_HEATER_ENABLE           = HTS221_REG_HUMTEMP_CTRL_REG2_HEATER_ENABLE
    
} htsHeater_t;

typedef enum
{
    HUMTEMP_ONE_SHOT_WAITING        = HTS221_REG_HUMTEMP_CTRL_REG2_ONE_SHOT_WAITING,
    HUMTEMP_ONE_SHOT_START          = HTS221_REG_HUMTEMP_CTRL_REG2_ONE_SHOT_START
    
} htsOneShot_t;


typedef struct
{
    uint8_t H0_rH_x2;
    uint8_t H1_rH_x2;
    uint16_t T0_degC_x8;
    uint16_t T1_degC_x8;
    uint8_t T1_T0_msb;
    uint16_t H0_T0_OUT;
    uint16_t H1_T0_OUT;
    uint16_t T0_OUT;
    uint16_t T1_OUT;
    
} htsCalibData_t;


typedef struct
{
    float H;
    float T;
    
} htsSensorData_t;


class HTS221
{
    protected:
        // Instance-specific properties
        uint8_t hts_conversionDelay;
        htsTempAverage_t hts_tempaverage;
        htsPresAverage_t hts_presaverage;
        htsMode_t hts_mode;
        htsBlockData_t hts_blockdata;
        htsDataRate_t hts_datarate;
        htsBoot_t hts_boot;
        htsHeater_t hts_heater;
        htsOneShot_t hts_oneshot;

    public:
        uint8_t hts_i2cAddress;
        htsSensorData_t hts_sensorData;
        htsCalibData_t hts_calibdata;
        void readCoefficients(void);
        void getAddr_HTS221(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpSensor(void);
        void Measure_Humidity(void);
        void Measure_Temperature(void);
        void setTempAverage(htsTempAverage_t tempaverage);
        htsTempAverage_t getTempAverage(void);
        void setPresAverage(htsPresAverage_t presaverage);
        htsPresAverage_t getPresAverage(void);
        void setMode(htsMode_t mode);
        htsMode_t getMode(void);
        void setBlockData(htsBlockData_t blockdata);
        htsBlockData_t getBlockData(void);
        void setDataRate(htsDataRate_t datarate);
        htsDataRate_t getDataRate(void);
        void setBoot(htsBoot_t boot);
        htsBoot_t getBoot(void);
        void setHeater(htsHeater_t heater);
        htsHeater_t getHeater(void);
        void setOneShot(htsOneShot_t oneshot);
        htsOneShot_t getOneShot(void);

    private:
};
