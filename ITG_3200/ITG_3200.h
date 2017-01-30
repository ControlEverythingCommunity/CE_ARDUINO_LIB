/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ITG-3200
        This code is designed to work with the ITG-3200_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Gyro?sku=ITG-3200_I2CS#tabs-0-product_tabset-2
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
    #define ITG_3200_DEFAULT_ADDRESS                        (0x68)  // b1101000, pin 9 is logic low
    #define ITG_3200_HIGH_ADDRESS                           (0x69)  // b1101001, pin 9 is logic high


/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define ITG_3200_CONVERSIONDELAY                        (100)

/**************************************************************************
    REGISTERS DESCRIPTION
**************************************************************************/
    #define ITG_3200_REG_GYRO_WHO_AM_I                      (0x00)      // Who Am I
    #define ITG_3200_REG_GYRO_SMPLRT_DIV                    (0x15)      // Sample Rate Divider
    #define ITG_3200_REG_GYRO_DLPF_FS                       (0x16)      // DLPF, Full Scale
    #define ITG_3200_REG_GYRO_INT_CFG                       (0x17)      // Interrupt Configuration
    #define ITG_3200_REG_GYRO_INT_STATUS                    (0x1A)      // Interrupt Status
    #define ITG_3200_REG_GYRO_TEMP_OUT_H                    (0x1B)      // Temperature High Data Output
    #define ITG_3200_REG_GYRO_TEMP_OUT_L                    (0x1C)      // Temperature Low Data Output
    #define ITG_3200_REG_GYRO_GYRO_XOUT_H                   (0x1D)      // Gyro X High Data Output
    #define ITG_3200_REG_GYRO_GYRO_XOUT_L                   (0x1E)      // Gyro X Low Data Output
    #define ITG_3200_REG_GYRO_GYRO_YOUT_H                   (0x1F)      // Gyro Y High Data Output
    #define ITG_3200_REG_GYRO_GYRO_YOUT_L                   (0x20)      // Gyro Y Low Data Output
    #define ITG_3200_REG_GYRO_GYRO_ZOUT_H                   (0x21)      // Gyro Z High Data Output
    #define ITG_3200_REG_GYRO_GYRO_ZOUT_L                   (0x22)      // Gyro Z Low Data Output
    #define ITG_3200_REG_GYRO_GYRO_PWR_MGM                  (0x3E)      // Power Management

/**************************************************************************
    DLPF, FULL SCALE REGISTER
**************************************************************************/
    #define ITG_3200_REG_GYRO_DLPF_FS_SEL_MASK              (0x18)      // Allows in Setting the Full-Scale Range of the Gyro Sensors
    #define ITG_3200_REG_GYRO_DLPF_FS_SEL_2000              (0x18)      // ±2000°/sec

    #define ITG_3200_REG_GYRO_DLPF_CFG_MASK                 (0x07)      // Sets the Digital Low Pass Filter Configuration
    #define ITG_3200_REG_GYRO_DLPF_CFG_256                  (0x00)      // Low Pass Filter Bandwidth: 256 Hz, Internal Sample Rate: 8 kHz
    #define ITG_3200_REG_GYRO_DLPF_CFG_188                  (0x01)      // Low Pass Filter Bandwidth: 188 Hz, Internal Sample Rate: 1 kHz
    #define ITG_3200_REG_GYRO_DLPF_CFG_98                   (0x02)      // Low Pass Filter Bandwidth: 98 Hz, Internal Sample Rate: 1 kHz
    #define ITG_3200_REG_GYRO_DLPF_CFG_42                   (0x03)      // Low Pass Filter Bandwidth: 42 Hz, Internal Sample Rate: 1 kHz
    #define ITG_3200_REG_GYRO_DLPF_CFG_20                   (0x04)      // Low Pass Filter Bandwidth: 20 Hz, Internal Sample Rate: 1 kHz
    #define ITG_3200_REG_GYRO_DLPF_CFG_10                   (0x05)      // Low Pass Filter Bandwidth: 10 Hz, Internal Sample Rate: 1 kHz
    #define ITG_3200_REG_GYRO_DLPF_CFG_5                    (0x06)      // Low Pass Filter Bandwidth: 5 Hz, Internal Sample Rate: 1 kHz

