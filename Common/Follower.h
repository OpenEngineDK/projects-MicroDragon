#ifndef __FOLLOWER_H__
#define __FOLLOWER_H__

#include <Math/Vector.h>

using OpenEngine::Math::Vector;

class Follower
{
public:
    Follower(Vector<3,float> initPos);
    ~Follower();
    void update(Vector<3,float> target, float acceleration, float speed, float timeStep);
    Vector<3,float> getPos();
    Vector<3,float> getVel();
    void reset(Vector<3,float> target);
private:
    Vector<3,float> position, velocity;
};

#endif
