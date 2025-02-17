#include "mbed.h"
#include "motor.h"
#include "TrackSpeed.h"
#include "C12832.h"

// For the NUCLEO-F401RE board, choose appropriate pins.
// PA_8 is available as a PWM-capable pin, and PA_9 can be used as a digital output.
/*
#define PWM_PIN PB_14
#define DIR_PIN PC_4

#define PWM_PIN PB_13
#define DIR_PIN PB_1

DigitalOut enable(PC_5);
*/
#define PWM_PIN1 PB_13 
#define DIR_PIN1 PC_4 // If Unipolar use directioon pins 2 and 5
#define BI_PIN1 PB_0

#define PWM_PIN2 PB_15
#define DIR_PIN2 PB_1 // If Unipolar use directioon pins 2 and 5
#define BI_PIN2 PA_4

#define CH_A_LFT PC_0
#define CH_A_RGT PC_3
#define CH_B_LFT PC_1
#define CH_B_RGT PC_3

#define R_TPR 1030
#define L_TPR 1015



#define ENB_PIN PC_5 // Use Pin 7

// Set pins 1 and 4 low, to enable unipolar mode.

int main() {
    
C12832 lcd(D11, D13, D12, D7, D10); //Creates an LCD Object from the LCD library


    DigitalOut enable(ENB_PIN);
    DigitalOut biMode1(BI_PIN1);
    DigitalOut biMode2(BI_PIN2);

    TrackSpeed velocityTracker(0.1f, CH_A_RGT, CH_B_RGT, R_TPR, CH_A_LFT, CH_B_LFT, L_TPR);

    velocityTracker.setLeftWheelDiameter(0.083f);
    velocityTracker.setRightWheelDiameter(0.083f);

    velocityTracker.setDistanceBetweenWheels(0.1872f);


    enable = 1;
    biMode1 = 0;
    biMode2 = 0;

     // Create a Motor object with an initial PWM frequency of 1 kHz.
    float frequency = 500.0f;
    Motor myMotor1(PWM_PIN1, DIR_PIN1, frequency);
    Motor myMotor2(PWM_PIN2, DIR_PIN2, frequency);
    float speed = 1;
    myMotor1.setSpeed(0);
    myMotor2.setSpeed(0);
    while (true) {
        lcd.cls(); //Clear the screen
        lcd.locate(12, 10);
        lcd.printf("%d", velocityTracker.getTranslationalVelocity());
        // lcd.locate(84, 10);
        // lcd.printf("BYE");//Print 
        wait(1);
    }    /*
    while(true) {
    myMotor1.setSpeed(0);
    myMotor2.setSpeed(0);
    wait(5);
    // myMotor1.setSpeed(0.5f);
    myMotor2.setSpeed(0.5f);
    wait(5);
    }
    */
}
