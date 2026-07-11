/*  Globals.h Global variables, definitions and functions.
    Copyright (c) 2023 Joe Mann.  All right reserved.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

*/

#ifndef Globals_H
#define Globals_H

#include <Arduino.h>
#include <Wire.h>
#include <STM32LowPower.h>
#include <STM32RTC.h>
#include <ChannelConfig.h>
#include <System.h>
#include <IMU.h>
#include <SPI.h>
#include <Storage.h>
#include <backup.h>

// Firmware version
#define FW_VER "v0.9"

// Build date
#define BUILD_DATE __DATE__ " " __TIME__

// Number of hardware output channels
// Exocet 22-channel tiered build: 4x BTS7002-1EPP (21A) + 4x BTS7008-2EPZ (2x7.5A)
// + 5x BTS7050-2EPL (2x3A). Dual-channel chips share one IS pin selected by DSEL.
#define NUM_CHANNELS 22

// Number of physical output driver chips
#define NUM_OUTPUT_CHIPS 13

// Number of digital input channels
#define NUM_DI_CHANNELS 8

// Number of analogue input channels
#define NUM_ANA_CHANNELS 8

// Delay (microseconds) before making an analog reading
// TODO: measure actual turn on delay and adjust this value
#define ANALOG_DELAY 100

// Gain to convert measured MCU analog pin voltage to source/input voltage
// Example calibration: 5.00V source -> 3.265V at MCU pin => gain ~= 1.5314
#define ANALOG_INPUT_PIN_TO_SOURCE_GAIN 1.5314F

// Maximum PWM duty accounting for turn off delay. Above this value, a PWM channel will be set to 100% duty
// BTS50010-1LUA max turn off time is 220µs. Default PWM frequency is 150Hz (~6667µs period). Configured max PWM duty remains conservatively limited to 95%.
#define MAX_DUTY 95

// Min duty accounting for turn on delay. Above this value, a PWM channel will be set to 0% duty.
// BTS50010-1LUA max turn on delay is 190µs. Default PWM frequency is 150Hz (~6667µs period). Configured min PWM duty remains conservatively limited to 10%.
#define MIN_DUTY 10

// IS current fault threshold voltage. Above this threshold, the channel is either open circuit, short circuit or over temperature
#define FAULT_THRESHOLD 2.00

// Microsecond representation of a CPU tick
#define CPU_TICK_MICROS (1E6 / F_CPU)

// Maximum per-channel current supported by hardware. No channel can exceed this limit.
// 22.0 covers the 21A BTS7002-1EPP high tier.
#define CURRENT_MAX 22.0

// Channel inrush delay (milliseconds)
#define INRUSH_DELAY 500

// Maximum inrush delay (milliseconds). This has to be balanced with what the wiring harness can support
#define INRUSH_MAX 2000

// Maximum inrush current threshold (Amps). This has to be balanced with what the wiring harness can support
#define INRUSH_CURRENT_MAX 50.0

// Sytem current max (total). Should never reach this as each channel is independantly monitored
#define SYSTEM_CURRENT_MAX 150

// Default current sense ratio as specified by the BTS50010 datasheet
#define DEFAULT_DK_VALUE 38000

// Main task timer intervals (milliseconds)
#define DISPLAY_INTERVAL 50
#define COMMS_INTERVAL 100
#define LOG_INTERVAL 100
#define LOG_INTERVAL_US ((uint32_t)LOG_INTERVAL * 1000UL)
#define GPS_INTERVAL 1000
#define SIGNAL_QUALITY_INTERVAL 5000
#define SIM_TEMPERATURE_INTERVAL 15000
#define SYSTEM_CAN_INTERVAL 100
#define BL_FADE_INTRVAL 0

#define SPLASH_SCREEN_DELAY 2000
#define FORCE_EEPROM_WIPE_ON_BOOT false

#define DEBUG_INTERVAL 1000

// I2C bus speed
#define I2C_BUS_SPEED 400000UL

// Watchdog timer interval (microseconds)
#define WATCHDOG_INTERVAL 2000000

// Wake debounce time (milliseconds)
#define WAKE_DEBOUNCE_TIME 50

// Unused pin that can be used to debug analog read timings which are critical to obtaining correct current measurements on PWM channels
#define ANALOG_READ_DEBUG_PIN 20

// Debug flag
// #define DEBUG
#define DEBUG_PIN PA15

// Battery measurement analog input pin
#define VBATT_ANALOG_PIN PC4

// Nominal battery voltage
#define VBATT_NOMINAL 13.8

// CAN bus termination resistor enable pin
#define CAN_BUS_RESISTOR_ENABLE 6

