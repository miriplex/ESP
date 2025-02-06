#include "motor.h"

Motor::Motor(PinName pwmPin, PinName dirPin, float frequency)
    : _pwm(pwmPin), _dir(dirPin), _frequency(frequency), _speed(0.0f)
{
    // Set the PWM period based on the desired frequency.
    _pwm.period(1.0f / frequency);
    // Initialize motor to stopped state.
    setSpeed(0.0f);
}

void Motor::setSpeed(float speed) {
    // Clamp speed to the range [-1.0, 1.0]
    if(speed > 1.0f) {
        speed = 1.0f;
    } else if(speed < -1.0f) {
        speed = -1.0f;
    }

    // Set direction and adjust duty cycle accordingly.
    if (speed < 0) {
        _dir = 0;              // 0 for reverse (example)
        _pwm.write(-speed);    // PWM duty cycle must be positive.
    } else {
        _dir = 1;              // 1 for forward (example)
        _pwm.write(speed);
    }
    _speed = speed;
}

float Motor::getSpeed() const {
    return _speed;
}

void Motor::setFrequency(float frequency) {
    _pwm.period(1.0f / frequency);
    _frequency = frequency;
}

float Motor::getFrequency() const {
    return _frequency;
}
