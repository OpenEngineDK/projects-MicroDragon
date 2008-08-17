#ifndef _INTRO_H_
#define _INTRO_H_

// inherits from
#include <Renderers/IRenderNode.h>
#include <Core/IListener.h>
#include <Core/EngineEvents.h>

// using templates
#include <Math/Vector.h>

// typedefs
#include <string>

//forward reference
class Boid;
class InputGrabber;
namespace OpenEngine {
    namespace Renderers {
        class IRenderingView;
    }
    namespace Scene {
        class TransformationNode;
    }
}

using OpenEngine::Core::IListener;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Scene::TransformationNode;
using std::string;

class Intro : public IListener<ProcessEventArg>, public IRenderNode {
private:
    InputGrabber* inputgrabber;
    float blend;

    Vector<3,float> pos;
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
    Intro(InputGrabber* inputgrabber);
    ~Intro();

    bool isDone();
    void disable();

    void Handle(ProcessEventArg arg);

    virtual void Apply(IRenderingView* rv);
};

#endif