/**************************************************************************
    INTERRUPT CONFIGURATION REGISTER
**************************************************************************/
    #define ITG_3200_REG_GYRO_INTCNF_ACTL_MASK              (0x80)      // Logic level for INT Output Pin
    #define ITG_3200_REG_GYRO_INTCNF_ACTL_HIGH              (0x00)      // Active High
    #define ITG_3200_REG_GYRO_INTCNF_ACTL_LOW               (0x80)      // Active Low

    #define ITG_3200_REG_GYRO_INTCNF_OPEN_MASK              (0x40)      // Drive Type for INT Output Pin
    #define ITG_3200_REG_GYRO_INTCNF_OPEN_PUSH_PULL         (0x00)      // Push-Pull
    #define ITG_3200_REG_GYRO_INTCNF_OPEN_OPEN              (0x40)      // Open Drain

    #define ITG_3200_REG_GYRO_INTCNF_LATCH_INT_EN_MASK      (0x20)      // Latch Mode
    #define ITG_3200_REG_GYRO_INTCNF_LATCH_INT_EN_50US      (0x00)      // 50us Pulse
    #define ITG_3200_REG_GYRO_INTCNF_LATCH_INT_EN_LATCH     (0x20)      // Latch until Interrupt is Cleared

    #define ITG_3200_REG_GYRO_INTCNF_INT_ANYRD_2CLEAR_MASK  (0x10)      // Latch Clear Method
    #define ITG_3200_REG_GYRO_INTCNF_INT_ANYRD_2CLEAR_STAT  (0x00)      // Status Register Read Only
    #define ITG_3200_REG_GYRO_INTCNF_INT_ANYRD_2CLEAR_ANY   (0x10)      // Any Register Read

    #define ITG_3200_REG_GYRO_INTCNF_INT_ITG_RDY_EN_MASK    (0x04)      // Interrupt Status when Device is Ready (PLL Ready after changing Clock Source)
    #define ITG_3200_REG_GYRO_INTCNF_INT_ITG_RDY_EN_DISABLE (0x00)      // Disable Interrupt when Device is Ready (PLL Ready after changing Clock Source)
    #define ITG_3200_REG_GYRO_INTCNF_INT_ITG_RDY_EN_ENABLE  (0x04)      // Enable Interrupt when Device is Ready (PLL Ready after changing Clock Source)

    #define ITG_3200_REG_GYRO_INTCNF_INT_RAW_RDY_EN_MASK    (0x01)      // Interrupt Status when Data is Ready
    #define ITG_3200_REG_GYRO_INTCNF_INT_RAW_RDY_EN_DISABLE (0x00)      // Disable Interrupt when Data is Ready
    #define ITG_3200_REG_GYRO_INTCNF_INT_RAW_RDY_EN_ENABLE  (0x01)      // Enable Interrupt when Data is Ready

/**************************************************************************
    INTERRUPT STATUS REGISTER
**************************************************************************/
    #define ITG_3200_REG_GYRO_INTSTAT_INT_ITG_RDY_MASK      (0x04)      // PLL Ready Status
    #define ITG_3200_REG_GYRO_INTSTAT_INT_ITG_RDY_NOT       (0x00)      // PLL Not Ready
    #define ITG_3200_REG_GYRO_INTSTAT_INT_ITG_RDY_READY     (0x04)      // PLL Ready

    #define ITG_3200_REG_GYRO_INTSTAT_INT_RAW_RDY_MASK      (0x01)      // Raw Data Ready Status
    #define ITG_3200_REG_GYRO_INTSTAT_INT_RAW_RDY_NOT       (0x00)      // Raw Data Not Ready
    #define ITG_3200_REG_GYRO_INTSTAT_INT_RAW_RDY_READY     (0x01)      // Raw Data is Ready

