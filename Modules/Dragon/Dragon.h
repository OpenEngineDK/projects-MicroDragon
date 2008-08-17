#ifndef _DRAGON_H_
#define _DRAGON_H_

// using templates
#include <Math/Vector.h>

// inherits from
#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Renderers/IRenderNode.h>

#include <string>
#include <list>

class Follower;
class HeightMap;
class ParticleSystem;
class Target;
class Tube;
namespace OpenEngine {
    namespace Scene {
        class TransformationNode;
    }
    namespace Geometry {
      class Line;
    }
}

/*
#include "Tube.h"
#include "../../Common/Follower.h"
#include <Meta/OpenGL.h>
#include <Scene/TransformationNode.h>
*/

using OpenEngine::Core::IListener;
using OpenEngine::Core::InitializeEventArg;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Scene::TransformationNode;

using OpenEngine::Geometry::Line;
using std::list;
using std::string;

const static int HEAD_DISPLAY_ID = 10;
const static int JAW_DISPLAY_ID  = 11;

class Dragon : public IListener<InitializeEventArg>,
  public IListener<ProcessEventArg>, public IRenderNode {
public:
  bool enabled;

  Dragon(HeightMap* heightMap, Target* target, ParticleSystem* particlesystem);
    ~Dragon();
    void toggleRenderState();

    void Handle(InitializeEventArg arg);
    void Handle(ProcessEventArg arg);

    virtual void Apply(IRenderingView* rv);
  
    void OnLogicEnter(float timeStep);
    float jawPos;
    bool isUsingBreathWeapon();
    void useBreathWeapon( bool input );
    void chargeFireball( bool input );
private:
    HeightMap* heightMap;
    Target* target;
    ParticleSystem* particlesystem;

    TransformationNode* headNode;
    TransformationNode* jawAngleNode;

    Tube* neck;

    unsigned int neckTextureID;

    list<Line*> redlines;
    list<Line*> bluelines;
    list<Line*> greenlines;

    string folder;

    double prevTime;
    float neckLength;
    Follower* headFocus;
    Follower* headPos;
    // Render flags
    unsigned int flags;

    bool enableTexture;
    int renderState, numberOfRenderStates;
    bool usingBreathWeapon, chargingFireball;
    float fireballCharge;
  
    Vector<3,float> fireSource;
    Vector<3,float> fireSourceVel;
    Vector<3,float> fireDir;
};

#endif
