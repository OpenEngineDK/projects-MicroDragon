#ifndef _BOID_H_
#define _BOID_H_

#include "../../Config.h"

//include templated classes only
#include <Math/Vector.h>

using OpenEngine::Math::Vector;

class BoidsSystem;
class HeightMap;
class OscSurface;
class BoidFire;

namespace OpenEngine {
    namespace ParticleSystem {
        class ParticleSystem;
    }
    namespace Renderers {
        class TextureLoader;
    }
    namespace Scene {
        class ISceneNode;
        class TransformationNode;
    }
}

using OpenEngine::Scene::ISceneNode;
using OpenEngine::Scene::TransformationNode;

#ifdef DRAGON_SOUND    
namespace OpenEngine {
    namespace Sound {
        class IMonoSound;
    }
}

using namespace OpenEngine::Sound;
#endif

class Boid {
public:
    Boid(HeightMap* heightMap, OscSurface* oscsurface, BoidsSystem* boidssystem, 
         Vector<3,float> position, Vector<3,float> forward,
         Vector<3,float> velocity, Vector<3,float> color,
#ifdef DRAGON_SOUND
         IMonoSound& voice,
#endif
         OpenEngine::ParticleSystem::ParticleSystem& oeparticlesystem,
         OpenEngine::Renderers::TextureLoader& texloader,
         ISceneNode* particleRoot);
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

    Vector<3,float> GetPosition();

private:
    BoidsSystem* boidssystem;
    OscSurface* oscsurface;
    HeightMap* heightMap;
    BoidFire* boidfire;
    TransformationNode* fireTrans;
    ISceneNode* particleRoot;
    OpenEngine::ParticleSystem::ParticleSystem& oeparticlesystem;

    Vector<3,float> color;
    bool airborn;
#ifdef DRAGON_SOUND    
    IMonoSound& voice;
#endif
    void draw2(  bool shadow );

    // accumulated variables. to accumulate force for all boids before applying it
  
    // Steering:
    void updateSteering( double timeDelta );
    Vector<3,float> steering_accumulated; // the directions the boids would like to go
    Vector<3,float> steering_force; // the direction they have strength to go 
  
    // Vehicle model:
    void updatePhysics( double timeDelta );
    float mass, max_force, max_speed;
    Vector<3,float> position, velocity, forward, up, left;
  
    // Additional physics:

    // how they are affected by other boids
    Vector<3,float> external_force_accumulated;
    // explotions
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
