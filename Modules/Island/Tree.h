#ifndef _TREE_H_
#define _TREE_H_

//inherits from
#include <Scene/RenderNode.h>

//templated classes
#include <Math/Vector.h>

namespace OpenEngine {
  namespace Renderers {
    class IRenderingView;
  }
}

using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Scene::RenderNode;

class Tree : public RenderNode {
private:
    Vector<3,float> position;
public:
    Tree(Vector<3,float> position);
    virtual ~Tree();

    virtual void Apply(IRenderingView* rv);
};

#endif
