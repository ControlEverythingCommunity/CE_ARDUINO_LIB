/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        CAT34TS02
        This code is designed to work with the CAT34TS02_I2CADC I2C Mini Module available from ControlEverything.com.
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
                                                                    Address
                                                                    Code        A2  A1  A0
*/
    #define CAT34TS02_DEFAULT_ADDRESS               (0x18)          // 0011     0    0   0
    #define CAT34TS02_ADDRESS_A2A1A0_001            (0x19)          // 0011     0    0   1
    #define CAT34TS02_ADDRESS_A2A1A0_010            (0x1A)          // 0011     0    1   0
    #define CAT34TS02_ADDRESS_A2A1A0_011            (0x1B)          // 0011     0    1   1
    #define CAT34TS02_ADDRESS_A2A1A0_100            (0x1C)          // 0011     1    0   0
    #define CAT34TS02_ADDRESS_A2A1A0_101            (0x1D)          // 0011     1    0   1
    #define CAT34TS02_ADDRESS_A2A1A0_110            (0x1E)          // 0011     1    1   0
    #define CAT34TS02_ADDRESS_A2A1A0_111            (0x1F)          // 0011     1    1   1

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define CAT34TS02_CONVERSIONDELAY               (100)

/**************************************************************************
    POINTER REGISTER
**************************************************************************/
    #define CAT34TS02_REG_POINTER_MASK              (0xFF)
    #define CAT34TS02_REG_POINTER_CAPABLE           (0x00)
    #define CAT34TS02_REG_POINTER_CONFIG            (0x01)
    #define CAT34TS02_REG_POINTER_THIGH             (0x02)
    #define CAT34TS02_REG_POINTER_TLOW              (0x03)
    #define CAT34TS02_REG_POINTER_TCRIT             (0x04)
    #define CAT34TS02_REG_POINTER_TEMP              (0x05)
    #define CAT34TS02_REG_POINTER_MANUFID           (0x06)
    #define CAT34TS02_REG_POINTER_DEVID             (0x07)

