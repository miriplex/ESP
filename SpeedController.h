#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

#include "mbed.h"

/**
 * @brief Class to control the speed and turning of the buggy using PD control.
 *
 * The controller uses two independent PD controllers:
 *   - One to control the overall forward speed.
 *   - One to control the turning (angular velocity).
 *
 * The computed control signals are combined to produce left and right motor outputs.
 */
class SpeedController {
public:
    /**
     * @brief Constructor.
     *
     * @param Kp_speed Proportional gain for speed.
     * @param Kd_speed Derivative gain for speed.
     * @param Kp_turn Proportional gain for turning.
     * @param Kd_turn Derivative gain for turning.
     */
    SpeedController(float Kp_speed, float Kd_speed, float Kp_turn, float Kd_turn);

    /**
     * @brief Set the desired forward speed (m/s).
     * @param speed Desired speed.
     */
    void setDesiredSpeed(float speed);

    /**
     * @brief Set the desired turning rate (rad/s). A positive value may correspond to turning right.
     * @param turnRate Desired angular velocity.
     */
    void setDesiredTurnRate(float turnRate);

    /**
     * @brief Update the controller with current measurements.
     *
     * This function computes new control outputs based on the measured speed and turn rate.
     * It outputs motor control values that can be applied to the left and right motors.
     *
     * @param currentSpeed Measured forward speed (m/s).
     * @param currentTurnRate Measured turning rate (rad/s).
     * @param leftOutput Reference to store computed left motor control signal.
     * @param rightOutput Reference to store computed right motor control signal.
     */
    void update(float currentSpeed, float currentTurnRate, float &leftOutput, float &rightOutput);

private:
    // PD gains for speed and turning controllers.
    float _Kp_speed;
    float _Kd_speed;
    float _Kp_turn;
    float _Kd_turn;

    // Desired setpoints.
    float _desiredSpeed;
    float _desiredTurnRate;

    // Previous errors for derivative calculation.
    float _prevSpeedError;
    float _prevTurnError;

    // Timer to compute dt between updates.
    Timer _timer;
    float _lastUpdateTime;
};

#endif // SPEEDCONTROLLER_H
