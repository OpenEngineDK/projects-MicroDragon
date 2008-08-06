#ifndef _FIRE_BALL_H_
#define _FIRE_BALL_H_

#include "Particle.h"

class FireBall: public Particle {
public:
    FireBall(Vec3 position, Vec3 velocity, double size, double lifeTime, double randomValue);
    ~FireBall();
	
    void draw();

private:
    void updatePhysics( double timeDelta );
};

#endif
