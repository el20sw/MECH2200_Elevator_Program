/*
 * File:   main.c
 * Author: samwi
 *
 * Created on 27 January 2022, 14:26
 */

#include "main.h"

void main(void) {
    unsigned int StringStatus;
    unsigned int Value;
    unsigned int SPIAddress;
    unsigned int SPIValue;

    //wait for PLL to stabilise
    while (OSCSTATbits.PLLR == 0);

    InitialisePorts();
    InitialiseComms();
    InitialiseADC();
    InitialiseDAC();
    InitialisePWM_10Bit();
    InitialisePWM_16Bit();
    InitialiseTimers();
    InitialiseSPI();
    InitialiseDRV8711();

    //enable interrupts
    INTCONbits.PEIE = 1; //enable peripheral interrupts
    INTCONbits.GIE = 1; //enable interrupts

    while (1) {
        //display options list
        DisplaySystemOptionsList();

        //test for user input
        StringStatus = GetString(1, GLOBAL_RxString);
        if (StringStatus != STRING_OK) {
            //string error
            DisplayStringError(StringStatus);
        } else {
            //string ok
            //convert to binary
            Value = StringToInteger(GLOBAL_RxString);
            //switch statement to perform requested action
            switch(Value) {
                case 1:     //Run Elevator Diagnostics
                    DiagnosticMenu();
                    break;
                    
                case 2:
                    RunTimeMenu();
                    break;
                    
                default:
                    SendMessage(InvalidNumber);
                    
            }
        }
    }

    return;
}

//*********************************************
//Display Main Menu
void DisplaySystemOptionsList(void) {
    SendMessage(OptionMessage);
    SendMessage(CRLF);
    SendMessage(OptionMessage1);
    SendMessage(OptionMessage2);
    SendMessage(OptionSelectMessage);
}

//*********************************************
//Display Diagnostic Menu
void DisplayDiagnosticMenuList(void) {
    SendMessage(DiagnosticMessage);
    SendMessage(CRLF);
    SendMessage(DiagnosticMessage1);
    SendMessage(DiagnosticMessage2);
    SendMessage(DiagnosticMessage3);
    SendMessage(DiagnosticMessage4);
    SendMessage(OptionSelectMessage);

}

//*********************************************
//Display Run Time Menu
void DisplayRunTimeMenuList(void) {
    SendMessage(RunTimeMessage);
    SendMessage(CRLF);
    SendMessage(RunTimeMessage1);
    SendMessage(RunTimeMessage2);
    SendMessage(RunTimeMessage3);
    SendMessage(ReturnMessage);
    SendMessage(OptionSelectMessage);

}

//*********************************************
//Diagnostic Menu
void DiagnosticMenu(void) {
    
    unsigned int Status = 0;
    unsigned int StringStatus;
    unsigned int Value;
    
    while (Status == 0) {
        //Display Diagnostic Menu
        DisplayDiagnosticMenuList();
        
        //Test user input
        StringStatus = GetString(1, GLOBAL_RxString);
        if (StringStatus != STRING_OK) {
            //string error
            DisplayStringError(StringStatus);
        } else {
            //string ok
            //convert to binary
            Value = StringToInteger(GLOBAL_RxString);
            //switch statement to perform requested action
            switch(Value) {
                case 1:
                    //Test Stepper Motor
                    TestStepperMotor();
                    break;
                    
                case 2:
                    //Test Switches
                    TestSwitch();
                    break;
                    
                case 3:
                    //Direction Calibration
                    CalibrateDirection();
                    break;
                    
                case 4:
                    //Return to Main Menu
                    Status = 1;
                    break;
                    
                default:
                    SendMessage(InvalidNumber);
            }
        }
    }
}

//*********************************************
//RunTime Menu
void RunTimeMenu(void) {
    
    unsigned int Status = 0;
    unsigned int StringStatus;
    unsigned int Value;
    
    while (Status == 0) {
        //Display RunTime Menu
        DisplayRunTimeMenuList();
        
        //Test user input
        StringStatus = GetString(1, GLOBAL_RxString);
        if (StringStatus != STRING_OK) {
            //string error
            DisplayStringError(StringStatus);
        } else {
            //string ok
            //convert to binary
            Value = StringToInteger(GLOBAL_RxString);
            //switch statement to perform requested action
            switch(Value) {
                case 1:
                    //Upwards
                    SendMessage(CRLF);
                    SendMessage(GoingUpMessage);
                    Upwards();
                    break;
                    
                case 2:
                    //Downwards
                    SendMessage(CRLF);
                    SendMessage(GoingDownMessage);
                    Downwards();
                    break;
                            
                case 3:
                    Status = 1;
                    break;
                    
                default:
                    SendMessage(InvalidNumber);
            }
        }
    }
}

//*********************************************
//display string error

