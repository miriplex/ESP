#include "mbed.h"
#include "motor.h"

// For the NUCLEO-F401RE board, choose appropriate pins.
// PA_8 is available as a PWM-capable pin, and PA_9 can be used as a digital output.
#define PWM_PIN PA_8
#define DIR_PIN PA_9

int main() {
     // Create a Motor object with an initial PWM frequency of 1 kHz.
    float frequency = 1000.0f;
    Motor myMotor(PWM_PIN, DIR_PIN, frequency);
    myMotor.setSpeed(0.5f);
    wait(2);

    while (true) {
        myMotor.setSpeed(0);
        wait(1);
        myMotor.setSpeed(0.5f);
        frequency += 1000.0f;
        myMotor.setFrequency(frequency);
        wait(2);
    }
}
