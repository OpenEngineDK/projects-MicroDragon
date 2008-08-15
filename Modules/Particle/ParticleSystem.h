#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

// inherits from
#include <Core/IModule.h>
#include <Renderers/IRenderNode.h>

// using templates
#include <Math/Vector.h>

// usnig typedef
#include <vector>

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

using OpenEngine::Core::IModule;
using OpenEngine::Display::IViewingVolume;
using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;
using std::vector;

class ParticleSystem : public IModule, public IRenderNode {
public:
  ParticleSystem(HeightMap* heightMap, IViewingVolume* vv, BoidsSystem* boidssystem);
    ~ParticleSystem();

    void Initialize();
    void Deinitialize();
    void Process(const float deltaTime, const float percent);
    bool IsTypeOf(const std::type_info& inf);
    void Initialize(IRenderingView* rv);

    virtual void Apply(IRenderingView* rv);
  
    void OnLogicEnter(float timeStep);
    void OnRenderEnter(float timeStep);

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
};

#endif