void DisplayStringError(unsigned int ErrorValue) {
    switch(ErrorValue)
    {
        case TOO_LONG:  //string is too long
            SendMessage(MessageTooLong);
            break;
            
        case NO_DATA:  //string has no data
            SendMessage(MessageNoValue);
            break;
            
        case INVALID_STRING:  //string has too many decimal points
            SendMessage(TooManyDecimalPoints);
            break;
            
        case VALUE_TOO_LARGE:  //string exceeds maximum value
            SendMessage(TooLarge);
            break;
            
        case VALUE_TOO_SMALL:  //string exceeds minimum value
            SendMessage(TooSmall);
            break;
            
        //No default
    }
}

//*********************************************
//test stepper motor.
//this has a secondary HMI for selecting direction, speed and motor on/off

void TestStepperMotor(void) {
    unsigned int Status = 0;
    unsigned int StringStatus;
    unsigned int Value;
    unsigned int MotorStopStatus;
    
    //set for stepper mode
    SetDRV8711_Mode(STEPPER_MODE);
    
    //set direction to clockwise
    GLOBAL_DirectionStatus = 0;

    //run motor control options menu
    while(Status == 0)
    {
        //display status
        DisplayStepperMotorStatus();
        //display options list
        DisplayStepperMotorOptionsList();

        //test for any string entry
        StringStatus = GetString(1,GLOBAL_RxString);
        if(StringStatus != STRING_OK)
        {
            //string error
            DisplayStringError(StringStatus);
        }
        else
        {
            //string ok
            //convert string to binary value
            Value = StringToInteger(GLOBAL_RxString);
            //test for required action
            switch(Value)
            {
                case 1:     //toggle motor direction
                    if(GLOBAL_DirectionStatus == 0)
                    {
                        //change from clockwise to anticlockwise
                        DRV8711_DIR_WRITE = 0b1;
                        GLOBAL_DirectionStatus = 1;
                    }
                    else
                    {
                        //change from anticlockwise to clockwise
                        DRV8711_DIR_WRITE = 0b0;
                        GLOBAL_DirectionStatus = 0;
                    }
                    break;

                case 2:     //set motor speed
                    SetStepperMotorSpeed();
                    break;

                case 3:     //switch motor on until character received
                    SendMessage(MotorRunningMessage);
                    //stepper is off therefore turn it on
                    MotorOn();
                    //enable the stepper interrupt timer
                    StepperTimerOn();
                    //wait for character to exit
                    MotorStopStatus = 0;
                    while(MotorStopStatus == 0)
                    {
                        Value = GetChar();
                        if(Value != 0xFFFF)
                        {
                            MotorStopStatus = 1;
                        }
                    }
                    //disable the stepper interrupt timer
                    StepperTimerOff();
                    //set the step output to 0
                    DRV8711_STEP_WRITE = 0b0;
                    //switch motor drive off
                    MotorOff();
                    break;

                case 4:     //return to main screen
                    Status = 1;
                    break;

                default:    //invalid entry
                    SendMessage(InvalidNumber);
            }
        }
    }
    //ensure the stepper interrupt timer is off
    StepperTimerOff();
    //set the step output to 0
    DRV8711_STEP_WRITE = 0b0;
    //ensure that motor drive is off
    MotorOff();
    //set stepper direction output to 0 and clockwise
    DRV8711_DIR_WRITE = 0; 
}

//***********************************************
//Set stepper motor speed. load a value between 500 and 9999

void SetStepperMotorSpeed(void) {
    unsigned int    Value;
    unsigned int    StringStatus; 
    
    //send the command string
    SendMessage(StepperMotorSpeedMessage);
    //get the string, maximum 4 characters
    StringStatus = GetString(4,GLOBAL_RxString);
    if(StringStatus != STRING_OK)
    {
        //string error
        DisplayStringError(StringStatus);
    }
    else
    {
        //convert string to binary
        Value = StringToInteger(GLOBAL_RxString);
        //test for value too small
        if(Value < 700)
        {
            DisplayStringError(VALUE_TOO_SMALL);
        }
        else
        {
            //load the speed value into the speed control register
            GLOBAL_StepperMotorSpeed = Value;
        }
    }
}

//*********************************************
//display Stepper motor drive options list

void DisplayStepperMotorOptionsList() {
    SendMessage(StepperOptionMessage);
    SendMessage(MotorOptionMessage1);
    SendMessage(MotorOptionMessage2);
    SendMessage(MotorOptionMessage3);
    SendMessage(MotorOptionMessage4);
    SendMessage(OptionSelectMessage);
}

//*********************************************
//display stepper motor status

