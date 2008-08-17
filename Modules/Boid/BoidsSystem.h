#ifndef _BOIDS_SYSTEM_H_
#define _BOIDS_SYSTEM_H_

//inherits from
#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Core/IEvent.h>
#include <Core/Event.h>
#include <Renderers/IRenderNode.h>

//templated classes
#include <Math/Vector.h>

// non reference or pointer member variables
#include <Utils/Timer.h>

//forward reference
class Boid;
class HeightMap;
class OscSurface;
namespace OpenEngine {
    namespace Renderers {
        class IRenderingView;
    }
}

using OpenEngine::Core::IListener;
using OpenEngine::Core::InitializeEventArg;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Core::IEvent;
using OpenEngine::Core::Event;
using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Utils::Timer;

struct BoidSystemEventArg;
struct ParticleSystemEventArg;

#define numberOfBoids 49 //must be a square number, 9, 16, 25, 36...

class BoidsSystem : public IListener<InitializeEventArg>,
  public IListener<ProcessEventArg>, public IRenderNode, 
public IListener<ParticleSystemEventArg> {
public:
  unsigned int aliveBoids;
  bool enabled;
    BoidsSystem(HeightMap* heightMap, OscSurface* oscsurface);
    ~BoidsSystem();
    void toggleRenderState();

    void Handle(InitializeEventArg arg);
    void Handle(ProcessEventArg arg);

    void ResetBoids();

    virtual void Apply(IRenderingView* rv);
  
    void Handle(ParticleSystemEventArg arg);
    void HandleFire(Vector<3,float> position, float strength);
    void IncNumberOfShownBoids();
    void DecNumberOfShownBoids();
    void IncAlignment();
    void DecAlignment();
    float getAlignment();

    void BoidDied(Boid& boid);
    virtual IEvent<BoidSystemEventArg>& BoidSystemEvent();

private:
    Event<BoidSystemEventArg> boidEvents;


    HeightMap* heightMap;
    OscSurface* oscsurface;

    Timer timer;

    unsigned int numberOfShownBoids;
    float alignment;
    Boid* boids[numberOfBoids];

    bool disableLogic;
    int renderState, numberOfRenderStates;
};

#endif
