#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include "Particle.h"

#include "../../Common/Vec3.h"
#include "../../Common/random.h"

#include <Core/IModule.h>
#include <Renderers/IRenderNode.h>
#include <Renderers/IRenderingView.h>

using OpenEngine::Core::IModule;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;

class ParticleSystem : public IModule, public IRenderNode {
public:
    static ParticleSystem* getInstance();
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
                         Vec3 position, Vec3 velocity, float velocityRandomness,
                         double size, double lifeTime);
    void CreateFireball(Vec3 position, Vec3 velocity, float size);

private:
    ParticleSystem();
    static ParticleSystem* instance;
    RandomGenerator* randObject;
    vector<Particle*> particles;
    vector<Particle*> tmpParticles;
};

#endif