/**************************************************************************
    POWER MANAGEMENT REGISTER
**************************************************************************/
    #define ITG_3200_REG_GYRO_PM_H_RESET_MASK               (0x80)      // Reset Device and Internal Registers to the Power-Up-Default Settings
    #define ITG_3200_REG_GYRO_PM_H_RESET_DISABLE            (0x00)      // Disable Reset Device and Internal Registers to the Power-Up-Default Settings
    #define ITG_3200_REG_GYRO_PM_H_RESET_ENABLE             (0x80)      // Enable Reset Device and Internal Registers to the Power-Up-Default Settings

    #define ITG_3200_REG_GYRO_PM_SLEEP_MASK                 (0x40)      // Low Power Sleep Mode
    #define ITG_3200_REG_GYRO_PM_SLEEP_DISABLE              (0x00)      // Disable Low Power Sleep Mode
    #define ITG_3200_REG_GYRO_PM_SLEEP_ENABLE               (0x40)      // Enable Low Power Sleep Mode

    #define ITG_3200_REG_GYRO_PM_STBY_XG_MASK               (0x20)      // Gyro X Mode
    #define ITG_3200_REG_GYRO_PM_STBY_XG_NORMAL             (0x00)      // Gyro X in Normal Mode
    #define ITG_3200_REG_GYRO_PM_STBY_XG_STANDBY            (0x20)      // Gyro X in Standby Mode

    #define ITG_3200_REG_GYRO_PM_STBY_YG_MASK               (0x10)      // Gyro Y Mode
    #define ITG_3200_REG_GYRO_PM_STBY_YG_NORMAL             (0x00)      // Gyro Y in Normal Mode
    #define ITG_3200_REG_GYRO_PM_STBY_YG_STANDBY            (0x10)      // Gyro Y in Standby Mode

    #define ITG_3200_REG_GYRO_PM_STBY_ZG_MASK               (0x08)      // Gyro Z Mode
    #define ITG_3200_REG_GYRO_PM_STBY_ZG_NORMAL             (0x00)      // Gyro Z in Normal Mode
    #define ITG_3200_REG_GYRO_PM_STBY_ZG_STANDBY            (0x08)      // Gyro Z in Standby Mode

    #define ITG_3200_REG_GYRO_PM_CLK_SEL_MASK               (0x07)      // Select Device Clock Source
    #define ITG_3200_REG_GYRO_PM_CLK_SEL_INT_OSCILLATOR     (0x00)      // Internal Oscillator
    #define ITG_3200_REG_GYRO_PM_CLK_SEL_PLL_GYRO_X         (0x01)      // PLL with X Gyro Reference
    #define ITG_3200_REG_GYRO_PM_CLK_SEL_PLL_GYRO_Y         (0x02)      // PLL with Y Gyro Reference
    #define ITG_3200_REG_GYRO_PM_CLK_SEL_PLL_GYRO_Z         (0x03)      // PLL with Z Gyro Reference
    #define ITG_3200_REG_GYRO_PM_CLK_SEL_PLL_EXT_32_768     (0x04)      // PLL with External 32.768 kHz Reference
    #define ITG_3200_REG_GYRO_PM_CLK_SEL_PLL_EXT_19_2       (0x05)      // PLL with External 19.2 kHz Reference


typedef enum
{
    FSR_2000                    = ITG_3200_REG_GYRO_DLPF_FS_SEL_2000

} itgFSRange_t;

typedef enum
{
    DLPF_CFG_256                = ITG_3200_REG_GYRO_DLPF_CFG_256,
    DLPF_CFG_188                = ITG_3200_REG_GYRO_DLPF_CFG_188,
    DLPF_CFG_98                 = ITG_3200_REG_GYRO_DLPF_CFG_98,
    DLPF_CFG_42                 = ITG_3200_REG_GYRO_DLPF_CFG_42,
    DLPF_CFG_20                 = ITG_3200_REG_GYRO_DLPF_CFG_20,
    DLPF_CFG_10                 = ITG_3200_REG_GYRO_DLPF_CFG_10,
    DLPF_CFG_5                  = ITG_3200_REG_GYRO_DLPF_CFG_5
    
} itgDLPFBandwidth_t;