/**************************************************************************
    CAPABILITY REGISTER
 **************************************************************************/
    #define CAT34TS02_REG_CAPABL_EVSD_MASK          (0x0080)    // EVENT Output with TS Shut-Down
    #define CAT34TS02_REG_CAPABL_EVSD_FROZEN        (0x0000)    // A TS shut−down freezes the EVENT output
    #define CAT34TS02_REG_CAPABL_EVSD_CLEARED       (0x0080)    // A TS shut−down de−asserts the EVENT output

    #define CAT34TS02_REG_CAPABL_TMOUT_MASK         (0x0040)    // SMBus Time−Out Limit by TS
    #define CAT34TS02_REG_CAPABL_TMOUT_LOW          (0x0000)    // The TS implements SMBus time−out within the range 10 to 60 ms
    #define CAT34TS02_REG_CAPABL_TMOUT_HIGH         (0x0040)    // The TS implements SMBus time−out within the range 25 to 35 ms

    #define CAT34TS02_REG_CAPABL_TRES_MASK          (0x0018)    // Conversion Resolution
    #define CAT34TS02_REG_CAPABL_TRES_9BIT          (0x0000)    // 0.50°C
    #define CAT34TS02_REG_CAPABL_TRES_10BIT         (0x0008)    // 0.25°C
    #define CAT34TS02_REG_CAPABL_TRES_11BIT         (0x0010)    // 0.125°C
    #define CAT34TS02_REG_CAPABL_TRES_12BIT         (0x0018)    // 0.0625°C

    #define CAT34TS02_REG_CAPABL_RANGE_MASK         (0x0004)    // Temperature Range
    #define CAT34TS02_REG_CAPABL_RANGE_POS          (0x0000)    // Positive Temperature Only
    #define CAT34TS02_REG_CAPABL_RANGE_POSNEG       (0x0004)    // Positive and Negative Temperature

    #define CAT34TS02_REG_CAPABL_ACC_MASK           (0x0002)    // Active Range Class
    #define CAT34TS02_REG_CAPABL_ACC_C              (0x0000)    // +/- 2°C over the active range and +/- 3°C over the operating range (Class C)
    #define CAT34TS02_REG_CAPABL_ACC_B              (0x0002)    // +/- 1°C over the active range and +/- 2°C over the monitor range (Class B)

    #define CAT34TS02_REG_CAPABL_EVENT_MASK         (0x0001)    // Temperature Event
    #define CAT34TS02_REG_CAPABL_EVENT_CRIT         (0x0000)    // Critical Temperature Only
    #define CAT34TS02_REG_CAPABL_EVENT_ALARMCRIT    (0x0001)    // Alarm and Critical Temperature

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/

    #define CAT34TS02_REG_CONFIG_THYST_MASK         (0x0600)    // TUPPER and TLOWER Limit Hysteresis
    #define CAT34TS02_REG_CONFIG_THYST_DIS          (0x0000)    // Disable Hysteresis
    #define CAT34TS02_REG_CONFIG_THYST_1_5C         (0x0200)    // Set hysteresis at 1.5°C
    #define CAT34TS02_REG_CONFIG_THYST_3C           (0x0400)    // Set hysteresis at 3.0°C
    #define CAT34TS02_REG_CONFIG_THYST_6C           (0x0600)    // Set hysteresis at 6.0°C

    #define CAT34TS02_REG_CONFIG_SHDN_MASK          (0x0100)    // Thermal Sensor Shutdown Mode
    #define CAT34TS02_REG_CONFIG_SHDN_ENABLE        (0x0000)    // Thermal Sensor is Enabled
    #define CAT34TS02_REG_CONFIG_SHDN_SHUT          (0x0100)    // Thermal Sensor is Shut Down

    #define CAT34TS02_REG_CONFIG_TCRIT_MASK         (0x0080)    // Critical Trip Output
    #define CAT34TS02_REG_CONFIG_TCRIT_UPDATE       (0x0000)    // Critical Trip register can be Updated
    #define CAT34TS02_REG_CONFIG_TCRIT_NOUPDATE     (0x0080)    // Critical Trip register cannot be Modified

    #define CAT34TS02_REG_CONFIG_TALARM_MASK        (0x0040)    // Alarm Trip Output
    #define CAT34TS02_REG_CONFIG_TALARM_UPDATE      (0x0000)    // Alarm Trip register can be Updated
    #define CAT34TS02_REG_CONFIG_TALARM_NOUPDATE    (0x0040)    // Alarm Trip register cannot be Modified

    #define CAT34TS02_REG_CONFIG_INTRCLEAR_MASK     (0x0020)    // Interrupt Clear
    #define CAT34TS02_REG_CONFIG_INTRCLEAR_SELF     (0x0000)    // Always reads as 0 (Self−Clearing)
    #define CAT34TS02_REG_CONFIG_INTRCLEAR_INTR     (0x0020)    // Writing a 1 to this position clears an event recording in interrupt mode only

    #define CAT34TS02_REG_CONFIG_EVRNTSTS_MASK      (0x0010)    // EVENT Output Status
    #define CAT34TS02_REG_CONFIG_EVRNTSTS_NOASRT    (0x0000)    // EVENT output pin is not being asserted
    #define CAT34TS02_REG_CONFIG_EVRNTSTS_ASRT      (0x0010)    // EVENT output pin is being asserted

    #define CAT34TS02_REG_CONFIG_EVENTCTRL_MASK     (0x0008)    // EVENT Output Control
    #define CAT34TS02_REG_CONFIG_EVENTCTRL_DISABLE  (0x0000)    // EVENT Output Disabled
    #define CAT34TS02_REG_CONFIG_EVENTCTRL_ENABLE   (0x0008)    // EVENT Output Enabled

    #define CAT34TS02_REG_CONFIG_TCRITONLY_MASK     (0x0004)    // Event Condition Triggering
    #define CAT34TS02_REG_CONFIG_TCRITONLY_ALL      (0x0000)    // Event condition triggered by Alarm or Critical Temperature Limit crossing
    #define CAT34TS02_REG_CONFIG_TCRITONLY_SELECT   (0x0004)    // Event condition triggered by critical temperature limit crossing only

    #define CAT34TS02_REG_CONFIG_EVENTPOL_MASK      (0x0002)    // EVENT Output Polarity
    #define CAT34TS02_REG_CONFIG_EVENTPOL_LOW       (0x0000)    // EVENT Output Active Low
    #define CAT34TS02_REG_CONFIG_EVENTPOL_HIGH      (0x0002)    // EVENT Output Active High

    #define CAT34TS02_REG_CONFIG_EVENTMOD_MASK      (0x0001)    // EVENT Output Mode
    #define CAT34TS02_REG_CONFIG_EVENTMOD_COMP      (0x0000)    // Comparator Mode
    #define CAT34TS02_REG_CONFIG_EVENTMOD_INTR      (0x0001)    // Interrupt Mode



