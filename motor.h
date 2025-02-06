#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"

class Motor {
private:
    PwmOut _pwm;        // PWM output for speed control (built on a Timer peripheral)
    DigitalOut _dir;    // Digital output for setting motor direction
    float _speed;       // Motor's speed
    float _frequency;   // PWM's frequency

public:
    /**
     * Constructor.
     * @param pwmPin Pin used for PWM output (controls speed).
     * @param dirPin Pin used for digital output (controls direction).
     * @param frequency PWM frequency in Hz (default is 1 kHz).
     */
    Motor(PinName pwmPin, PinName dirPin, float frequency = 1000.0f);

    /**
     * Set the motor speed.
     *
     * @param speed Motor speed as a float between -1.0 and 1.0.
     *              Negative values indicate reverse direction.
     *              Zero stops the motor.
     */
    void setSpeed(float speed);

    /**
     * Get the current motor speed.
     *
     * @return Current speed value.
     */
    float getSpeed() const;

    /**
     * Update the PWM frequency.
     *
     * @param frequency New PWM frequency in Hz.
     */
    void setFrequency(float frequency);

    /**
     * Get the current PWM frequency.
     *
     * @return The PWM frequency in Hz.
     */
    float getFrequency() const;
};

#endif // MOTOR_H
