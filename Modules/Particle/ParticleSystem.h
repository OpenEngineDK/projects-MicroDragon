#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

// inherits from
#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Renderers/IRenderNode.h>

// using templates
#include <Math/Vector.h>

// usnig typedef
#include <vector>

#include <Utils/Timer.h>

class BoidsSystem;
class HeightMap;
class Particle;
class RandomGenerator;
namespace OpenEngine {
  namespace Display {
    class IViewingVolume;
  }
  namespace Renderers {
    class IRenderingView;
  }
}

using OpenEngine::Core::IListener;
using OpenEngine::Core::InitializeEventArg;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Display::IViewingVolume;
using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Utils::Timer;
using std::vector;

class ParticleSystem : public IListener<InitializeEventArg>,
public IListener<ProcessEventArg>, public IRenderNode {
public:
  ParticleSystem(HeightMap* heightMap, IViewingVolume* vv, BoidsSystem* boidssystem);
    ~ParticleSystem();

    void Handle(InitializeEventArg arg);
    void Handle(ProcessEventArg arg);

    virtual void Apply(IRenderingView* rv);
  
    void CreateParticles(double time, double prevTime, float particlesPerSecond,
                         Vector<3,float> position, Vector<3,float> velocity,
			 float velocityRandomness,
                         double size, double lifeTime);
    void CreateFireball(Vector<3,float> position, Vector<3,float> velocity, float size);

 private:
    IViewingVolume* vv;
    HeightMap* heightMap;
    BoidsSystem* boidssystem;
    RandomGenerator* randObject;
    vector<Particle*> particles;
    vector<Particle*> tmpParticles;

    Timer timer;
};

#endif
