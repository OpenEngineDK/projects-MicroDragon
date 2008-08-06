#include "Follower.h"

#include <Logging/Logger.h>

#include <math.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/time.h>
#endif

Follower::Follower(Vec3 initPos) {
    this->position = initPos;
    this->velocity = Vec3(0,0,0);
    logger.info << "init pos: " << initPos << logger.end;
}

Follower::~Follower() {}

void Follower::update(Vec3 target, float acceleration, float speed, float timeStep) {
    if(timeStep<0.0001) return; //@todo: remove this hack, done by cpvc
    float factor = min(1.0f,(float)pow(acceleration,timeStep));
    velocity = velocity*(1-factor)+(target-position)*factor;
    position = position+velocity*speed*timeStep;
}

Vec3 Follower::getPos() { return position; }

Vec3 Follower::getVel() { return velocity; }

void Follower::reset(Vec3 target) {
    position = target;
    velocity = Vec3(0,0,0);
}
