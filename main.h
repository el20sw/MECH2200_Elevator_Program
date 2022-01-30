#ifndef MAIN_H
#define MAIN_H

#include <xc.h>
#include "Init.h"   //this file includes all of the port definitions
#include "Comms.h"  //this file includes comms functions
#include "ADC.h"    //this file includes ADC functions
#include "DAC.h"    //this file includes DAC functions
#include "PWM.h"    //this file includes PWM functions
#include "SPI.h"    //this file includes SPI and DREV8711 functions
#include "Timer.h"  //this file includes timer functions

/*
 * The default state for the CONFIG registers is as follows:
 * CONFI1 register:
 *  FCMEN = ON
 *  IESO = ON
 *  CLKOUTEN = ON (this is inverted logic and is therefore disabled
 *  BOREN = ON (both bits set and is enabled
 *  CP = OFF 
 *  MCLR = ON 
 *  PWRTE = OFF
 *  WDTE = ON
 *  FOSC = ECH
 * 
 * CONFIG2 register
 *  LVP = ON
 *  DEBUG = OFF
 *  LPBOR = OFF
 *  BORV = LO
 *  STVREN = ON
 *  PLLEN = ON
 *  ZCD = OFF
 *  PPS1WAY = ON
 *  WRT = OFF
 * 
 * 
 * On the basis that these are the default settings then we need to consider how we are using the device.
 * The following pragma statements configure the device as required
 *  
 * 
 */

//CONFIG1
#pragma config FCMEN 	= OFF 		// Fail safe clock disabled
#pragma config IESO 	= OFF  		// Internal/external switch over disable
#pragma config MCLRE    = OFF       // MCLR pin is a digital input
#pragma config WDTE     = OFF       // watch dog off
#pragma config FOSC	 	= HS    	// crystal oscillator

//CONFIG2
#pragma config LVP      = OFF       // low voltage programming disabled


//define constants

#define     STEPPER_MOTOR       1
#define     DC_MOTOR            2
#define     LED_STRING_LENGTH   10


//define strings
const unsigned char OptionMessage[] = "\r\n\r\n **** ELEVATOR OPION LIST ****\r\n";
const unsigned char OptionMessage1[] = "1: Run Diagnostics\r\n";
const unsigned char OptionMessage2[] = "2: Run Elevator\r\n";
const unsigned char OptionSelectMessage[] = "\r\nEnter option number: ";
const unsigned char CRLF[] = "\r\n";

//Diagnostics Menu Options
const unsigned char DiagnosticMessage[] = "\r\n\r\n **** Elevator Diagnostics Menu ****\r\n";
const unsigned char DiagnosticMessage1[] = "1: Test stepper motor\r\n";
const unsigned char DiagnosticMessage2[] = "2: Test Switch\r\n";
const unsigned char DiagnosticMessage3[] = "3: Direction Calibration\r\n";
const unsigned char DiagnosticMessage4[] = "4: Test Doors\r\n";
const unsigned char DiagnosticMessage5[] = "5: Return to Main Menu\r\n";

//Run Time Menu Options
const unsigned char RunTimeMessage[] = "\r\n\r\n **** Run Time Menu ****\r\n";
const unsigned char RunTimeMessage1[] = "1: Upwards\r\n";
const unsigned char RunTimeMessage2[] = "2: Downwards\r\n";
const unsigned char RunTimeMessage3[] = "3: Doors Open\r\n";
const unsigned char RunTimeMessage4[] = "4: Doors Closed\r\n";
const unsigned char RunTimeMessage5[] = "-: To Emergency Stop Press the Button\r\n";
const unsigned char ReturnMessage[] = "5: Return to Main Menu\r\n";
const unsigned char GoingUpMessage[] = "Going Up\r\n";
const unsigned char GoingDownMessage[] = "Going Down\r\n";
const unsigned char DoorsOpening[] = "Doors Open\r\n";
const unsigned char DoorsClosing[] = "Doors Closed\r\n";
const unsigned char StopMessage[] = "STOPPED\r\n";

