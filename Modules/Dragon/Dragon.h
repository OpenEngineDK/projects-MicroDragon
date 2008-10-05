#ifndef _DRAGON_H_
#define _DRAGON_H_

// using templates
#include <Math/Vector.h>

// inherits from
#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Renderers/IRenderNode.h>
#include <Resources/ITextureResource.h>

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
    namespace Renderers {
        class TextureLoader;
    }
}

using OpenEngine::Core::IListener;
using OpenEngine::Core::InitializeEventArg;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Renderers::TextureLoader;
using OpenEngine::Scene::TransformationNode;

using OpenEngine::Geometry::Line;
using std::list;
using std::string;

using namespace OpenEngine::Resources;

class Dragon : public IListener<InitializeEventArg>,
  public IListener<ProcessEventArg>, public IRenderNode {
public:
  bool enabled;

  Dragon(HeightMap* heightMap, Target* target, ParticleSystem* particlesystem, TextureLoader& textureLoader);
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
    ITextureResourcePtr neckTexture;

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
