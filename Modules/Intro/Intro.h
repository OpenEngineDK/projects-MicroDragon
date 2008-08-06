#ifndef _INTRO_H_
#define _INTRO_H_

//#include "../InputGrabber.h"
//#include "../../setup.h"
//#include "../../Common/gltga.h"
//#include "../../Common/glm.h"
#include "../../Common/Vec3.h"
#include <Core/IModule.h>
#include <Renderers/IRenderNode.h>
#include <Renderers/IRenderingView.h>
#include <Resources/IModelResource.h>
#include <Meta/OpenGL.h>
#include <Scene/GeometryNode.h>
#include <Scene/TransformationNode.h>

using OpenEngine::Core::IModule;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Scene::GeometryNode;
using OpenEngine::Scene::TransformationNode;

#define GLM_POINTS   (1 << 0)       /* render with only vertices */
#define GLM_FLAT     (1 << 1)       /* render with facet normals */
#define GLM_SMOOTH   (1 << 2)       /* render with vertex normals */
#define GLM_TEXTURE  (1 << 3)       /* render with texture coords */
#define GLM_COLOR    (1 << 4)       /* render with colors */
#define GLM_MATERIAL (1 << 5)       /* render with materials */

class Intro : public IModule, public IRenderNode {
private:
  Intro( /*InputGrabber* input*/ );
  //InputGrabber* input;
    static Intro* instance;
    float blend;
    //GLMmodel* creditModel;

    Vec3 pos;
    float fadeoutTime; //in seconds
    float timepassed;
    bool fadeDone;
    int curDisplayId;
    float startDelay;
    
    TransformationNode* rune;
    TransformationNode* cpvc;
    TransformationNode* tic;
    TransformationNode* cea;

    const static int RUNE_DISPLAY_ID = 13;
    const static int CPVC_DISPLAY_ID = 14;
    const static int TIC_DISPLAY_ID  = 15;
    const static int CEA_DISPLAY_ID  = 16;
    TransformationNode* LoadIntoDisplaylist(string filename, int id);

public:
    static Intro* getInstance();
    //static Intro* getInstance( InputGrabber* input );
    ~Intro();

    bool isDone();
    void disable();

    void Initialize();
    void Deinitialize();
    void Process(const float deltaTime, const float percent);
    bool IsTypeOf(const std::type_info& inf);
    void Initialize(IRenderingView* rv);

    virtual void Apply(IRenderingView* rv);

};

#endif
