#ifndef _TARGET_H_
#define _TARGET_H_

// using templates
#include <Math/Vector.h>

// inherits from
#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Renderers/IRenderNode.h>

class HeightMap;
using OpenEngine::Core::IListener;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;

class Target : public IListener<ProcessEventArg>, public IRenderNode {
private:
  bool active;
  Vector<3,float> target;
  HeightMap* heightMap;
public:
  Target(HeightMap* heightMap);
  ~Target();

  void SetActive(bool active);
  void Handle(ProcessEventArg arg);
  virtual void Apply(IRenderingView* rv);
  
  Vector<3,float> getTarget();
  void setTarget(float x, float y, float z);
  void setTarget( Vector<3,float> v );
  void printTarget();
};

#endif