// Battery voltage threshold at which power loss is immenent and logging should be stopped
#define LOGGING_VBATT_THRESHOLD 9.0

// Maximum permissible system temperature
#define SYSTEM_TEMP_LIMIT 80.0

#define SYSTEM_TEMP_WARNING_THRESHOLD 70.0f
#define SYSTEM_TEMP_WARNING_CLEAR_THRESHOLD 68.0f
#define SYSTEM_TEMP_ERROR_THRESHOLD 75.0f
#define SYSTEM_TEMP_ERROR_CLEAR_THRESHOLD 73.0f

// System error bitmasks
#define OVERCURRENT 0x0001
#define OVERTEMP 0x0002
#define UNDERVOLTAGE 0x0004
#define CRC_CHECK_FAILED 0x0008
#define SDCARD_ERROR 0x0010
#define PC_COMMS_CHECKSUM_ERROR 0x0020
#define GPS_ERROR 0x0040
#define TEMP_WARNING 0x0080

// Channel error bitmasks
#define CHN_OVERCURRENT 0x01
#define CHN_UNDERCURRENT 0x02
#define IS_FAULT 0x04
#define RETRY_LOCKOUT 0x08
#define CHN_TEMP_SHUTDOWN 0x10

// ECU CAN addresses
#define CHAN_CAN_ID 0x700
#define DIG_INPUT_CAN_ID 0x710
#define ANA_INPUT_CAN_ID 0x712
#define SYS_CAN_ID 0x720
#define SYS_CONFIG_CAN_ID 0x730
#define CONF_CAN_ID 0x740

// Ignition input (KL15)
#define IGN_INPUT PE2

// IMU Pins
#define IMU_INT1 PE4
#define IMU_INT2 PE5

// Default wake window for IMU checks
#define DEFAULT_WW 5000

// Default motion dead time (minutes). Ignore motion after ignition off for this period (gives time for vehicle to come to rest, passengers to disembark etc.)
#define DEFAULT_MOTION_DEADTIME 5

// Backup register to store ignition off time
#define BACKUP_REG_IGN_OFF_TIME 0

// Default log frequecy of 10Hz
#define DEFAULT_LOG_FREQUENCY 10

// Default number of log lines. 36000 = 1 hour @ 10Hz
#define DEFAULT_LOG_LINES 36000

// Number of logs to keep on the SD card
#define NUMBER_LOGS 10

// Maximum motion dead time in minutes
#define MAX_MOTION_DEAD_TIME 254

// Power enable pins
#define PWR_EN_5V PE7
#define PWR_EN_3V3 PF11

// Charging pins
#define CHARGE_EN PG0
#define BATT_INT PG1

// SIM module power enable
#define SIM_REGULATOR PD13

// Battery states
#define COMMSOK 0
#define BATTERY_CHARGED 1
#define BATTERY_CHARGING 2
#define BATTERY_LOW 3
#define BATTERY_CRITICAL 4

// Power states
#define RUN 0
#define PREPARE_SLEEP 1
#define SLEEPING 2
#define IGNITION_WAKING 3
#define IGNITION_WAKE 4
#define IMU_WAKING 5
#define IMU_WAKE 6
#define IMU_WAKE_WINDOW 7

// SPI 2 Pins
#define PICO PB15
#define POCI PB14
#define SCK2 PB13
#define CS1 PB12
#define CS2 PB11

// LCD pins
#define TFT_BL PB10

// GSM Module Pins
#define SIM_PWR PC6
#define SIM_RST PC7
#define SIM_FLIGHT PB8

#define COMMS_TIMEOUT 5000

// Maximum inrush delay in milliseconds
#define MAX_INRUSH_DELAY 2000

// Maximum soft start time in milliseconds
#define MAX_SOFT_START_TIME 5000

// Maximum soft stop time in milliseconds
#define MAX_SOFT_STOP_TIME 5000

// Max run on time in milliseconds
#define MAX_RUN_ON_TIME 3600000

// Maximum intermittent on/off time in milliseconds
#define MAX_INTERMITTENT_TIME 10000

// EEPROM write delay (milliseconds) after a change is made to allow time for multiple changes to be made without writing to EEPROM after each change
#define EEPROM_WRITE_DELAY 5000

/// @brief Tracks which channels are eligible for RunOn (were enabled at PREPARE_SLEEP entry)
extern bool runOnEligible[NUM_CHANNELS];

/// @brief Absolute millis deadline for each active RunOn window. Zero means inactive.
extern uint32_t runOnDeadline[NUM_CHANNELS];

/// @brief Output enabled flags
extern bool enabledFlags[NUM_CHANNELS];

