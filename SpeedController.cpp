#include "SpeedController.h"

SpeedController::SpeedController(float Kp_speed, float Kd_speed, float Kp_turn, float Kd_turn)
    : _Kp_speed(Kp_speed), _Kd_speed(Kd_speed), _Kp_turn(Kp_turn), _Kd_turn(Kd_turn),
      _desiredSpeed(0.0f), _desiredTurnRate(0.0f), _prevSpeedError(0.0f), _prevTurnError(0.0f),
      _lastUpdateTime(0.0f)
{
    _timer.start();
    _lastUpdateTime = _timer.read();
}

void SpeedController::setDesiredSpeed(float speed) {
    _desiredSpeed = speed;
}

void SpeedController::setDesiredTurnRate(float turnRate) {
    _desiredTurnRate = turnRate;
}

void SpeedController::update(float currentSpeed, float currentTurnRate, float &leftOutput, float &rightOutput) {
    // Get the current time and compute the time delta.
    float currentTime = _timer.read();
    float dt = currentTime - _lastUpdateTime;
    if (dt <= 0.0f) {
        dt = 0.001f; // Prevent division by zero
    }

    // Compute errors.
    float speedError = _desiredSpeed - currentSpeed;
    float turnError  = _desiredTurnRate - currentTurnRate;

    // Compute derivative terms.
    float dSpeedError = (speedError - _prevSpeedError) / dt;
    float dTurnError  = (turnError - _prevTurnError) / dt;

    // Compute PD control outputs.
    float u_speed = _Kp_speed * speedError + _Kd_speed * dSpeedError;
    float u_turn  = _Kp_turn  * turnError  + _Kd_turn  * dTurnError;

    // Combine speed and turning control signals.
    // For example: adding u_turn to the left motor and subtracting from the right motor
    // produces a right turn.
    leftOutput  = u_speed + u_turn;
    rightOutput = u_speed - u_turn;

    // Save errors and time for the next update.
    _prevSpeedError = speedError;
    _prevTurnError  = turnError;
    _lastUpdateTime = currentTime;
}
