#ifndef _BOIDS_SYSTEM_H_
#define _BOIDS_SYSTEM_H_

#include "../../Common/Vec3.h"
#include "Boid.h"

#include <Renderers/IRenderNode.h>
#include <Renderers/IRenderingView.h>
#include <Resources/IModelResource.h>

using OpenEngine::Core::IModule;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;

#define numberOfBoids 49 //must be a square number, 9, 16, 25, 36...

class BoidsSystem : public IModule, public IRenderNode {
public:
  bool enabled;
    static BoidsSystem* getInstance();
    ~BoidsSystem();
    void toggleRenderState();

    void Initialize();
    void Deinitialize();
    void Process(const float deltaTime, const float percent);
    bool IsTypeOf(const std::type_info& inf);
    void Initialize(IRenderingView* rv);

    virtual void Apply(IRenderingView* rv);
  
    void OnLogicEnter(float timeStep);
    void OnRenderEnter(float timeStep);
    void HandleFire(Vec3 position, float strength);
    void HandleFireball(Vec3 position, float strength);
    void IncNumberOfShownBoids();
    void DecNumberOfShownBoids();
    void IncAlignment();
    void DecAlignment();
    float getAlignment();

private:
    unsigned int numberOfShownBoids;
    float alignment;
    BoidsSystem();
    static BoidsSystem* instance;
    Boid* boids[numberOfBoids];

    bool disableLogic;
    int renderState, numberOfRenderStates;
};

#endif
