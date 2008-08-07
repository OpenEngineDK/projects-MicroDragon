#ifndef INPUT_GRABBER_H_
#define INPUT_GRABBER_H_

#include "../../Common/Vec3.h"
#include "../../Common/Follower.h"
#include "../Target/Target.h"
#include <Renderers/IRenderNode.h>
#include <Display/IViewingVolume.h>

using OpenEngine::Core::IModule;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Display::IViewingVolume;

#define minMultiplier 0
#define maxMultiplier 100
#define damper 1.1

class InputGrabber : public IModule, public IRenderNode {
public:
    static InputGrabber* getInstance();
    static InputGrabber* getInstance(IViewingVolume* vv);

    ~InputGrabber();
    void OnLogicEnter(float timeStep);
    void OnRenderEnter(float timeStep);
    void OnRenderLeave(float timeStep);
    void Initialize();
    void Deinitialize();

    void Process(const float deltaTime, const float percent);
    bool IsTypeOf(const std::type_info& inf);
    void Initialize(IRenderingView* rv);

    virtual void Apply(IRenderingView* rv);  

    void printLocation();
    virtual void reset();
    Vec3 getTarget();
    void moveTarget( float x, float z );
    void rotateViewRelative( float x, float y );
    void rotateViewAbsolute( float x, float y, float distance, float globalScale );
    void incMultiplier();
    void decMultiplier();
    void zoom(float speed);
    void updateDistance( float distance);
    void scaleGlobal(float scale);
  bool isTimePaused();
  void togglePause();
    float	rotX, rotXI, rotY, rotYI, rotZ, distance, distanceI, globalScale, globalScaleI;
protected:
    Follower* focus;
    float multiplier;
    float timeFactor;
    Target* target;
  bool pauseTime;
private:
    IViewingVolume* vv;
    static InputGrabber* instance;
    InputGrabber(IViewingVolume* vv);
    void reset( float rotX, float rotY, float rotZ, float distance );
};

#endif
