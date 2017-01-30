/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ITG-3200
        This code is designed to work with the ITG-3200_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Gyro?sku=ITG-3200_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/
#include <Wire.h>
#include <ITG_3200.h>

ITG_3200 itg;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    itg.getAddr_ITG_3200(ITG_3200_DEFAULT_ADDRESS);   // 0x68
    // itg.getAddr_ITG_3200(ITG_3200_HIGH_ADDRESS);   // 0x69

    // The Full-Scale Range for the Gyro Sensors, Digital Low Pass Filter Configuration,
    // Logic Level for INT Output Pin, Drive Type for INT Output Pin, Latch Mode, Latch Clear Method,
    // Interrupt Status when Device is Ready, Interrupt Status when Data is Available,
    // PLL Ready Status, Raw Data Ready Status,
    // Device Reset Status and Internal Registers to the Power-Up-Default Settings,
    // Low Power Sleep Mode Status, Gyro X, Y, Z Mode Status, Device Clock Source
    // can be changed via the following functions

    itg.setFSRange(FSR_2000);                       // ±2000°/sec

    itg.setDLPFBandwidth(DLPF_CFG_256);             // Low Pass Filter Bandwidth: 256 Hz, Internal Sample Rate: 8 kHz
    // itg.setDLPFBandwidth(DLPF_CFG_188);          // Low Pass Filter Bandwidth: 188 Hz, Internal Sample Rate: 1 kHz
    // itg.setDLPFBandwidth(DLPF_CFG_98);           // Low Pass Filter Bandwidth: 98 Hz, Internal Sample Rate: 1 kHz
    // itg.setDLPFBandwidth(DLPF_CFG_42);           // Low Pass Filter Bandwidth: 43 Hz, Internal Sample Rate: 1 kHz
    // itg.setDLPFBandwidth(DLPF_CFG_20);           // Low Pass Filter Bandwidth: 20 Hz, Internal Sample Rate: 1 kHz
    // itg.setDLPFBandwidth(DLPF_CFG_10);           // Low Pass Filter Bandwidth: 10 Hz, Internal Sample Rate: 1 kHz
    // itg.setDLPFBandwidth(DLPF_CFG_5);            // Low Pass Filter Bandwidth: 5 Hz, Internal Sample Rate: 1 kHz

    itg.setINTLogic(INT_LOGIC_LOW);                 // Logic level for INT Output Pin: Active Low
    // itg.setINTLogic(INT_LOGIC_HIGH);             // Logic level for INT Output Pin: Active High

    itg.setINTDrive(DRIVE_LOGIC_OPEN);              // Drive Type for INT Output Pin: Open Drain
    // itg.setINTDrive(DRIVE_LOGIC_PUSHPULL);       // Drive Type for INT Output Pin: Push-Pull

    itg.setLatchMode(LATCH_MODE_LATCH);             // Latch until Interrupt is Cleared
    // itg.setLatchMode(LATCH_MODE_50USPULSE);      // Latch Mode: 50us Pulse

    itg.setLatchClear(LATCH_CLEAR_ANY);             // Latch Clear Method: Any Register Read
    // itg.setLatchClear(LATCH_CLEAR_STATUS);       // Latch Clear Method: Status Register Read Only

    itg.setINTDevice(INTERRUPT_DEVICE_DISABLE);     // Disable Interrupt when Device is Ready (PLL Ready after changing Clock Source)
    // itg.setINTDevice(INTERRUPT_DEVICE_ENABLE);   // Enable Interrupt when Device is Ready (PLL Ready after changing Clock Source)

    itg.setINTData(INTERRUPT_DATA_DISABLE);         // Disable Interrupt when Data is Ready
    // itg.setINTData(INTERRUPT_DATA_ENABLE);       // Enable Interrupt when Data is Ready

    itg.setPLLReady(PLL_NOT_READY);                 // PLL Not Ready
    // itg.setPLLReady(PLL_READY);                  // PLL Ready

    itg.setRawDataReady(RAW_DATA_NOT_READY);        // Raw Data Not Ready
    // itg.setRawDataReady(RAW_DATA_READY);         // Raw Data is Ready

    itg.setDeviceReset(DEVICE_RESET_DISABLE);       // Disable Reset Device and Internal Registers to the Power-Up-Default Settings
    // itg.setDeviceReset(DEVICE_RESET_ENABLE);     // Enable Reset Device and Internal Registers to the Power-Up-Default Settings

    itg.setSleepMode(SLEEP_MODE_DISABLE);           // Disable Low Power Sleep Mode
    // itg.setSleepMode(SLEEP_MODE_ENABLE);         // Enable Low Power Sleep Mode

    itg.setGyroXMode(GYRO_X_NORMAL);                // Gyro X in Normal Mode
    // itg.setGyroXMode(GYRO_X_STANDBY);            // Gyro X in Standby Mode

    itg.setGyroYMode(GYRO_Y_NORMAL);                // Gyro Y in Normal Mode
    // itg.setGyroYMode(GYRO_Y_STANDBY);            // Gyro Y in Standby Mode

    itg.setGyroZMode(GYRO_Z_NORMAL);                // Gyro Z in Normal Mode
    // itg.setGyroZMode(GYRO_Z_STANDBY);            // Gyro Z in Standby Mode

    itg.setClockSource(PLL_GYRO_X_REF);             // PLL with X Gyro Reference
    // itg.setClockSource(INTERNAL_OSCILLATOR);     // Internal Oscillator
    // itg.setClockSource(PLL_GYRO_Y_REF);          // PLL with Y Gyro Reference
    // itg.setClockSource(PLL_GYRO_Z_REF);          // PLL with Z Gyro Reference
    // itg.setClockSource(PLL_EXT_32_768_REF);      // PLL with External 32.768 kHz Reference
    // itg.setClockSource(PLL_EXT_19_2_REF);        // PLL with External 19.2 kHz Reference

    itg.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = itg.itg_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Angular Rate Readings from ITG-3200");
        Serial.println(" ");
        // Set up the sensor for the Gyroscope
        itg.setUpGyoscope();
        delay(500);
        // Display the results for Angular Velocity Vector Values and DIE Temperature
        itg.Measure_Gyroscope();
        itg.Measure_Temperature();

        // Output data to screen
        Serial.print("Rotation in X-Axis: ");
        Serial.println(itg.itg_gyrodata.X);
        Serial.print("Rotation in Y-Axis: ");
        Serial.println(itg.itg_gyrodata.Y);
        Serial.print("Rotation in Z-Axis: ");
        Serial.println(itg.itg_gyrodata.Z);
        Serial.println(" ");
        Serial.println("ITG-3200 Die Temperature: ");
        Serial.print("Die Temperature in Celsius: ");
        Serial.print(itg.itg_gyrodata.T);
        Serial.println(" °C");
        Serial.print("Die Temperature in Fahrenheit: ");
        Serial.print(((itg.itg_gyrodata.T * 1.8) + 32));
        Serial.println(" °F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("ITG-3200 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
