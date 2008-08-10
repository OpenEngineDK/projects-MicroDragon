#ifndef _BOID_H_
#define _BOID_H_

//include templated classes only
#include <Math/Vector.h>

using OpenEngine::Math::Vector;

class BoidsSystem;
class Island;
class OscSurface;
class ParticleSystem;

class Boid {
public:
    Boid(Island* island, OscSurface* oscsurface, BoidsSystem* boidssystem, 
	 Vector<3,float> position, Vector<3,float> forward,
	 Vector<3,float> velocity, Vector<3,float> color);
    ~Boid();
    void addDesiredVelocity( Vector<3,float> steering, double weight );
    void addSteering( Vector<3,float> steering, double weight );
    void addExternalForce( Vector<3,float> force );
    void addExternalImpulse( Vector<3,float> impulse );
    void handleBoid( Boid* boid );
    void HandleFire(Vector<3,float> firePosition, float fireStrength);
    void gotoTarget( Vector<3,float> target, double radius, double weight, bool squared );
    Vector<3,float> getPosition();
    Vector<3,float> getVelocity();
    void update( double timeDelta );
	
    void draw( );

private:
    ParticleSystem* particlesystem;
    BoidsSystem* boidssystem;
    OscSurface* oscsurface;
    Island* island;

    Vector<3,float> color;
    bool airborn;

    void draw2(  bool shadow );
  
    // Steering:
    void updateSteering( double timeDelta );
    Vector<3,float> steering_accumulated;
    Vector<3,float> steering_force;
  
    // Vehicle model:
    void updatePhysics( double timeDelta );
    double mass, max_force, max_speed;
    Vector<3,float> position, velocity, forward, up, left;
  
    // Additional physics:
    Vector<3,float> external_force_accumulated;
    Vector<3,float> external_impulse_accumulated;
  
    // Locomotion
    void updateLocomotion( double timeDelta );
    Vector<3,float> getUprightVector();
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