//Test Doors Message
const unsigned char DoorTestMessage1[] = "\r\nDoor Operational\r\n";

//stepper diagnostic options list
const unsigned char StepperOptionMessage[] = "\r\n\r\n **** STEPPER MOTOR TEST OPION LIST ****\r\n";
const unsigned char MotorOptionMessage1[] = " 1: Toggle direction\r\n";
const unsigned char MotorOptionMessage2[] = " 2: Set the motor step interval\r\n";
const unsigned char MotorOptionMessage3[] = " 3: Start motor\r\n";
const unsigned char MotorOptionMessage4[] = " 4: Return to diagnostic menu\r\n";
const unsigned char MotorRunningMessage[] = "\r\n Motor running. Enter any character to stop: ";

//stepper drive status messages
const unsigned char StepperMotorStatusMessage[] = "\r\n\r\n*** STEPPER MOTOR DRIVE STATUS ***\r\n";
const unsigned char StepperMotorStatusMessage1[] = "\r\n      Direction: ";
const unsigned char StepperMotorStatusMessage2[] = "\r\n  Step interval: ";
const unsigned char Clockwise[] = "CLOCKWISE";
const unsigned char AntiClockwise[] = "ANTICLOCKWISE";

//string error messages
const unsigned char MessageTooLong[] = "\r\n String entered is too long";
const unsigned char MessageNoValue[] = "\r\n No Value Entered";
const unsigned char InvalidNumber[] = "\r\n Value out of range";
const unsigned char TooManyDecimalPoints[] = "\r\n Too many decimal points";
const unsigned char TooLarge[] = "\r\n Value too large";
const unsigned char TooSmall[] = "\r\n Value too small";

//relay option message
const unsigned char RelayOptionMessage[] = "\r\n1 = ON, 0 = OFF, X/x = EXIT. Enter value: ";

//PWM test message
const unsigned char PWM_16Bit_TestMessage[] = "\r\n Enter a value between 1000 and 2000. Enter 0 to exit: ";
const unsigned char PWM_10Bit_TestMessage[] = "\r\n Enter a percentage value between 1 and 99. Enter 0 to exit: ";

//Stepper motor speed message
const unsigned char StepperMotorSpeedMessage[] = "\r\n Enter a step interval in microseconds. Value between 700 and 9999: ";

//Switch Testing Messages
const unsigned char TestSwitchMessage[] = "\r\n Switch Test Running. Press any key to exit: ";
const unsigned char Switch1_StatusMessage[] = "\r\n Switch 1 = ";
const unsigned char Switch2_StatusMessage[] = "\r\n Emergency Stop = ";
const unsigned char Switch_ON_Message[] = "ON";
const unsigned char Switch_OFF_Message[] = "OFF";

//Direction Calibration Messages
const unsigned char CalibrationMessage[] = "\r\n\r\n **** Stepper Direction Calibration Menu ****\r\n";
const unsigned char ClockwiseMessage[] = "1: Rotate Stepper Motor Clockwise\r\n";
const unsigned char AntiClockwiseMessage[] = "2: Rotate Stepper Motor Anticlockwise\r\n";
const unsigned char CalibrationMessage1[] = "3: Return to diagnostic menu\r\n";
const unsigned char SetUpwardsMessage[] = "1. Set as Upwards\r\n";
const unsigned char SetDownwardsMessage[] = "2. Set as Downwards\r\n";

