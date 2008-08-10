#ifndef _BOIDS_SYSTEM_H_
#define _BOIDS_SYSTEM_H_

//inherits from
#include <Core/IModule.h>
#include <Renderers/IRenderNode.h>

//templated classes
#include <Math/Vector.h>

//forward reference
class Boid;
class Island;
class OscSurface;
namespace OpenEngine {
    namespace Renderers {
        class IRenderingView;
    }
}

using OpenEngine::Core::IModule;
using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;

#define numberOfBoids 49 //must be a square number, 9, 16, 25, 36...

class BoidsSystem : public IModule, public IRenderNode {
public:
  bool enabled;
    BoidsSystem(Island* island, OscSurface* oscsurface);
    ~BoidsSystem();
    void toggleRenderState();

    void Initialize();
    void Deinitialize();
    void Process(const float deltaTime, const float percent);
    bool IsTypeOf(const std::type_info& inf);
    void Initialize(IRenderingView* rv);

    virtual void Apply(IRenderingView* rv);
  
    void HandleFire(Vector<3,float> position, float strength);
    void HandleFireball(Vector<3,float> position, float strength);
    void IncNumberOfShownBoids();
    void DecNumberOfShownBoids();
    void IncAlignment();
    void DecAlignment();
    float getAlignment();

private:
    Island* island;
    OscSurface* oscsurface;

    unsigned int numberOfShownBoids;
    float alignment;
    Boid* boids[numberOfBoids];

    bool disableLogic;
    int renderState, numberOfRenderStates;
};

#endif
