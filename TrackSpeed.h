#ifndef TRACKSPEED_H
#define TRACKSPEED_H

#include "mbed.h"
#include "QEI.h"

class TrackSpeed {
private:
    volatile int rightCurrentEncoderTicks;
    volatile int rightPreviousEncoderTicks;
    volatile int leftCurrentEncoderTicks;
    volatile int leftPreviousEncoderTicks;
    int rightTicksPerRev;
    int leftTicksPerRev;
    float rightWheelDiameter;
    float leftWheelDiameter;
    float distanceBetweenWheels;
    float updateTime;
    float rSpeed;
    float lSpeed;
    float calculateRWheelVelocity();
    float calculateLWheelVelocity();
    void updateTickCount();
    QEI rightWheel;
    QEI leftWheel;
    Ticker sampler;


public:
    /**
     * Constructor.
     * @param channelA Channel A of the encoder
     * @param channelB Channel B of the encoder
     * @param index I have no idea what this is
     * @param updateTime The delta time between measurements of the tick rate
     */
    TrackSpeed(float updateTime, 
               PinName rightChA, PinName rightChB, int rightPulsesPerRev,
               PinName leftChA,  PinName leftChB,  int leftPulsesPerRev);

    void setLeftWheelDiameter(float wheelDiameter);

    void setRightWheelDiameter(float wheelDiameter);

    void setDistanceBetweenWheels(float distance);


    /**
     * Get Robot's translational Velocity
     *
     * @return Buggy's translational velocity in meters 
     */
    float getTranslationalVelocity();

        /**
     * Get Robot's angular Velocity
     *
     * @return Buggy's angular velocity in meters 
     */
    float getAngularVelocity();

    /**
     * Update Update time fo the buggy
     *
     * @param time New update time in seconds.
     */
    void setUpdateTime(float time);

    void reset();

    int getRightTicks();

    int getLeftTicks();


};

#endif // TRACKSPEED_H
