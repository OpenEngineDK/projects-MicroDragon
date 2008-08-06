#ifndef _DRAGON_H_
#define _DRAGON_H_

#include "Tube.h"

#include "../../Common/Vec3.h"
#include "../../Common/Follower.h"
/*
#include "../../Common/glm.h"
#include "../../Common/gltga.h"
*/

#include <Core/IModule.h>
#include <Renderers/IRenderNode.h>
#include <Meta/OpenGL.h>
#include <Scene/TransformationNode.h>

using OpenEngine::Core::IModule;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Scene::TransformationNode;

const static int HEAD_DISPLAY_ID = 10;
const static int JAW_DISPLAY_ID  = 11;

class Dragon : public IModule, public IRenderNode {
public:
  bool enabled;
    static Dragon* getInstance();
    ~Dragon();
    void toggleRenderState();

    void Initialize();
    void Deinitialize();
    void Process(const float deltaTime, const float percent);
    bool IsTypeOf(const std::type_info& inf);
    void Initialize(IRenderingView* rv);

    virtual void Apply(IRenderingView* rv);
  
    void OnLogicEnter(float timeStep);
    float jawPos;
    bool isUsingBreathWeapon();
    void useBreathWeapon( bool input );
    void chargeFireball( bool input );
private:
    TransformationNode* headNode;
    TransformationNode* jawAngleNode;

    Dragon();

    static Dragon* instance;
    Tube* neck;

    unsigned int neckTextureID;

    double prevTime;
    float neckLength;
    Follower* headFocus;
    Follower* headPos;
    // Render flags
    GLuint flags;

    bool enableTexture;
    int renderState, numberOfRenderStates;
    bool usingBreathWeapon, chargingFireball;
    float fireballCharge;
  
    Vec3 fireSource;
    Vec3 fireSourceVel;
    Vec3 fireDir;
};

#endif
