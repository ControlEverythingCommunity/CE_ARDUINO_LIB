/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LPS25HB
        This code is designed to work with the LPS25HB_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Barometer?sku=LPS25HB_I2CS#tabs-0-product_tabset-2
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
    #define LPS25HB_DEFAULT_ADDRESS_BARO                    (0x5C)      // 1011100b
    #define LPS25HB_ADDRESS_BARO_UPDATED                    (0x5D)      // 1011101b
    #define LPS25HB_BARO_DEV_ID                             (0xBD)      // 10111101

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define LPS25HB_CONVERSIONDELAY                         (100)

/**************************************************************************
    BAROMETER REGISTERS
**************************************************************************/
    #define LPS25HB_REG_BARO_REF_PRES_LSB                   (0x08)      // Reference Pressure (LSB Data)
    #define LPS25HB_REG_BARO_REF_PRES_MIDDLE                (0x09)      // Reference Pressure (Middle Part)
    #define LPS25HB_REG_BARO_REF_PRES_MSB                   (0x0A)      // Reference Pressure (MSB Data)
    #define LPS25HB_REG_BARO_WHO_AM_I                       (0x0F)      // Device Identification Register
    #define LPS25HB_REG_BARO_RES_CONFIG                     (0x10)      // Pressure and Temperature Resolution
    #define LPS25HB_REG_BARO_CTRL_REG1                      (0x20)      // Barometer Control Register 1
    #define LPS25HB_REG_BARO_CTRL_REG2                      (0x21)      // Barometer Control Register 2
    #define LPS25HB_REG_BARO_CTRL_REG3                      (0x22)      // Barometer Interrupt Control Register
    #define LPS25HB_REG_BARO_CTRL_REG4                      (0x23)      // Barometer Interrupt Configuration Register
    #define LPS25HB_REG_BARO_INTERRUPT_CFG                  (0x24)      // Barometer Interrupt Configuration Register
    #define LPS25HB_REG_BARO_INT_SOURCE                     (0x25)      // Interrupt Source Register
    #define LPS25HB_REG_BARO_STATUS                         (0x27)      // Status Register
    #define LPS25HB_REG_BARO_PRESS_OUT_XL                   (0x28)      // Pressure Output Value Low Register
    #define LPS25HB_REG_BARO_PRESS_OUT_L                    (0x29)      // Pressure Output Value Mid Register
    #define LPS25HB_REG_BARO_PRESS_OUT_H                    (0x2A)      // Pressure Output Value High Register
    #define LPS25HB_REG_BARO_TEMP_OUT_L                     (0x2B)      // Temperature Output Value Low Register
    #define LPS25HB_REG_BARO_TEMP_OUT_H                     (0x2C)      // Temperature Output Value High Register
    #define LPS25HB_REG_BARO_FIFO_CTRL                      (0x2E)      // FIFO Control Register
    #define LPS25HB_REG_BARO_FIFO_STATUS                    (0x2F)      // FIFO Status Register
    #define LPS25HB_REG_BARO_THS_P_L                        (0x30)      // Interrupt Pressure Low Threshold Register
    #define LPS25HB_REG_BARO_THS_P_H                        (0x31)      // Interrupt Pressure High Threshold Register
    #define LPS25HB_REG_BARO_RPDS_L                         (0x39)      // Pressure Offset Low Register
    #define LPS25HB_REG_BARO_RPDS_H                         (0x3A)      // Pressure Offset High Register

/**************************************************************************
    BAROMETER PRESSURE AND TEMPERATURE RESOLUTION REGISTER DESCRIPTION
**************************************************************************/
    #define LPS25HB_REG_BARO_RES_CONF_TEMP_MASK             (0x0C)      // Temperature Resolution Configuration
    #define LPS25HB_REG_BARO_RES_CONF_TEMP_8                (0x00)      // Nr. Internal Average: 8
    #define LPS25HB_REG_BARO_RES_CONF_TEMP_16               (0x04)      // Nr. Internal Average: 16
    #define LPS25HB_REG_BARO_RES_CONF_TEMP_32               (0x08)      // Nr. Internal Average: 32
    #define LPS25HB_REG_BARO_RES_CONF_TEMP_64               (0x0C)      // Nr. Internal Average: 64

    #define LPS25HB_REG_BARO_RES_CONF_PRES_MASK             (0x03)      // Pressure Resolution Configuration
    #define LPS25HB_REG_BARO_RES_CONF_PRES_8                (0x00)      // Nr. Internal Average: 8
    #define LPS25HB_REG_BARO_RES_CONF_PRES_32               (0x01)      // Nr. Internal Average: 32
    #define LPS25HB_REG_BARO_RES_CONF_PRES_128              (0x02)      // Nr. Internal Average: 128
    #define LPS25HB_REG_BARO_RES_CONF_PRES_512              (0x03)      // Nr. Internal Average: 512

