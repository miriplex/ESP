#include "mbed.h"
#include "motor.h"
#include "TrackSpeed.h"
#include "C12832.h"
#include <cmath>

// For the NUCLEO-F401RE board, choose appropriate pins.
// PA_8 is available as a PWM-capable pin, and PA_9 can be used as a digital output.

#define PWM_PIN1 PB_13 
#define DIR_PIN1 PC_4 // If Unipolar use directioon pins 2 and 5
#define BI_PIN1 PB_0

#define PWM_PIN2 PB_15
#define DIR_PIN2 PB_1 // If Unipolar use directioon pins 2 and 5
#define BI_PIN2 PA_4

#define CH_A_LFT PC_0
#define CH_B_LFT PC_1
#define CH_A_RGT PC_3
#define CH_B_RGT PC_2

#define R_TPR 1030
#define L_TPR 1015



#define ENB_PIN PC_5 // Use Pin 7

// Set pins 1 and 4 low, to enable unipolar mode.

float getTicksRight(TrackSpeed velocityTracker) {
    velocityTracker.reset();
    return velocityTracker.getRightTicks();
}

float getTicksLeft(TrackSpeed velocityTracker) {
    velocityTracker.reset();
    return velocityTracker.getLeftTicks();
}

void go(C12832* lcd, TrackSpeed* velocityTracker, Motor* motor_right, Motor* motor_left, int ticks_forward, int ticks_difference, int* state) {
    // Get current tick counts from encoders
    float right_ticks = velocityTracker->getRightTicks();
    float left_ticks  = velocityTracker->getLeftTicks();
    
    // Calculate normalized forward error for each wheel (1 means far from target, 0 means reached)
    float right_error_frwd = (ticks_forward - right_ticks) / (float)ticks_forward;
    float left_error_frwd  = (ticks_forward - left_ticks) / (float)ticks_forward;
    
    // Calculate turning error based on the difference between wheel ticks.
    // This error represents how far the difference is from the desired ticks_difference.
    float error_difference;
    if (ticks_forward == 0) {
        right_error_frwd = 0;
        left_error_frwd = 0;
    }
    if(ticks_difference == 0) {
        // If no turning target is specified, use the raw difference (inverted)
        error_difference = 0;
    } else if (ticks_difference > 0) {
            error_difference = (ticks_difference - (right_ticks - left_ticks))/ (float)ticks_difference;
    } else {
        error_difference = ((right_ticks - left_ticks) - ticks_difference)/ (float)ticks_difference;
    }
    
    // Proportional gain for turning correction (tune this value as needed)
    float Kp_turn = 0.35;
    float turnCorrection = Kp_turn * error_difference;

    if (turnCorrection > 0) {
        turnCorrection += 0.12f;
    }
    else if (turnCorrection < 0) {
        turnCorrection -= 0.12f;
    }
    
    // Combine forward control with turning correction:
    // Increase the right motor's speed and decrease the left motor's speed (or vice versa)
    float final_right_speed = right_error_frwd + turnCorrection;
    float final_left_speed  = left_error_frwd - turnCorrection;
    
    // Set motor speeds using the adjusted PWM values
    motor_right->setSpeed(final_right_speed);
    motor_left->setSpeed(final_left_speed);

        if (abs(right_error_frwd) < 0.125 && abs(left_error_frwd) < 0.15 && abs(turnCorrection) < 0.14 && abs(velocityTracker->getTranslationalVelocity()) < 0.01 && abs(velocityTracker->getAngularVelocity()) < 0.2) {
        velocityTracker->reset();
        *state += 1;
    }

    lcd->locate(12, 10);
    lcd->printf("%2.3f", left_error_frwd);
    lcd->locate(84, 10);
    lcd->printf("%2.3f", turnCorrection);
}

int main() {
    
C12832 lcd(D11, D13, D12, D7, D10); //Creates an LCD Object from the LCD library

    int state = 1;

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
    float frequency = 5000.0f;
    Motor motor_right(PWM_PIN1, DIR_PIN1, frequency);
    Motor motor_left(PWM_PIN2, DIR_PIN2, frequency);
    motor_right.setSpeed(0);
    motor_left.setSpeed(0);

    while(true) {
        lcd.cls(); //Clear the screen
        switch (state) {
            case 1:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 1900, 0, &state);
                break;
            case 2:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 0, 1250, &state);
                break;
            case 3:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 1900, 0, &state);
                break;
            case 4:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 0, 1250, &state);
                break;
            case 5:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 1900, 0, &state);
                break;
            case 6:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 0, 1250, &state);
                break;
            case 7:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 1900, 0, &state);
                break;
            case 8:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 0, 2500, &state);
                break;
            case 9:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 1900, 0, &state);
                break;
            case 10:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 0, -1250, &state);
                break;
            case 11:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 1900, 0, &state);
                break;
            case 12:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 0, -1250, &state);
                break;
            case 13:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 1900, 0, &state);
                break;
            case 14:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 0, -1250, &state);
                break;
            case 15:
                go(&lcd, &velocityTracker, &motor_right, &motor_left, 1900, 0, &state);
                break;
            default:
                wait(1);
                break;
        }
        // lcd.printf("%d", velocityTracker.getRightTicks());
        wait(0.04f);
    }

}
