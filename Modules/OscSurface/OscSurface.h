#ifndef _OSC_SURFACE_H_
#define _OSC_SURFACE_H_

// using templates
#include <Math/Vector.h>

// inherits from
#include <Core/IModule.h>
#include <Renderers/IRenderNode.h>

class Island;
namespace OpenEngine {
    namespace Renderers {
        class IRenderingView;
    }
}

using OpenEngine::Core::IModule;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Math::Vector;

class OscSurface : public IModule, public IRenderNode {
private:
  Island* island;

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

public:
  OscSurface(Island* island);
  ~OscSurface();
  void Initialize();
  void Deinitialize();
  void Process(const float deltaTime, const float percent);
  bool IsTypeOf(const std::type_info& inf);

  virtual void Apply(IRenderingView* rv);
  
  void OnLogicEnter(float timeStep);
  void OnRenderEnter(float timeStep);
  void OnRenderLeave(float timeStep);

  void preFrame(double time);
  void createRipple(float x, float z, float width, float height);
};

#endif
