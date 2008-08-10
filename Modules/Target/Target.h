#ifndef _TARGET_H_
#define _TARGET_H_

// using templates
#include <Math/Vector.h>

// inherits from
#include <Core/IModule.h>
#include <Renderers/IRenderNode.h>

class Island;

using OpenEngine::Core::IModule;
using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;

class Target : public IModule, public IRenderNode {
private:
  bool active;
  Vector<3,float> target;
  Island* island;
public:
  Target(Island* island);
  ~Target();

  void SetActive(bool active);

  void Initialize();
  void Deinitialize();
  void Process(const float deltaTime, const float percent);
  bool IsTypeOf(const std::type_info& inf);

  virtual void Apply(IRenderingView* rv);
  
  Vector<3,float> getTarget();
  void setTarget(float x, float y, float z);
  void setTarget( Vector<3,float> v );
  void printTarget();
};

#endif