typedef enum
{
    INT_LOGIC_HIGH              = ITG_3200_REG_GYRO_INTCNF_ACTL_HIGH,
    INT_LOGIC_LOW               = ITG_3200_REG_GYRO_INTCNF_ACTL_LOW
    
} itgINTLogic_t;

typedef enum
{
    DRIVE_LOGIC_PUSHPULL        = ITG_3200_REG_GYRO_INTCNF_OPEN_PUSH_PULL,
    DRIVE_LOGIC_OPEN            = ITG_3200_REG_GYRO_INTCNF_OPEN_OPEN
    
} itgINTDrive_t;

typedef enum
{
    LATCH_MODE_50USPULSE        = ITG_3200_REG_GYRO_INTCNF_LATCH_INT_EN_50US,
    LATCH_MODE_LATCH            = ITG_3200_REG_GYRO_INTCNF_LATCH_INT_EN_LATCH
    
} itgLatchMode_t;

typedef enum
{
    LATCH_CLEAR_STATUS          = ITG_3200_REG_GYRO_INTCNF_INT_ANYRD_2CLEAR_STAT,
    LATCH_CLEAR_ANY             = ITG_3200_REG_GYRO_INTCNF_INT_ANYRD_2CLEAR_ANY
    
} itgLatchClear_t;


typedef enum
{
    INTERRUPT_DEVICE_DISABLE    = ITG_3200_REG_GYRO_INTCNF_INT_ITG_RDY_EN_DISABLE,
    INTERRUPT_DEVICE_ENABLE     = ITG_3200_REG_GYRO_INTCNF_INT_ITG_RDY_EN_ENABLE
    
} itgINTDevice_t;

typedef enum
{
    INTERRUPT_DATA_DISABLE      = ITG_3200_REG_GYRO_INTCNF_INT_RAW_RDY_EN_DISABLE,
    INTERRUPT_DATA_ENABLE       = ITG_3200_REG_GYRO_INTCNF_INT_RAW_RDY_EN_ENABLE
    
} itgINTData_t;

typedef enum
{
    PLL_NOT_READY               = ITG_3200_REG_GYRO_INTSTAT_INT_ITG_RDY_NOT,
    PLL_READY                   = ITG_3200_REG_GYRO_INTSTAT_INT_ITG_RDY_READY
    
} itgPLLReady_t;

typedef enum
{
    RAW_DATA_NOT_READY          = ITG_3200_REG_GYRO_INTSTAT_INT_RAW_RDY_NOT,
    RAW_DATA_READY              = ITG_3200_REG_GYRO_INTSTAT_INT_RAW_RDY_READY
    
} itgRawDataReady_t;

typedef enum
{
    DEVICE_RESET_DISABLE        = ITG_3200_REG_GYRO_PM_H_RESET_DISABLE,
    DEVICE_RESET_ENABLE         = ITG_3200_REG_GYRO_PM_H_RESET_ENABLE
    
} itgDeviceReset_t;

typedef enum
{
    SLEEP_MODE_DISABLE          = ITG_3200_REG_GYRO_PM_SLEEP_DISABLE,
    SLEEP_MODE_ENABLE           = ITG_3200_REG_GYRO_PM_SLEEP_ENABLE
    
} itgSleepMode_t;

typedef enum
{
    GYRO_X_NORMAL               = ITG_3200_REG_GYRO_PM_STBY_XG_NORMAL,
    GYRO_X_STANDBY              = ITG_3200_REG_GYRO_PM_STBY_XG_STANDBY
    
} itgGyroXMode_t;

typedef enum
{
    GYRO_Y_NORMAL               = ITG_3200_REG_GYRO_PM_STBY_YG_NORMAL,
    GYRO_Y_STANDBY              = ITG_3200_REG_GYRO_PM_STBY_YG_STANDBY
    
} itgGyroYMode_t;

typedef enum
{
    GYRO_Z_NORMAL               = ITG_3200_REG_GYRO_PM_STBY_ZG_NORMAL,
    GYRO_Z_STANDBY              = ITG_3200_REG_GYRO_PM_STBY_ZG_STANDBY
    
} itgGyroZMode_t;