typedef enum
{
    EVSD_FROZEN             = CAT34TS02_REG_CAPABL_EVSD_FROZEN,
    EVSD_CLEARED            = CAT34TS02_REG_CAPABL_EVSD_CLEARED
} catEVSD_t;

typedef enum
{
    SMBUS_LOW             = CAT34TS02_REG_CAPABL_TMOUT_LOW,
    SMBUS_HIGH            = CAT34TS02_REG_CAPABL_TMOUT_HIGH
} catSMBus_t;


typedef enum
{
    RES_9BIT            = CAT34TS02_REG_CAPABL_TRES_9BIT,
    RES_10BIT           = CAT34TS02_REG_CAPABL_TRES_10BIT,
    RES_11BIT           = CAT34TS02_REG_CAPABL_TRES_11BIT,
    RES_12BIT           = CAT34TS02_REG_CAPABL_TRES_12BIT
    
} catTRes_t;

typedef enum
{
    TEMPRANGE_POSTIVE       = CAT34TS02_REG_CAPABL_RANGE_POS,
    TEMPRANGE_BOTH          = CAT34TS02_REG_CAPABL_RANGE_POSNEG
} catTRange_t;

typedef enum
{
    ACTIVERANGE_C           = CAT34TS02_REG_CAPABL_ACC_C,
    ACTIVERANGE_B           = CAT34TS02_REG_CAPABL_ACC_B
} catARange_t;


typedef enum
{
    TEMPEVENT_CRIT          = CAT34TS02_REG_CAPABL_EVENT_CRIT,
    TEMPEVENT_BOTH          = CAT34TS02_REG_CAPABL_EVENT_ALARMCRIT
} catTEvent_t;

typedef enum
{
    THYST_DISABLE           = CAT34TS02_REG_CONFIG_THYST_DIS,
    THYST_1_5C              = CAT34TS02_REG_CONFIG_THYST_1_5C,
    THYST_3C                = CAT34TS02_REG_CONFIG_THYST_3C,
    THYST_6C                = CAT34TS02_REG_CONFIG_THYST_6C
} catTHyst_t;

typedef enum
{
    SHDN_ENABLE               = CAT34TS02_REG_CONFIG_SHDN_ENABLE,
    SHDN_SHUT               = CAT34TS02_REG_CONFIG_SHDN_SHUT
} catTShdn_t;

typedef enum
{
    TCRIT_UPDATE            = CAT34TS02_REG_CONFIG_TCRIT_UPDATE,
    TCRIT_NOUPDATE          = CAT34TS02_REG_CONFIG_TCRIT_NOUPDATE
} catTCrit_t;


typedef enum
{
    TALARM_UPDATE            = CAT34TS02_REG_CONFIG_TALARM_UPDATE,
    TALARM_NOUPDATE          = CAT34TS02_REG_CONFIG_TALARM_NOUPDATE
} catTAlarm_t;


typedef enum
{
    INTRCLEAR_SELF              = CAT34TS02_REG_CONFIG_INTRCLEAR_SELF,
    INTRCLEAR_INTR              = CAT34TS02_REG_CONFIG_INTRCLEAR_INTR
} catIntrClear_t;