/// @brief Output enabled timers
extern unsigned long enabledTimers[NUM_CHANNELS];

// SPI 2
extern SPIClass SPI_2;

/// @brief PC connection status. 0 = disconnected, 1 = connected, 2 = Checksum fail
extern uint8_t connectionStatus;

/// @brief PC communications OK flag
extern bool pcCommsOK;

/// @brief Number of bytes read from PC
extern int recBytesRead;

/// @brief Flag to denote if the background has been drawn
extern bool backgroundDrawn;

/// @brief Flag to denote if the system is to boot to sleep mode. Display background needs to be drawn first.
extern bool bootToSleep;

/// @brief IMU wake mode flag
extern volatile bool IMUWakeMode;

/// @brief Ignition wake pending flag
extern volatile bool ignitionWakePending;

/// @brief IMU wake pending flag
extern volatile bool imuWakePending;

/// @brief Flag to re-draw display
extern bool invalidateDisplay;

/// @brief EEPROM save timeout. Set to a future time (millis) when a change is made that requires saving to EEPROM. When millis exceeds this value, the config will be saved and this reset to 0.
extern uint32_t EEPROMSaveTimout;

/// @brief Save to EEPROM flag. Set to true when a change is made that requires saving to EEPROM.
extern bool pendingEEPROMSave;

/// @brief Flag to indicate to the main look that the channel config needs to be saved to EEPROM. When EEPROMSaveTimout is exceeded, the config will be saved and this reset to false.
extern bool saveEEPROMOnTimeout;

/// @brief Defines available analogue channel types
enum AnalogueChannelType
{
  RAW_VOLTAGE, // Raw voltage input
  ACTIVE,      // Active input
  PASSIVE,     // Passive input
  NTC,         // NTC thermistor input
  DIGITAL      // Digital use of analogue-capable input pin
};

/// @brief Analogue units options
enum AnalogueUnits
{
  ANA_UNITS_VOLTS = 0,
  ANA_UNITS_AMPS = 1,
  ANA_UNITS_CELSIUS = 2,
  ANA_UNITS_FAHRENHEIT = 3,
  ANA_UNITS_PERCENT = 4,
  ANA_UNITS_RPM = 5,
  ANA_UNITS_KPH = 6,
  ANA_UNITS_MPH = 7,
  ANA_UNITS_BAR = 8,
  ANA_UNITS_PSI = 9
};

/// @brief Analogue input config structure
struct __attribute__((packed)) AnalogueInputs
{
  AnalogueChannelType ChanType; // Channel type
  uint8_t InputPin;             // Input pin
  uint8_t PullUpPin;            // Pull-up enable pin
  uint8_t PullDownPin;          // Pull-down enable pin
  bool PullUpEnable;            // Pull-up enable flag
  bool PullDownEnable;          // Pull-down enable flag
  float InputVoltage;           // Live input voltage at source/input connector
  float InputValue;             // Live converted value in selected units
  AnalogueUnits Units;          // Units for display
  uint8_t CalibrationPoints;    // 2 or 3 point calibration for ACTIVE/PASSIVE
  float CalibrationVolt1;       // Calibration point 1 voltage
  float CalibrationValue1;      // Calibration point 1 value
  float CalibrationVolt2;       // Calibration point 2 voltage
  float CalibrationValue2;      // Calibration point 2 value
  float CalibrationVolt3;       // Calibration point 3 voltage
  float CalibrationValue3;      // Calibration point 3 value
  float NTCBeta;                // NTC beta value
  float NTCNominalResistance;   // NTC nominal resistance at 25C (ohms)
};

// Sentinel for "no pin assigned". Guarded at every use site.
#define PIN_UNASSIGNED 0xFE

// Shared diagnostics-enable (DEN) pin gating all PROFET chips. Driven HIGH in run,
// can be dropped LOW in sleep to kill diagnostic quiescent draw.
#define DEN_SHARED_PIN PD6

// Settle time after switching a dual-channel chip's DSEL before sampling IS
#define DSEL_SETTLE_MICROS 100

/// @brief Channel digital input pins (defaults). Exocet build: original PE8-15 DI pins
/// are repurposed as output drives; DI moves to former spare pins, routed to a header.
const uint8_t DIchannelInputPins[NUM_DI_CHANNELS] = {PA8, PC13, PD10, PD11, PD12, PD15, PE6, PG7};

/// @brief Channel analogue input pins (defaults)
const uint8_t ANAchannelInputPins[NUM_ANA_CHANNELS] = {PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10};

