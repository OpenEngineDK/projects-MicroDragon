#ifndef _BOID_H_
#define _BOID_H_

//#include "../../setup.h"
#include "../../Common/Vec3.h"

class Boid {
public:
    Boid(Vec3 position, Vec3 forward, Vec3 velocity, Vec3 color);
    ~Boid();
    void addDesiredVelocity( Vec3 steering, double weight );
    void addSteering( Vec3 steering, double weight );
    void addExternalForce( Vec3 force );
    void addExternalImpulse( Vec3 impulse );
    void handleBoid( Boid* boid );
    void HandleFire(Vec3 firePosition, float fireStrength);
    void gotoTarget( Vec3 target, double radius, double weight, bool squared );
    Vec3 getPosition();
    Vec3 getVelocity();
    void update( double timeDelta );
	
    void draw( );

private:
    Vec3 color;
    bool airborn;

    void draw2(  bool shadow );
  
    // Steering:
    void updateSteering( double timeDelta );
    Vec3 steering_accumulated;
    Vec3 steering_force;
  
    // Vehicle model:
    void updatePhysics( double timeDelta );
    double mass, max_force, max_speed;
    Vec3 position, velocity, forward, up, left;
  
    // Additional physics:
    Vec3 external_force_accumulated;
    Vec3 external_impulse_accumulated;
  
    // Locomotion
    void updateLocomotion( double timeDelta );
    Vec3 getUprightVector();
    double running, walkCycle;
  
    // Effects
    float burned;
    float hot;
    bool burning;
    bool drowning;
    float life;
    bool dead;

    double prevTime;
};

#endif
