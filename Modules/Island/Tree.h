#ifndef _TREE_H_
#define _TREE_H_

//inherits from
#include <Renderers/IRenderNode.h>

//templated classes
#include <Math/Vector.h>

namespace OpenEngine {
  namespace Renderers {
    class IRenderingView;
  }
}

using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Renderers::IRenderNode;

class Tree : public IRenderNode {
private:
    Vector<3,float> position;
public:
    Tree(Vector<3,float> position);
    virtual ~Tree();

    virtual void Apply(IRenderingView* rv);
};

#endif
