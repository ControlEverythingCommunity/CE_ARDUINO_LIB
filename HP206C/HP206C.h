/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HP206C
        This code is designed to work with the HP206C_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/barometer-and-altimeter-300-1200-mbar?variant=25687698891#tabs-0-product_tabset-2
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
    #define HP206C_DEFAULT_ADDRESS              (0x76)     // (0xEC)>>1
    #define HP206C_ADDRESS_UPDATED              (0x77)     // (0xEE)>>1

/**************************************************************************
    CONVERSION DELAY (in hp)
**************************************************************************/
    #define HP206C_CONVERSIONDELAY              (100)

/**************************************************************************
    COMMAND REGISTER
**************************************************************************/
    #define HP206C_CMD_SOFT_RESET               (0x06)
    #define HP206C_CMD_CONVERT                  (0x40)      // Convert the Sensor Output to the Digital Values
    #define HP206C_CMD_OSR_4096                 (0x00)      // Convert D1 (OSR=4096)
    #define HP206C_CMD_OSR_2048                 (0x04)      // OSR: 2048
    #define HP206C_CMD_OSR_1024                 (0x08)      // OSR: 1024
    #define HP206C_CMD_OSR_512                  (0x0C)      // OSR: 512
    #define HP206C_CMD_OSR_256                  (0x10)      // OSR: 256
    #define HP206C_CMD_OSR_128                  (0x14)      // OSR: 128
    #define HP206C_CMD_CHNL_PRESTEMP            (0x00)      // Sensor Pressure and Temperature Channel
    #define HP206C_CMD_CHNL_TEMP                (0x10)      // Sensor Temperature Channel
    #define HP206C_CMD_READ_PT                  (0x10)      // Read Temperature and Pressure Values
    #define HP206C_CMD_READ_AT                  (0x11)      // Read Temperature and Altitude Values
    #define HP206C_CMD_READ_P                   (0x30)      // Read Pressure Value Only
    #define HP206C_CMD_READ_A                   (0x31)      // Read Altitude Value Only
    #define HP206C_CMD_READ_T                   (0x32)      // Read Temperature Value Only
    #define HP206C_CMD_ANA_CAL                  (0x28)      // Re-Calibrate the Internal Analog Blocks
    #define HP206C_CMD_READ_REG                 (0x80)      // Read Out the Control Registers
    #define HP206C_CMD_WRITE_REG                (0xC0)      // Write Out the Control Registers


typedef enum
{
    OSR_4096                = HP206C_CMD_OSR_4096,
    OSR_2048                = HP206C_CMD_OSR_2048,
    OSR_1024                = HP206C_CMD_OSR_1024,
    OSR_512                 = HP206C_CMD_OSR_512,
    OSR_256                 = HP206C_CMD_OSR_256,
    OSR_128                 = HP206C_CMD_OSR_128
    
} hpOSR_t;


typedef struct
{
    float P;
    float A;
    float T;
    
} hpSensorData_t;


class HP206C
{
    protected:
        // Instance-specific properties
        uint8_t hp_conversionDelay;
        hpOSR_t hp_osr;

    public:
        uint8_t hp_i2cAddress;
        hpSensorData_t hp_sensorData;
        void Reset(void);
        void getAddr_HP206C(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void Measure_Pressure(void);
        void Measure_Altitude(void);
        void Measure_Temperature(void);
        void setOSR(hpOSR_t osr);
        hpOSR_t getOSR(void);
    
    private:

};