void DisplayStepperMotorStatus(void) {
    //send status header message
    SendMessage(StepperMotorStatusMessage);
    
    //send motor direction information
    SendMessage(StepperMotorStatusMessage1);
    if(GLOBAL_DirectionStatus == 0)
    {
        SendMessage(Clockwise);
    }
    else
    {
        SendMessage(AntiClockwise);
    }

    //send motor speed information
    SendMessage(StepperMotorStatusMessage2);
    //convert the integer value into a string
    DecimalToResultString(GLOBAL_StepperMotorSpeed, GLOBAL_ResultString, 4);
    //display the result
    SendString(GLOBAL_ResultString);
}

//*********************************************
//Test Switches

void TestSwitch(void) {
    
    unsigned char GPIO_Status;
    unsigned char OldGPIO_Status;
    unsigned int Status = 0;
    unsigned int Value;
    
    //test message for switch
    SendMessage(TestSwitchMessage);
    
    //get Switch Status
    GPIO_Status = GetGPIO_Status();
    //display Switch Status
    DisplayGPIO_Status(GPIO_Status);
    //save latest switch status
    OldGPIO_Status = GPIO_Status;
    
    //main loop
    while (Status == 0) {
        
        //get any character
        Value = GetChar();
        //test for any characters received
        if (Value == NO_DATA) {
            //get GPIO status
            GPIO_Status = GetGPIO_Status();
            //test for difference
            if (GPIO_Status != OldGPIO_Status) {
                //display GPIO status
                DisplayGPIO_Status(GPIO_Status);
                //save latest value of GPIO status
                OldGPIO_Status = GPIO_Status;
            }
        }
        else {
            //exit request
            Status = 1;
        }
    }
    
}

//*********************************************
//get status of all GPIO and return byte value

unsigned char GetGPIO_Status(void) {
    unsigned char GPIO_Status = 0;
    
    //test each GPIO bit
    if (GPIO_1_READ_PORT == 1)  {GPIO_Status = GPIO_Status | 0b00000001;}
    if (GPIO_2_READ_PORT == 1)  {GPIO_Status = GPIO_Status | 0b00000010;}
    if (GPIO_3_READ_PORT == 1)  {GPIO_Status = GPIO_Status | 0b00000100;}
    if (GPIO_4_READ_PORT == 1)  {GPIO_Status = GPIO_Status | 0b00001000;}
    if (GPIO_5_READ_PORT == 1)  {GPIO_Status = GPIO_Status | 0b00010000;}
    if (GPIO_6_READ_PORT == 1)  {GPIO_Status = GPIO_Status | 0b00100000;}
    if (GPIO_7_READ_PORT == 1)  {GPIO_Status = GPIO_Status | 0b01000000;}
    if (GPIO_8_READ_PORT == 1)  {GPIO_Status = GPIO_Status | 0b10000000;}
    
    //return value
    return GPIO_Status;
}

//*********************************************
//display GPIO status
void DisplayGPIO_Status(unsigned char GPIO_Status) {
    
    SendMessage(CRLF);
    
    //display and check status of GPIO bit
    SendMessage(Switch1_StatusMessage);
    if (GPIO_Status & 0b00000001)  {
        SendMessage(Switch_OFF_Message);
    } else {
        SendMessage(Switch_ON_Message);
    }
    
    SendMessage(Switch2_StatusMessage);
    if (GPIO_Status & 0b00000010)  {
        SendMessage(Switch_OFF_Message);
    } else {
        SendMessage(Switch_ON_Message);
    }
}

void GoingUp(void) {

    //set for stepper mode
    SetDRV8711_Mode(STEPPER_MODE);

    //set direction
    DRV8711_DIR_WRITE = GLOBAL_UpwardsSetting;
    
    //set stepper motor speed
    GLOBAL_StepperMotorSpeed = 700;
    
    //stepper is off therefore turn it on
    MotorOn();
    
    //enable the stepper interrupt timer
    StepperTimerOn();
    
    

}

void GoingDown(void) {

    //set for stepper mode
    SetDRV8711_Mode(STEPPER_MODE);

    //set direction
    DRV8711_DIR_WRITE = GLOBAL_DownwardsSetting;
    
    //set stepper motor speed
    GLOBAL_StepperMotorSpeed = 700;
    
    //stepper is off therefore turn it on
    MotorOn();
    
    //enable the stepper interrupt timer
    StepperTimerOn();
    
    

}

void StepperStop(void) {

    //disable the stepper interrupt timer
    StepperTimerOff();

    //set the step output to 0
    DRV8711_STEP_WRITE = 0b0;

    //switch motor drive off
    MotorOff();
    
}

void Upwards(void) {
    
    unsigned int Status = 1;
    
    while(Status == 1) {
        
        if (GPIO_1_READ_PORT == 0 || GPIO_2_READ_PORT == 0) {
            
            StepperStop();
            Status = 0;
        } else {
            
            GoingUp();
        }
    }
}

