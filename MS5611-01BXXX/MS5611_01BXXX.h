/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MS5611-01BXXX
        This code is designed to work with the MS5611-01BXXX_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/variometer-10-to-1200-mbar?variant=25687736203#tabs-0-product_tabset-2
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
    #define MS5611_01BXXX_ADDRESS_LOW           (0x76)
    #define MS5611_01BXXX_ADDRESS_HIGH          (0x77)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define MS5611_01BXXX_CONVERSIONDELAY       (100)

/**************************************************************************
    COMMAND REGISTER
**************************************************************************/
    #define MS5611_01BXXX_CMD_ADC               (0x00)
    #define MS5611_01BXXX_CMD_RESET             (0x1E)

    #define MS5611_01BXXX_CMD_C0                (0xA0)  // Constant
    #define MS5611_01BXXX_CMD_C1                (0xA2)  // Pressure Sensitivity | SENST1
    #define MS5611_01BXXX_CMD_C2                (0xA4)  // Pressure Offset | OFFT1
    #define MS5611_01BXXX_CMD_C3                (0xA6)  // Temperature Coefficient of Pressure Sensitivity | TCS
    #define MS5611_01BXXX_CMD_C4                (0xA8)  // Temperature Coefficient of Pressure Offset | TCO
    #define MS5611_01BXXX_CMD_C5                (0xAA)  // Reference Temperature | TREF
    #define MS5611_01BXXX_CMD_C6                (0xAC)  // Temperature coefficient of the temperature | TEMPSENS
    #define MS5611_01BXXX_CMD_C7                (0xAE)  // Constant

    #define MS5611_01BXXX_CMD_D1_OSR_256        (0x40)  // Convert D1 (OSR=256)
    #define MS5611_01BXXX_CMD_D1_OSR_512        (0x42)  // Convert D1 (OSR=512)
    #define MS5611_01BXXX_CMD_D1_OSR_1024       (0x44)  // Convert D1 (OSR=1024)
    #define MS5611_01BXXX_CMD_D1_OSR_2048       (0x46)  // Convert D1 (OSR=2048)
    #define MS5611_01BXXX_CMD_D1_OSR_4096       (0x48)  // Convert D1 (OSR=4096)

    #define MS5611_01BXXX_CMD_D2_OSR_256        (0x50)  // Convert D2 (OSR=256)
    #define MS5611_01BXXX_CMD_D2_OSR_512        (0x52)  // Convert D2 (OSR=512)
    #define MS5611_01BXXX_CMD_D2_OSR_1024       (0x54)  // Convert D2 (OSR=1024)
    #define MS5611_01BXXX_CMD_D2_OSR_2048       (0x56)  // Convert D2 (OSR=2048)
    #define MS5611_01BXXX_CMD_D2_OSR_4096       (0x58)  // Convert D2 (OSR=4096)

typedef enum
{
    D1_OSR_256              = MS5611_01BXXX_CMD_D1_OSR_256,
    D1_OSR_512              = MS5611_01BXXX_CMD_D1_OSR_512,
    D1_OSR_1024             = MS5611_01BXXX_CMD_D1_OSR_1024,
    D1_OSR_2048             = MS5611_01BXXX_CMD_D1_OSR_2048,
    D1_OSR_4096             = MS5611_01BXXX_CMD_D1_OSR_4096
} msD1OSR_t;

typedef enum
{
    D2_OSR_256              = MS5611_01BXXX_CMD_D2_OSR_256,
    D2_OSR_512              = MS5611_01BXXX_CMD_D2_OSR_512,
    D2_OSR_1024             = MS5611_01BXXX_CMD_D2_OSR_1024,
    D2_OSR_2048             = MS5611_01BXXX_CMD_D2_OSR_2048,
    D2_OSR_4096             = MS5611_01BXXX_CMD_D2_OSR_4096
} msD2OSR_t;


class MS5611_01BXXX
{
    protected:
        // Instance-specific properties
        uint8_t ms_conversionDelay;
        msD1OSR_t ms_d1osr;
        msD2OSR_t ms_d2osr;

    public:
        uint8_t ms_i2cAddress;
        uint16_t C[6];
        int32_t T2;
        int64_t OFF2, SENS2;
        void reset(void);
        void initialize(void);
        void getAddr_MS5611_01BXXX(uint8_t i2cAddress);
        bool begin(void);
        uint32_t readTemperature(void);
        uint32_t readPressure(void);
        double Measure_Temperature(void);
        double Measure_Pressure(void);
        void setinitiatePressure(msD1OSR_t d1osr);
        msD1OSR_t getinitiatePressure(void);
        void setinitiateTemperature(msD2OSR_t d2osr);
        msD2OSR_t getinitiateTemperature(void);
    
    private:

};