typedef enum
{
    INTERNAL_OSCILLATOR            = ITG_3200_REG_GYRO_PM_CLK_SEL_INT_OSCILLATOR,
    PLL_GYRO_X_REF           = ITG_3200_REG_GYRO_PM_CLK_SEL_PLL_GYRO_X,
    PLL_GYRO_Y_REF           = ITG_3200_REG_GYRO_PM_CLK_SEL_PLL_GYRO_Y,
    PLL_GYRO_Z_REF           = ITG_3200_REG_GYRO_PM_CLK_SEL_PLL_GYRO_Z,
    PLL_EXT_32_768_REF  = ITG_3200_REG_GYRO_PM_CLK_SEL_PLL_EXT_32_768,
    PLL_EXT_19_2_REF  = ITG_3200_REG_GYRO_PM_CLK_SEL_PLL_EXT_19_2
    
} itgClockSource_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    float T;
} itgGyroData_t;


class ITG_3200
{
    protected:
        // Instance-specific properties
        uint8_t itg_ConversionDelay;
        itgFSRange_t itg_fsrange;
        itgDLPFBandwidth_t itg_dlpfbandwidth;
        itgINTLogic_t itg_intlogic;
        itgINTDrive_t itg_intdrive;
        itgLatchMode_t itg_latchmode;
        itgLatchClear_t itg_latchclear;
        itgINTDevice_t itg_intdevice;
        itgINTData_t itg_intdata;
        itgPLLReady_t itg_pllready;
        itgRawDataReady_t itg_rawdataready;
        itgDeviceReset_t itg_devicereset;
        itgSleepMode_t itg_sleepmode;
        itgGyroXMode_t itg_gyroxmode;
        itgGyroYMode_t itg_gyroymode;
        itgGyroZMode_t itg_gyrozmode;
        itgClockSource_t itg_clocksource;
    
    public:
        uint8_t itg_i2cAddress;
        itgGyroData_t itg_gyrodata;
        void getAddr_ITG_3200(uint8_t i2cAddress);
        bool begin(void);
        void setUpGyoscope(void);
        void Measure_Gyroscope(void);
        void Measure_Temperature(void);
        void setFSRange(itgFSRange_t fsrange);
        itgFSRange_t getFSRange(void);
        void setDLPFBandwidth(itgDLPFBandwidth_t dlpfbandwidth);
        itgDLPFBandwidth_t getDLPFBandwidth(void);
        void setINTLogic(itgINTLogic_t intlogic);
        itgINTLogic_t getINTLogic(void);
        void setINTDrive(itgINTDrive_t intdrive);
        itgINTDrive_t getINTDrive(void);
        void setLatchMode(itgLatchMode_t latchmode);
        itgLatchMode_t getLatchMode(void);
        void setLatchClear(itgLatchClear_t clearmode);
        itgLatchClear_t getLatchClear(void);
        void setINTDevice(itgINTDevice_t intdevice);
        itgINTDevice_t getINTDevice(void);
        void setINTData(itgINTData_t intdata);
        itgINTData_t getINTData(void);
        void setPLLReady(itgPLLReady_t pllready);
        itgPLLReady_t getPLLReady(void);
        void setRawDataReady(itgRawDataReady_t rawdataready);
        itgRawDataReady_t getRawDataReady(void);
        void setDeviceReset(itgDeviceReset_t devicereset);
        itgDeviceReset_t getDeviceReset(void);
        void setSleepMode(itgSleepMode_t sleepmode);
        itgSleepMode_t getSleepMode(void);
        void setGyroXMode(itgGyroXMode_t gyroxmode);
        itgGyroXMode_t getGyroXMode(void);
        void setGyroYMode(itgGyroYMode_t gyroymode);
        itgGyroYMode_t getGyroYMode(void);
        void setGyroZMode(itgGyroZMode_t gyrozmode);
        itgGyroZMode_t getGyroZMode(void);
        void setClockSource(itgClockSource_t clocksource);
        itgClockSource_t getClockSource(void);
    
    private:
};