void Downwards(void) {
    
    unsigned int Status = 1;
    
    while(Status == 1) {
        
        if (GPIO_1_READ_PORT == 0 || GPIO_2_READ_PORT == 0) {
            
            StepperStop();
            Status = 0;
        } else {
            
            GoingDown();
        }
    }
}

void CalibrateDirection(void) {
    
    unsigned int Status = 0;
    unsigned int StringStatus;
    unsigned int Value;
    
    while (Status == 0) {
        //Display Calibration Menu
        DisplayCalibrateDirectionMenuList();
        
        //Test user input
        StringStatus = GetString(1, GLOBAL_RxString);
        if (StringStatus != STRING_OK) {
            //string error
            DisplayStringError(StringStatus);
        } else {
            //string ok
            //convert to binary
            Value = StringToInteger(GLOBAL_RxString);
            //switch statement to perform requested action
            switch(Value) {
                case 1:
                    ClockwiseCalibration();
                    break;
                    
                case 2:
                    AntiClockwiseCalibration();
                    break;
                    
                case 3:
                    //Return to Main Menu
                    Status = 1;
                    break;
                    
                default:
                    SendMessage(InvalidNumber);
            }
        }
    }
}

void DisplayCalibrateDirectionMenuList(void) {
    
    SendMessage(CalibrationMessage);
    SendMessage(CRLF);
    SendMessage(ClockwiseMessage);
    SendMessage(AntiClockwiseMessage);
    SendMessage(CalibrationMessage1);
    SendMessage(OptionSelectMessage);

}

void DisplayCalibrateSettingList(void) {
    
    SendMessage(CRLF);
    SendMessage(SetUpwardsMessage);
    SendMessage(SetDownwardsMessage);
    SendMessage(OptionSelectMessage);
}

void ClockwiseCalibration(void) {
    
    unsigned int Status = 0;
    unsigned int StringStatus;
    unsigned int Value;
    unsigned int MotorStopStatus;
    
    DisplayCalibrateSettingList();
    
    //set for stepper mode
    SetDRV8711_Mode(STEPPER_MODE);
    
    //set direction to clockwise
    GLOBAL_DirectionStatus = 0;
    DRV8711_DIR_WRITE = 0b0;
    
    //set stepper motor speed
    GLOBAL_StepperMotorSpeed = 1000;

    //run motor control options menu
    while(Status == 0)
    {
        //stepper is off therefore turn it on
        MotorOn();
        
        //enable the stepper interrupt timer
        StepperTimerOn();
        
        //test for any string entry
        StringStatus = GetString(1,GLOBAL_RxString);
        if(StringStatus != STRING_OK)
        {
            //string error
            DisplayStringError(StringStatus);
        }
        else
        {
            //string ok
            //convert string to binary value
            Value = StringToInteger(GLOBAL_RxString);
            //test for required action
            switch(Value)
            {
                case 1:     //Set Clockwise as Upwards
                    GLOBAL_UpwardsSetting = 0b0;
                    StepperStop();
                    Status = 1;
                    break;

                case 2:     //Set Clockwise as Downwards
                    GLOBAL_DownwardsSetting = 0b0;
                    StepperStop();            
                    Status = 1;                   
                    break;

                default:    //invalid entry
                    SendMessage(InvalidNumber);
            }
        }
    }
    
    StepperStop();
    
}

void AntiClockwiseCalibration(void) {
    
    unsigned int Status = 0;
    unsigned int StringStatus;
    unsigned int Value;
    unsigned int MotorStopStatus;
    
    DisplayCalibrateSettingList();
    
    //set for stepper mode
    SetDRV8711_Mode(STEPPER_MODE);
    
    //set direction to anticlockwise
    GLOBAL_DirectionStatus = 0;
    DRV8711_DIR_WRITE = 0b1;
    
    //set stepper motor speed
    GLOBAL_StepperMotorSpeed = 1000;

    //run motor control options menu
    while(Status == 0)
    {
        //stepper is off therefore turn it on
        MotorOn();
        
        //enable the stepper interrupt timer
        StepperTimerOn();
        
        //test for any string entry
        StringStatus = GetString(1,GLOBAL_RxString);
        if(StringStatus != STRING_OK)
        {
            //string error
            DisplayStringError(StringStatus);
        }
        else
        {
            //string ok
            //convert string to binary value
            Value = StringToInteger(GLOBAL_RxString);
            //test for required action
            switch(Value)
            {
                case 1:     //Set AntiClockwise as Upwards
                    GLOBAL_UpwardsSetting = 0b1;
                    StepperStop();
                    Status = 1;
                    break;

                case 2:     //Set AntiClockwise as Downwards
                    GLOBAL_DownwardsSetting = 0b1;
                    StepperStop();                    
                    Status = 1;                   
                    break;

                default:    //invalid entry
                    SendMessage(InvalidNumber);
            }
        }
    }
    
    StepperStop();
    
}