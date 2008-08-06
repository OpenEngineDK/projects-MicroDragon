#ifndef _TARGET_H_
#define _TARGET_H_

#include "../../Common/Vec3.h"
#include <Core/IModule.h>
#include <Renderers/IRenderNode.h>

using OpenEngine::Core::IModule;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;

class Target : public IModule, public IRenderNode {
private:
  Target();
  static Target* instance;
  Vec3 target;
public:
  static Target* getInstance();
  ~Target();

  void Initialize();
  void Deinitialize();
  void Process(const float deltaTime, const float percent);
  bool IsTypeOf(const std::type_info& inf);

  virtual void Apply(IRenderingView* rv);
  
  Vec3 getTarget();
  void setTarget(float x, float y, float z);
  void setTarget( Vec3 v );
  void printTarget();
};

#endif
