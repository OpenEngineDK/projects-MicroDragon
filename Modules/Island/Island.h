#ifndef _ISLAND_H_
#define _ISLAND_H_

//inherits from
#include <Renderers/IRenderNode.h>

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
}

using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;
using namespace std;

class Island : public IRenderNode {
private:
    HeightMap* heightMap;
    Vector<3,float>** treePositions;

    bool enabled; //original inherited from MTree - Module
    bool bRender; /* Polygon Flag Set To TRUE By Default */

    void drawTrees( Vector<3,float> pos );
    int numberOfTrees;
    bool renderTrees;
    bool enableTexture;
    int renderState, numberOfRenderStates;

public:
    Island();
    ~Island();

    virtual void Apply(IRenderingView* rv);

    void toggleRenderState();

    //temp
    Vector<3,float> normalAt(Vector<3,float> p);
    float heightAt(float x, float z);
    Vector<3,float> heightAt(Vector<3,float> p);
};

#endif
