#ifndef _ISLAND_H_
#define _ISLAND_H_

//inherits from
#include <Scene/RenderNode.h>

//templated classes
#include <Math/Vector.h>

//forward reference
class HeightMap;
namespace OpenEngine {
    namespace Geometry {
        class Face;
    }
    namespace Renderers {
        class IRenderingView;
    }
    namespace Scene {
      class TransformationNode;
    }
}

using OpenEngine::Math::Vector;
using OpenEngine::Scene::RenderNode;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Scene::TransformationNode;
using namespace std;

class Island : public RenderNode {
private:
    HeightMap* heightMap;
    TransformationNode* trees;

    bool enabled; //original inherited from MTree - Module
    bool bRender; /* Polygon Flag Set To TRUE By Default */

    int numberOfTrees;
    bool renderTrees;
    bool enableTexture;
    int renderState, numberOfRenderStates;

public:
    Island(HeightMap* heightMap);
    ~Island();

    virtual void Apply(IRenderingView* rv);

    void toggleRenderState();
};

#endif
