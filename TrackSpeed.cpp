#include "QEI.h"
#include "TrackSpeed.h"

#define PI 3.14159265358979323846

TrackSpeed::TrackSpeed(float updateTime, 
                       PinName rightChA, PinName rightChB, int rightPulsesPerRev,
                       PinName leftChA,  PinName leftChB,  int leftPulsesPerRev)
    : rightWheel(rightChA, rightChB, NC, rightPulsesPerRev, QEI::X4_ENCODING),
      leftWheel(leftChA, leftChB, NC, leftPulsesPerRev, QEI::X4_ENCODING), leftTicksPerRev(leftPulsesPerRev), rightTicksPerRev(rightPulsesPerRev)
{
    sampler.attach(callback(this, &TrackSpeed::updateTickCount), updateTime);
}

float TrackSpeed::calculateWheelVelocity(bool selectRightwheel) const {
    int tickDifference;
    float speed;
    if (selectRightwheel) {
        tickDifference = rightCurrentEncoderTicks - rightPreviousEncoderTicks;
        speed = (static_cast<float>(tickDifference) / rightTicksPerRev) * (rightWheelDiameter * PI);
    }
    else {
        tickDifference = leftCurrentEncoderTicks - leftPreviousEncoderTicks;
        speed = (static_cast<float>(tickDifference) / leftTicksPerRev) * (leftWheelDiameter * PI);
    }
    return speed;
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

float TrackSpeed::getTranslationalVelocity() const {
    float rightWheelVelocity = TrackSpeed::calculateWheelVelocity(true);
    float leftWheelVelocity = TrackSpeed::calculateWheelVelocity(false);

    return (rightWheelVelocity + leftWheelVelocity) / 2;
}

float TrackSpeed::getAngularVelocity() const {
    float rightWheelVelocity = TrackSpeed::calculateWheelVelocity(true);
    float leftWheelVelocity = TrackSpeed::calculateWheelVelocity(false);

    return (rightWheelVelocity - leftWheelVelocity) / distanceBetweenWheels;
}