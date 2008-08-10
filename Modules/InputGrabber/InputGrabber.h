#ifndef INPUT_GRABBER_H_
#define INPUT_GRABBER_H_

// inherits from
#include <Core/IModule.h>

// using templates
#include <Math/Vector.h>

class Follower;
class Island;
class Target;

namespace OpenEngine {
  namespace Display {
    class Camera;
  }
  namespace Renderers {
    class IRenderingView;
  }
}

using OpenEngine::Core::IModule;
using OpenEngine::Display::Camera;
using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderingView;

#define minMultiplier 0
#define maxMultiplier 100
#define damper 1.1

class InputGrabber : public IModule {
private:
    Vector<3,float> cameraDir;
    Vector<3,float> cameraPos;

    Camera* camera;
    Island* island;
    Target* target;
    Follower* focus;

    float multiplier;
    float timeFactor;
    bool pauseTime;

    void reset( float rotX, float rotY, float distance );

public:
    InputGrabber(Camera* camera, Island* island, Target* target);
    virtual ~InputGrabber();
    void Initialize();
    void Deinitialize();

    void Process(const float deltaTime, const float percent);
    bool IsTypeOf(const std::type_info& inf);
    void Initialize(IRenderingView* rv);

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