typedef enum
{
    EVENTSTAT_NOASRT        = CAT34TS02_REG_CONFIG_EVRNTSTS_NOASRT,
    EVENTSTAT_ASRT          = CAT34TS02_REG_CONFIG_EVRNTSTS_ASRT
} catEventStat_t;

typedef enum
{
    EVENTCNT_DISABLE        = CAT34TS02_REG_CONFIG_EVENTCTRL_DISABLE,
    EVENTCNT_ENABLE         = CAT34TS02_REG_CONFIG_EVENTCTRL_ENABLE
} catEventCtrl_t;

typedef enum
{
    EVENTCRIT_ALL          = CAT34TS02_REG_CONFIG_TCRITONLY_ALL,
    EVENTCRIT_SELECT       = CAT34TS02_REG_CONFIG_TCRITONLY_SELECT
} catEventCrit_t;

typedef enum
{
    EVENTPOL_LOW        = CAT34TS02_REG_CONFIG_EVENTPOL_LOW,
    EVENTPOL_HIGH       = CAT34TS02_REG_CONFIG_EVENTPOL_HIGH
} catEventPol_t;

typedef enum
{
    EVENTMOD_COMP       = CAT34TS02_REG_CONFIG_EVENTMOD_COMP,
    EVENTMOD_INTR       = CAT34TS02_REG_CONFIG_EVENTMOD_INTR
} catEventMod_t;


class CAT34TS02
{
    protected:
        // Instance-specific properties
        uint8_t cat_conversionDelay;
        catEVSD_t cat_evsd;
        catSMBus_t cat_smbus;
        catTRes_t cat_tres;
        catTRange_t cat_trange;
        catARange_t cat_arange;
        catTEvent_t cat_tevent;
        catTHyst_t cat_thyst;
        catTShdn_t cat_tshdn;
        catTCrit_t cat_tcrit;
        catTAlarm_t cat_talarm;
        catIntrClear_t cat_intrclear;
        catEventStat_t cat_eventstat;;
        catEventCtrl_t cat_eventctrl;
        catEventCrit_t cat_eventcrit;
        catEventPol_t cat_eventpol;
        catEventMod_t cat_eventmod;

    public:
        uint8_t cat_i2cAddress;
        void getAddr_CAT34TS02(uint8_t i2cAddress);
        boolean begin(void);
        int16_t Measure_Temp(void);
        void setEVSD(catEVSD_t evsd);
        catEVSD_t getEVSD(void);
        void setSMBus(catSMBus_t smbus);
        catSMBus_t getSMBus(void);
        void setTRes(catTRes_t tres);
        catTRes_t getTRes(void);
        void setTRange(catTRange_t trange);
        catTRange_t getTRange(void);
        void setARange(catARange_t arange);
        catARange_t getARange(void);
        void setTEvent(catTEvent_t tevent);
        catTEvent_t getTEvent(void);
        void setTHyst(catTHyst_t thyst);
        catTHyst_t getTHyst(void);
        void setTShdn(catTShdn_t tshdn);
        catTShdn_t getTShdn(void);
        void setTCrit(catTCrit_t tcrit);
        catTCrit_t getTCrit(void);
        void setTAlarm_t(catTAlarm_t talarm);
        catTAlarm_t getTAlarm_t(void);
        void setIntrClear(catIntrClear_t intrclear);
        catIntrClear_t getIntrClear(void);
        void setEventStat(catEventStat_t eventstat);
        catEventStat_t getEventStat(void);
        void setEventCtrl(catEventCtrl_t eventctrl);
        catEventCtrl_t getEventCtrl(void);
        void setEventCrit(catEventCrit_t eventcrit);
        catEventCrit_t getEventCrit(void);
        void setEventPol(catEventPol_t eventpol);
        catEventPol_t getEventPol(void);
        void setEventMod(catEventMod_t eventmod);
        catEventMod_t getEventMod(void);

    private:
};
