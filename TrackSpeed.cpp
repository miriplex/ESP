#include "QEI.h"
#include "TrackSpeed.h"

#define PI 3.14159265358979323846

TrackSpeed::TrackSpeed(float updateTime, 
                       PinName rightChA, PinName rightChB, int rightPulsesPerRev,
                       PinName leftChA,  PinName leftChB,  int leftPulsesPerRev)
    : rightWheel(rightChA, rightChB, NC, rightPulsesPerRev, QEI::X4_ENCODING),
      leftWheel(leftChA, leftChB, NC, leftPulsesPerRev, QEI::X4_ENCODING), leftTicksPerRev(leftPulsesPerRev), rightTicksPerRev(rightPulsesPerRev), updateTime(updateTime)
{
    sampler.attach(callback(this, &TrackSpeed::updateTickCount), updateTime);
}

float TrackSpeed::calculateRWheelVelocity() {
    int tickDifference;
    tickDifference = rightCurrentEncoderTicks - rightPreviousEncoderTicks;
    rSpeed = ((static_cast<float>(tickDifference) / rightTicksPerRev) * (rightWheelDiameter * PI)) / updateTime;
    return rSpeed;
}

float TrackSpeed::calculateLWheelVelocity() {
    int tickDifference;
    tickDifference = leftCurrentEncoderTicks - leftPreviousEncoderTicks;
    lSpeed = ((static_cast<float>(tickDifference) / leftTicksPerRev) * (leftWheelDiameter * PI)) / updateTime;
    return lSpeed;
}

void TrackSpeed::updateTickCount() {
    rightPreviousEncoderTicks = rightCurrentEncoderTicks;
    leftPreviousEncoderTicks = leftCurrentEncoderTicks;
    rightCurrentEncoderTicks = rightWheel.getPulses();
    leftCurrentEncoderTicks = leftWheel.getPulses();
}

void TrackSpeed::setLeftWheelDiameter(float wheelDiameter) {
    leftWheelDiameter = wheelDiameter;
}

void TrackSpeed::setRightWheelDiameter(float wheelDiameter) {
    rightWheelDiameter = wheelDiameter;
}

void TrackSpeed::setDistanceBetweenWheels(float distance) {
    distanceBetweenWheels = distance;
}

float TrackSpeed::getTranslationalVelocity() {
    float rightWheelVelocity = -TrackSpeed::calculateRWheelVelocity();
    float leftWheelVelocity = TrackSpeed::calculateLWheelVelocity();

    return (rightWheelVelocity + leftWheelVelocity) / 2;
}

float TrackSpeed::getAngularVelocity() {
    float rightWheelVelocity = -TrackSpeed::calculateRWheelVelocity();
    float leftWheelVelocity = TrackSpeed::calculateLWheelVelocity();

    return (rightWheelVelocity - leftWheelVelocity) / distanceBetweenWheels;
}

void TrackSpeed::reset() {
    rightWheel.reset();
    leftWheel.reset();
}

int TrackSpeed::getRightTicks() {
    return -rightCurrentEncoderTicks;
}

int TrackSpeed::getLeftTicks() {
    return leftCurrentEncoderTicks;
}
