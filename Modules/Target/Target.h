#ifndef _TARGET_H_
#define _TARGET_H_

// using templates
#include <Math/Vector.h>

// inherits from
#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Renderers/IRenderNode.h>

#include <Scene/TransformationNode.h>

class HeightMap;
using OpenEngine::Core::IListener;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;

using OpenEngine::Scene::TransformationNode;

class Target : public IListener<ProcessEventArg> {
private:

    class TargetRenderNode : public IRenderNode {
        Target* target;
    public:
        TargetRenderNode();
        virtual ~TargetRenderNode() {}
        void SetTarget(Target* target);
        virtual void Apply(IRenderingView* rv);
    };

    bool active;
    Vector<3,float> target;
    HeightMap& heightMap;
    TargetRenderNode* rnode;
    TransformationNode* tnode;

public:
    Target(HeightMap& heightMap);
    virtual ~Target();

    void SetActive(bool active);
    void Handle(ProcessEventArg arg);
    
    TransformationNode* GetTargetNode();
  
    Vector<3,float> getTarget();
    void setTarget(float x, float y, float z);
    void setTarget( Vector<3,float> v );
    void printTarget();
};

#endif
