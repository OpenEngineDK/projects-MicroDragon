#ifndef _OSC_SURFACE_H_
#define _OSC_SURFACE_H_

#include <Core/IModule.h>
#include <Renderers/IRenderNode.h>
#include <Renderers/IRenderingView.h>
#include "../../Common/Vec3.h"

using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Core::IModule;
using OpenEngine::Renderers::IRenderingView;

class OscSurface : public IModule, public IRenderNode {
private:
  OscSurface();
  static OscSurface* instance;

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

  Vec3 translate;
  float scale;

public:
  static OscSurface* getInstance();
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
