#ifndef _OSC_SURFACE_H_
#define _OSC_SURFACE_H_

// using templates
#include <Math/Vector.h>

// inherits from
#include <Core/IModule.h>
#include <Core/EngineEvents.h>
#include <Renderers/IRenderNode.h>

#include <Utils/Timer.h>

class HeightMap;
namespace OpenEngine {
    namespace Core {
      class DeinitializeEventArg;
      class InitializeEventArg;
      class ProcessEventArg;
    }
    namespace Renderers {
        class IRenderingView;
    }
}

using OpenEngine::Core::IModule;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Core::InitializeEventArg;
using OpenEngine::Core::DeinitializeEventArg;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Math::Vector;
using OpenEngine::Utils::Timer;

class OscSurface : public IModule, public IRenderNode {
private:
  HeightMap* heightMap;

  float   *z_norm;
  double runningTime;

  // rubberPlane
  float   *zCur, *zOld, *zNew;
  double  t;  // time stamp for 'z'
  int     M, N;
  float   aspect;
  float   c;
  double  dt;  // integration time step
  float   taux2, tauy2, tauxy2;

  Vector<3,float> translate;
  float scale;

  Timer timer;

public:
  OscSurface(HeightMap* heightMap);
  ~OscSurface();

  void Handle(InitializeEventArg arg);
  void Handle(ProcessEventArg arg);
  void Handle(DeinitializeEventArg arg);

  virtual void Apply(IRenderingView* rv);
  
  void OnLogicEnter(float timeStep);
  void OnRenderEnter(float timeStep);
  void OnRenderLeave(float timeStep);

  void preFrame(double time);
  void createRipple(float x, float z, float width, float height);
};

#endif
