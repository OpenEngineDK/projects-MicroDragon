#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <Math/Vector.h>

using OpenEngine::Math::Vector;

class Island;
namespace OpenEngine {
  namespace Display {
    class IViewingVolume;
  }
}

using OpenEngine::Display::IViewingVolume;

class Particle {
public:
  Particle(Island* island, IViewingVolume* vv, Vector<3,float> position,
	   Vector<3,float> velocity, double size, double lifeTime, 
	   double randomValue);
    virtual ~Particle();
    void addExternalForce( Vector<3,float> force );
    void addExternalImpulse( Vector<3,float> impulse );
    Vector<3,float> getPosition();
    Vector<3,float> getVelocity();
    bool isDead();
    virtual void update( double timeDelta );
	
    virtual void draw();
    bool IsCloserToCamera(Particle* p);
    float DistanceToCamera();

protected:
    Island* island;
    IViewingVolume* vv;
    bool dead;
    double size;
    double lifeTimeLeft;
    double lifeTimeTotal;
    double randomValue;
  
    // Vehicle model:
    virtual void updatePhysics( double timeDelta );
    double mass, max_force, max_speed;
    Vector<3,float> position, velocity, forward, up, left;
  
    // Additional physics:
    Vector<3,float> external_force_accumulated;
    Vector<3,float> external_impulse_accumulated;
};

#endif
