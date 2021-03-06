
#include "Control/Navigator.h"


Navigator::Navigator(double iWaypointRadius, double iWaypointAngle):mActualPose(Pose::ZERO),mGoalPose(Pose::ZERO),
    mIsNewGoalReady(false),mWaypointRadius(iWaypointRadius),mWaypointAngle(iWaypointAngle){
}

Navigator::~Navigator(){
}

//return true if goal is reached
bool Navigator::isPositionGoalReached(){
    double lDistanceFromGoal = (mGoalPose.Position - mActualPose.Position).length();
    return lDistanceFromGoal < mWaypointRadius;
}

bool Navigator::isAngleGoalReached(){
    double lDif = _angleBtwTwoAngles(mGoalPose.Angle.getPolar(),
                                     mActualPose.Angle.getPolar());
    if(std::abs(lDif) < mWaypointAngle){
        return true;
    }
    else{
        return false;
    }
}

Vector2d Navigator::calculateNewVector(){

    Vector2d lVect = mGoalPose.Position - mActualPose.Position;
    double lSpeed = _calculateSpeed(mGoalPose.Speed,
                                    mActualPose.Speed,
                                    lVect.length());

    return  lVect / lVect.length() * lSpeed;
}

Orientation Navigator::calculateNewOrientation(){

    double lAngleSpeed;
    double lDif = _angleBtwTwoAngles(mGoalPose.Angle.getPolar(),
                                     mActualPose.Angle.getPolar());

    if(lDif > 0){
        lAngleSpeed = 1.5;
    }
    else{
        lAngleSpeed = -1.5;
    }
    return Orientation(lAngleSpeed);
}

double Navigator::_angleBtwTwoAngles(double iangle1, double iangle2){
    double lDif = iangle1 - iangle2;
    if(lDif >= M_PI){
        lDif -= 2 * M_PI;
    }
    else if(lDif <= - M_PI){
        lDif += 2 * M_PI;
    }
    return lDif;
}

double Navigator::_calculateSpeed(double iGoalTargetSpeed, double iCurrentSpeed, double distance){
    // If ditance is in the breaking range (0c3m)
    if(distance <= 300){
        double newTargetSpeed = distance/300 /(1 - iGoalTargetSpeed) + iGoalTargetSpeed;
        if(newTargetSpeed < 0.001)
            newTargetSpeed = 0;
        return newTargetSpeed;
    }
    else // Maximun speed
        return 1.0;
}






