#ifndef _HEIGHT_MAP_
#define _HEIGHT_MAP_

//inherits from
#include <Renderers/IRenderNode.h>

//using shared pointers
#include <Resources/ITextureResource.h>

//templated classes
#include <Math/Vector.h>

// using typedefs
#include <string>

namespace OpenEngine {
  namespace Renderers {
    class IRenderingView;
  }
  namespace Scene {
    class GeometryNode;
  }
}

using OpenEngine::Math::Vector;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Resources::ITextureResourcePtr;
using OpenEngine::Scene::GeometryNode;
using std::string;

class HeightMap : public IRenderNode {
private:
    float scale;
    Vector<3,float> translate;
    float HEIGHT_RATIO; /* Ratio That The Y Is Scaled According To The X And Z */
    GeometryNode* geometry;
    ITextureResourcePtr scaledHeightMap;
    Vector<3,float>* normalArray; /* Holds The Normal Map Data */

    inline Vector<3,float> Point(int X, int Z);
    inline Vector<3,float> Normal(int X, int Z);
    inline Vector<3,float> Color(int X, int Z);
    GeometryNode* ConstructGeometry(ITextureResourcePtr texture);
    void CalculateNormalArray();
    inline float Height(int X, int Z);

public:
    HeightMap(ITextureResourcePtr heightMap, ITextureResourcePtr texture,
	      float scale, float heightRatio, int stepSize);
    virtual ~HeightMap();

    virtual void Apply(IRenderingView* rv);
    
    float HeightAt(float x, float z);
    Vector<3,float> HeightAt(Vector<3,float> p);
    Vector<3,float> NormalAt(Vector<3,float> p);
};

#endif