/**************************************************************************
    BAROMETER CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define LPS25HB_REG_BARO_CTRL_REG1_PD_MASK              (0x80)      // Power-Down Mode Enable
    #define LPS25HB_REG_BARO_CTRL_REG1_PD_DOWN              (0x00)      // Power-Down Mode
    #define LPS25HB_REG_BARO_CTRL_REG1_PD_ACTIVE            (0x80)      // Active Mode

    #define LPS25HB_REG_BARO_CTRL_REG1_ODR_MASK             (0x70)      // Output Data Rate Bit Configuration
    #define LPS25HB_REG_BARO_CTRL_REG1_ODR_ONE_SHOT         (0x00)      // One-Shot Mode Enabled
    #define LPS25HB_REG_BARO_CTRL_REG1_ODR_1HZ              (0x10)      // Pressure (Hz): 1, Temperature (Hz): 1
    #define LPS25HB_REG_BARO_CTRL_REG1_ODR_7HZ              (0x20)      // Pressure (Hz): 7, Temperature (Hz): 7
    #define LPS25HB_REG_BARO_CTRL_REG1_ODR_12_5HZ           (0x30)      // Pressure (Hz): 12.5, Temperature (Hz): 12.5
    #define LPS25HB_REG_BARO_CTRL_REG1_ODR_25HZ             (0x40)      // Pressure (Hz): 25, Temperature (Hz): 25

    #define LPS25HB_REG_BARO_CTRL_REG1_DIFF_EN_MASK         (0x08)      // Interrupt Generation Enable
    #define LPS25HB_REG_BARO_CTRL_REG1_DIFF_EN_DISABLE      (0x00)      // Interrupt Generation Disabled
    #define LPS25HB_REG_BARO_CTRL_REG1_DIFF_EN_ENABLE       (0x08)      // Interrupt Generation Enabled

    #define LPS25HB_REG_BARO_CTRL_REG1_BDU_MASK             (0x04)      // Block Data Update
    #define LPS25HB_REG_BARO_CTRL_REG1_BDU_CONTINUOUS       (0x00)      // Continuous Update
    #define LPS25HB_REG_BARO_CTRL_REG1_BDU_NOTUPDATE        (0x04)      // Output Registers Not Updated until MSB and LSB Read

    #define LPS25HB_REG_BARO_CTRL_REG1_RESET_AZ_MASK        (0x02)      // Reset Autozero Function
    #define LPS25HB_REG_BARO_CTRL_REG1_RESET_AZ_NORMAL      (0x00)      // Normal Mode
    #define LPS25HB_REG_BARO_CTRL_REG1_RESET_AZ_RESET       (0x02)      // Reset Autozero Function

    #define LPS25HB_REG_BARO_CTRL_REG1_SIM_MASK             (0x01)      // SPI Serial Interface Mode Selection
    #define LPS25HB_REG_BARO_CTRL_REG1_SIM_4WIRE            (0x00)      // 4-Wire Interface
    #define LPS25HB_REG_BARO_CTRL_REG1_SIM_3WIRE            (0x01)      // 3-Wire Interface


typedef enum
{
    TEMP_RESOLUTION_8               = LPS25HB_REG_BARO_RES_CONF_TEMP_8,
    TEMP_RESOLUTION_16              = LPS25HB_REG_BARO_RES_CONF_TEMP_16,
    TEMP_RESOLUTION_32              = LPS25HB_REG_BARO_RES_CONF_TEMP_32,
    TEMP_RESOLUTION_64              = LPS25HB_REG_BARO_RES_CONF_TEMP_64
    
} lpsTempResolution_t;

typedef enum
{
    PRES_RESOLUTION_8               = LPS25HB_REG_BARO_RES_CONF_PRES_8,
    PRES_RESOLUTION_32              = LPS25HB_REG_BARO_RES_CONF_PRES_32,
    PRES_RESOLUTION_128             = LPS25HB_REG_BARO_RES_CONF_PRES_128,
    PRES_RESOLUTION_512             = LPS25HB_REG_BARO_RES_CONF_PRES_512
    
} lpsPresResolution_t;

typedef enum
{
    BARO_POWER_DOWN_MODE            = LPS25HB_REG_BARO_CTRL_REG1_PD_DOWN,
    BARO_ACTIVE_MODE                = LPS25HB_REG_BARO_CTRL_REG1_PD_ACTIVE
    
} lpsBaroMode_t;

typedef enum
{
    BARO_DATARATE_ONE_SHOT          = LPS25HB_REG_BARO_CTRL_REG1_ODR_ONE_SHOT,
    BARO_DATARATE_1HZ               = LPS25HB_REG_BARO_CTRL_REG1_ODR_1HZ,
    BARO_DATARATE_7HZ               = LPS25HB_REG_BARO_CTRL_REG1_ODR_7HZ,
    BARO_DATARATE_12_5HZ            = LPS25HB_REG_BARO_CTRL_REG1_ODR_12_5HZ,
    BARO_DATARATE_25HZ              = LPS25HB_REG_BARO_CTRL_REG1_ODR_25HZ
    
} lpsBaroDataRate_t;

typedef enum
{
    BARO_INTERRUPT_DISABLED         = LPS25HB_REG_BARO_CTRL_REG1_DIFF_EN_DISABLE,
    BARO_INTERRUPT_ENABLED          = LPS25HB_REG_BARO_CTRL_REG1_DIFF_EN_ENABLE
    
} lpsBaroInterrupt_t;

typedef enum
{
    BARO_CONTINUOUS                 = LPS25HB_REG_BARO_CTRL_REG1_BDU_CONTINUOUS,
    BARO_NOT_UPDATED                = LPS25HB_REG_BARO_CTRL_REG1_BDU_NOTUPDATE
    
} lpsBaroBlockData_t;

typedef enum
{
    BARO_AUTOZERO_NORMAL            = LPS25HB_REG_BARO_CTRL_REG1_RESET_AZ_NORMAL,
    BARO_AUTOZERO_RESET             = LPS25HB_REG_BARO_CTRL_REG1_RESET_AZ_RESET
    
} lpsBaroAutozero_t;

typedef enum
{
    BARO_SPI_WIRE_4                 = LPS25HB_REG_BARO_CTRL_REG1_SIM_4WIRE,
    BARO_SPI_WIRE_3                 = LPS25HB_REG_BARO_CTRL_REG1_SIM_3WIRE
    
} lpsBaroSPIWire_t;


typedef struct
{
    float P;
    float T;
    
} lpsSensorData_t;


class LPS25HB
{
    protected:
        // Instance-specific properties
        uint8_t lps_conversionDelay;
        lpsTempResolution_t lps_tempresolution;
        lpsPresResolution_t lps_presresolution;
        lpsBaroMode_t lps_baromode;
        lpsBaroDataRate_t lps_barodatarate;
        lpsBaroInterrupt_t lps_barointerrupt;
        lpsBaroBlockData_t lps_baroblockdata;
        lpsBaroAutozero_t lps_baroautozero;
        lpsBaroSPIWire_t lps_barospiwire;

    public:
        uint8_t lps_i2cAddress;
        lpsSensorData_t lps_baroData;
        void getAddr_LPS25HB(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpBarometer(void);
        void Measure_Barometer(void);
        void Measure_Temperature(void);
        void setTempResolution(lpsTempResolution_t tempresolution);
        lpsTempResolution_t getTempResolution(void);
        void setPresResolution(lpsPresResolution_t presresolution);
        lpsPresResolution_t getPresResolution(void);
        void setBaroMode(lpsBaroMode_t baromode);
        lpsBaroMode_t getBaroMode(void);
        void setBaroDataRate(lpsBaroDataRate_t barodatarate);
        lpsBaroDataRate_t getBaroDataRate(void);
        void setBaroInterrupt(lpsBaroInterrupt_t barointerrupt);
        lpsBaroInterrupt_t getBaroInterrupt(void);
        void setBaroBlockData(lpsBaroBlockData_t baroblockdata);
        lpsBaroBlockData_t getBaroBlockData(void);
        void setBaroAutozero(lpsBaroAutozero_t baroautozero);
        lpsBaroAutozero_t getBaroAutozero(void);
        void setBaroSPIWire(lpsBaroSPIWire_t barospiwire);
        lpsBaroSPIWire_t getBaroSPIWire(void);
    
    private:
};
