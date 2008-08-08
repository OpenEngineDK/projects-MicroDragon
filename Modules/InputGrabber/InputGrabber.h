#ifndef INPUT_GRABBER_H_
#define INPUT_GRABBER_H_

#include "../../Common/Vec3.h"
#include "../../Common/Follower.h"
#include "../Target/Target.h"
#include <Display/Camera.h>

using OpenEngine::Core::IModule;
using OpenEngine::Display::Camera;

#define minMultiplier 0
#define maxMultiplier 100
#define damper 1.1

class InputGrabber : public IModule {
public:
    static InputGrabber* getInstance();
    static InputGrabber* getInstance(Camera* camera);

    ~InputGrabber();
    void OnLogicEnter(float timeStep);
    void Initialize();
    void Deinitialize();

    void Process(const float deltaTime, const float percent);
    bool IsTypeOf(const std::type_info& inf);
    void Initialize(IRenderingView* rv);

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
    float rotX, rotXI, rotY, rotYI, distance, distanceI, globalScale, globalScaleI;
protected:
    Follower* focus;
    float multiplier;
    float timeFactor;
    Target* target;
  bool pauseTime;
private:
    Camera* camera;
    static InputGrabber* instance;
    InputGrabber(Camera* camera);
    void reset( float rotX, float rotY, float distance );
};

#endif