// @brief Channel analogue input pull-up pins. Exocet build: MCU-switched pulls removed —
// the GPIOs were reassigned to DSEL/DEN duty; the PCB provides fixed DNP pull resistors.
const uint8_t ANAchannelInputPullUps[NUM_ANA_CHANNELS] = {PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED};

/// @brief Channel analogue input pull-down pins (see pull-up note above)
const uint8_t ANAchannelInputPullDowns[NUM_ANA_CHANNELS] = {PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED};

/// @brief Channel digital output pins.
/// idx 0-3   HIGH tier, 4x BTS7002-1EPP           (port G/F pins keep DMA PWM support)
/// idx 4-11  MED tier,  4x BTS7008-2EPZ pairs     (chip pairs: {4,5} {6,7} {8,9} {10,11})
/// idx 12-21 LOW tier,  5x BTS7050-2EPL pairs     (chip pairs: {12,13} {14,15} {16,17} {18,19} {20,21})
/// idx 14-21 sit on GPIOE: digital on/off only, no DMA PWM (see updatePWMDutyCycle fallback).
const uint8_t channelOutputPins[NUM_CHANNELS] = {PG10, PG9, PG6, PG5, PG4, PG3, PG2, PF15, PF14, PF13, PF12, PF2, PF1, PF0, PE15, PE14, PE13, PE12, PE11, PE10, PE9, PE8};

/// @brief Channel analog current sense pins. Dual-channel chips share one IS pin
/// (both channels of a pair carry the same sense pin; DSEL picks which channel is mirrored).
/// PC0 is deliberately unused — spare ADC routed to a test point on the PCB.
const uint8_t channelCurrentSensePins[NUM_CHANNELS] = {PA0, PA1, PA2, PA3, PA4, PA4, PA5, PA5, PA6, PA6, PB1, PB1, PB0, PB0, PA7, PA7, PC3, PC3, PC2, PC2, PC1, PC1};

/// @brief Channel DSEL (IS channel select) pins. PIN_UNASSIGNED for single-channel chips.
const uint8_t channelDselPins[NUM_CHANNELS] = {PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED, PIN_UNASSIGNED, PD3, PD3, PD5, PD5, PD7, PD7, PG12, PG12, PG15, PG15, PB4, PB4, PB9, PB9, PE1, PE1, PD4, PD4};

/// @brief DSEL level selecting this channel's IS mirror (0 = ch0/DSEL low, 1 = ch1/DSEL high)
const uint8_t channelDselStates[NUM_CHANNELS] = {0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};

/// @brief Default per-channel kILIS by tier (BTS7002-1EPP 22900, BTS7008-2EPZ 9500, BTS7050-2EPL 2030)
const float channelDefaultKILIS[NUM_CHANNELS] = {22900, 22900, 22900, 22900, 9500, 9500, 9500, 9500, 9500, 9500, 9500, 9500, 2030, 2030, 2030, 2030, 2030, 2030, 2030, 2030, 2030, 2030};

/// @brief CAN enabled channel enabled flags
extern bool CANChannelEnableFlags[NUM_CHANNELS];

// Timers for main tasks
extern uint32_t imuWWtimer;
extern uint32_t DisplayTimer;
extern uint32_t CommsTimer;
extern uint32_t BattTimer;
extern uint32_t LogTimer;
extern uint32_t GPSTimer;
extern uint32_t signalTimer;
extern uint32_t simTemperatureTimer;
extern uint32_t BLTimer;
extern uint32_t wakeDebounceTimer;
extern uint32_t systemCANTimer;
extern int blLevel;

/// @brief HSD Output channels
extern ChannelConfig Channels[NUM_CHANNELS];

/// @brief Channel runtime data
extern ChannelConfigRuntime ChannelRuntime[NUM_CHANNELS];

/// @brief Channel configurations
extern AnalogueInputs AnalogueIns[NUM_ANA_CHANNELS];

/// @brief  Channel config union for reading and writing from and to EEPROM storage
union ChannelConfigUnion
{
  ChannelConfig data[NUM_CHANNELS];
  byte dataBytes[sizeof(Channels)];
};

union AnalogueConfigUnion
{
  AnalogueInputs data[NUM_ANA_CHANNELS];
  byte dataBytes[sizeof(AnalogueIns)];
};

/// @brief Config storage union for channel data
extern ChannelConfigUnion ChannelConfigData;

/// @brief Config storage union for analogue input data
extern AnalogueConfigUnion AnalogueConfigData;

// Global RTC
extern STM32RTC &rtc;

/// @brief Initialise channel data to known defaults
void InitialiseChannelData();

/// @brief Initialise system data to known
void InitialiseSystemData();

/// @brief Initialise analogue input data to known defaults
void InitialiseAnalogueData();

#endif