//7 segment LED display test messages
const unsigned char LED_DisplayOptionMessage[] = "\r\n\r\n **** LED 7 SEGMENT DISPLAY MODULE TEST OPTION LIST ****\r\n\r\n";
const unsigned char LED_DisplayOptionMessage1[] = " 1: Clear Display\r\n";
const unsigned char LED_DisplayOptionMessage2[] = " 2: Load numerical string from Digit 1 (Left justified)\r\n";
const unsigned char LED_DisplayOptionMessage3[] = " 3: Load numerical string from Digit 2\r\n";
const unsigned char LED_DisplayOptionMessage4[] = " 4: Load numerical string from Digit 3\r\n";
const unsigned char LED_DisplayOptionMessage5[] = " 5: Load numerical string from Digit 4\r\n";
const unsigned char LED_DisplayOptionMessage6[] = " 6: Control LED segments on Digit 1 (Left justified)\r\n";
const unsigned char LED_DisplayOptionMessage7[] = " 7: Control LED segments on Digit 2\r\n";
const unsigned char LED_DisplayOptionMessage8[] = " 8: Control LED segments on Digit 3\r\n";
const unsigned char LED_DisplayOptionMessage9[] = " 9: Control LED segments on Digit 4\r\n";
const unsigned char LED_DisplayOptionMessage10[] = "10: Set DCA value (see manual for decimal control value)\r\n";
const unsigned char LED_DisplayOptionMessage11[] = "11: Set LED intensity\r\n";
const unsigned char LED_DisplayOptionMessage12[] = "12: Return to main menu\r\n";
const unsigned char LED_DisplayClearDisplayMessage[] = "\r\n Clearing Display\r\n";
const unsigned char LED_DisplayEnter4DigitMessage[] = "\r\n Enter up to a four digit numerical string (0 to 9999): ";
const unsigned char LED_DisplayEnter3DigitMessage[] = "\r\n Enter up to a three digit numerical string (0 to 999): ";
const unsigned char LED_DisplayEnter2DigitMessage[] = "\r\n Enter up to a two digit numerical string (0 to 99): ";
const unsigned char LED_DisplayEnter1DigitMessage[] = "\r\n Enter a single digit numerical string (0 to 9): ";
const unsigned char LED_DisplayDCA_Message[] = "\r\n Enter DCA value (0 to 63): ";
const unsigned char LED_DisplayIntensityMessage[] = "\r\n Enter Display Intensity Value (0 to 100): ";
const unsigned char LED_DisplaySegmentControlMessage[] = "\r\n Enter decimal control value (0 to 127): ";

//global variables
volatile unsigned int GLOBAL_TimerEventCounter = 0;
volatile unsigned int GLOBAL_TimerEventFlag = 0;
volatile unsigned int GLOBAL_MasterTimeOutCounter = 0;
volatile unsigned int GLOBAL_MasterTimeOutFlag = 0;
volatile unsigned char GLOBAL_ResultString[RESULT_STRING_LENGTH];
volatile unsigned char GLOBAL_RxString[RX_STRING_LENGTH];
volatile unsigned char GLOBAL_LED_DisplayString[LED_STRING_LENGTH];
volatile unsigned int GLOBAL_PWM1_PulseTime;
volatile unsigned int GLOBAL_PWM2_PulseTime;
volatile unsigned int GLOBAL_PWM3_PulseTime;
volatile unsigned int GLOBAL_PWM4_PulseTime;
volatile unsigned int GLOBAL_StepperMotorSpeed;
volatile unsigned int GLOBAL_DirectionStatus;

volatile unsigned int GLOBAL_UpwardsSetting = 0b0;
volatile unsigned int GLOBAL_DownwardsSetting = 0b1;

//declare functions
void DisplaySystemOptionsList(void);
void DisplayDiagnosticMenuList(void);
void DisplayRunTimeMenuList(void);
void DiagnosticMenu(void);
void RunTimeMenu(void);
void DisplayStringError(unsigned int);
void TestStepperMotor(void);
void SetStepperMotorSpeed(void);
void DisplayStepperMotorOptionsList(void);
void DisplayStepperMotorStatus(void);
void TestSwitch(void);
unsigned char GetGPIO_Status(void);
void DisplayGPIO_Status(unsigned char GPIO_Status);
void GoingUp(void);
void GoingDown(void);
void StepperStop(void);
void Upwards(void);
void Downwards(void);
void CalibrateDirection(void);
void DisplayCalibrateDirectionMenuList(void);
void DisplayCalibrateSettingList(void);
void ClockwiseCalibration(void);
void AntiClockwiseCalibration(void);
void OpenDoors(void);
void CloseDoors(void);
void TestDoors(void);

#endif
