/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MPL115A2
        This code is designed to work with the MPL115A2_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/collections/barometers/products/mpl115a2-digital-barometer-500-to-1150-mbar
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
    #define MPL115A2_ADDRESS_DEFAULT            (0x60)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define MPL115A2_CONVERSIONDELAY            (100)

/**************************************************************************
    COMMAND REGISTER
**************************************************************************/
    #define MPL115A2_REG_P_ADC_MSB              (0x00)      // 10-bit Pressure ADC Output Value MSB Register
    #define MPL115A2_REG_P_ADC_LSB              (0x01)      // 10-bit Pressure ADC Output Value LSB Register
    #define MPL115A2_REG_T_ADC_MSB              (0x02)      // 10-bit Temperature ADC Output Value MSB Register
    #define MPL115A2_REG_T_ADC_LSB              (0x03)      // 10-bit Temperature ADC Output Value LSB Register
    #define MPL115A2_REG_A0_MSB                 (0x04)      // A0 Coefficient MSB Register
    #define MPL115A2_REG_A0_LSB                 (0x05)      // A0 Coefficient LSB Register
    #define MPL115A2_REG_B1_MSB                 (0x06)      // B1 Coefficient MSB Register
    #define MPL115A2_REG_B1_LSB                 (0x07)      // B1 Coefficient LSB Register
    #define MPL115A2_REG_B2_MSB                 (0x08)      // B2 Coefficient MSB Register
    #define MPL115A2_REG_B2_LSB                 (0x09)      // B2 Coefficient LSB Register
    #define MPL115A2_REG_C12_MSB                (0x0A)      // C12 Coefficient MSB Register
    #define MPL115A2_REG_C12_LSB                (0x0B)      // C12 Coefficient LSB Register
    #define MPL115A2_REG_START_CONVERSION       (0x12)      // Start Pressure and Temperature Conversion


typedef struct
{
    float P;
    float T;
    
} mplSensorData_t;


class MPL115A2
{
    protected:
        // Instance-specific properties
        uint8_t mpl_conversionDelay;

    public:
        uint8_t mpl_i2cAddress;
        mplSensorData_t mpl_sensorData;
        float a0, b1, b2, c12;
        void readCoefficients(void);
        void getAddr_MPL115A2(uint8_t i2cAddress);
        bool begin(void);
        void Measure_PressureTemperature(void);
    
    private:

};
