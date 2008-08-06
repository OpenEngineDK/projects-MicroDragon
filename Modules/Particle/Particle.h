#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "../../Common/Vec3.h"

class Particle {
public:
    Particle(Vec3 position, Vec3 velocity, double size, double lifeTime, double randomValue);
    virtual ~Particle();
    void addExternalForce( Vec3 force );
    void addExternalImpulse( Vec3 impulse );
    Vec3 getPosition();
    Vec3 getVelocity();
    bool isDead();
    virtual void update( double timeDelta );
	
    virtual void draw();

protected:
    bool dead;
    double size;
    double lifeTimeLeft;
    double lifeTimeTotal;
    double randomValue;
  
    // Vehicle model:
    virtual void updatePhysics( double timeDelta );
    double mass, max_force, max_speed;
    Vec3 position, velocity, forward, up, left;
  
    // Additional physics:
    Vec3 external_force_accumulated;
    Vec3 external_impulse_accumulated;
};

#endif
