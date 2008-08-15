#ifndef _FIRE_BALL_H_
#define _FIRE_BALL_H_

// inherits from
#include "Particle.h"

class BoidsSystem;
class ParticleSystem;

class FireBall: public Particle {
public:
  FireBall(HeightMap* heightMap, IViewingVolume* vv, BoidsSystem* boidssystem,
	   ParticleSystem* particlesystem, Vector<3,float> position,
	   Vector<3,float> velocity, double size, double lifeTime, 
	   double randomValue);
    ~FireBall();

    void draw();

private:
    BoidsSystem* boidssystem;
    ParticleSystem* particlesystem;
    void updatePhysics( double timeDelta );
};

#endif
