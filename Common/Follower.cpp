#include "Follower.h"

#include <math.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/time.h>
#endif

using namespace std;

Follower::Follower(Vector<3,float> initPos) {
    this->position = initPos;
    this->velocity = Vector<3,float>(0,0,0);
}

Follower::~Follower() {}

void Follower::update(Vector<3,float> target, float acceleration, float speed, float timeStep) {
    if(timeStep<0.0001) return; //@todo: remove this hack, done by cpvc
    float factor = min(1.0f,(float)pow(acceleration,timeStep));
    velocity = velocity*(1-factor)+(target-position)*factor;
    position = position+velocity*speed*timeStep;
}

Vector<3,float> Follower::getPos() { return position; }

Vector<3,float> Follower::getVel() { return velocity; }

void Follower::reset(Vector<3,float> target) {
    position = target;
    velocity = Vector<3,float>(0,0,0);
}
