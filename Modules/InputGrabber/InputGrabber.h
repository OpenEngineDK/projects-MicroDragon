#ifndef INPUT_GRABBER_H_
#define INPUT_GRABBER_H_

// inherits from
#include <Core/IListener.h>
#include <Core/EngineEvents.h>

// using templates
#include <Math/Vector.h>

#include <Utils/Timer.h>

class Follower;
class HeightMap;
class Target;

namespace OpenEngine {
  namespace Display {
    class Camera;
  }
  namespace Renderers {
    class IRenderingView;
  }
}

using OpenEngine::Core::IListener;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Core::InitializeEventArg;
using OpenEngine::Display::Camera;
using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Utils::Timer;

#define minMultiplier 0
#define maxMultiplier 100
#define damper 1.1

class InputGrabber : public IListener<InitializeEventArg>,
public IListener<ProcessEventArg> {
private:
    Vector<3,float> cameraDir;
    Vector<3,float> cameraPos;

    Camera* camera;
    HeightMap* heightMap;

    Target* target;
    Follower* focus;

    Timer timer;

    float multiplier;
    float timeFactor;
    bool pauseTime;

    void reset( float rotX, float rotY, float distance );

public:
    InputGrabber(Camera* camera, HeightMap* heightMap, Target* target);
    virtual ~InputGrabber();
    void Initialize();
    void Deinitialize();

    void Handle(InitializeEventArg arg);
    void Handle(ProcessEventArg arg);

    void printLocation();
    void reset();
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
    Vector<3,float> GetCameraDir();
};

#endif
