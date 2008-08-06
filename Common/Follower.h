#ifndef __FOLLOWER_H__
#define __FOLLOWER_H__

#include "Vec3.h"

class Follower
{
public:
    Follower(Vec3 initPos);
    ~Follower();
    void update(Vec3 target, float acceleration, float speed, float timeStep);
    Vec3 getPos();
    Vec3 getVel();
    void reset(Vec3 target);
private:
    Vec3 position, velocity;
};

#endif
