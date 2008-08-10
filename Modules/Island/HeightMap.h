#ifndef _HEIGHT_MAP_
#define _HEIGHT_MAP_

//using shared pointers
#include <Resources/ITextureResource.h>

//templated classes
#include <Math/Vector.h>

#include <string>

namespace OpenEngine {
    namespace Scene {
        class GeometryNode;
    }
}

using OpenEngine::Math::Vector;
using OpenEngine::Resources::ITextureResourcePtr;
using OpenEngine::Scene::GeometryNode;
using std::string;

class HeightMap {
private:
    GeometryNode* geometry;

    float scale;

    Vector<3,float> translate;

    void RenderLines();

    const static int IMAGE_SIZE = 1024; /* Size Of Our .RAW Height Map */

    const static int STEP_SIZE = 16; /* Width And Height Of Each Quad */
    const static int MAP_SIZE = IMAGE_SIZE/STEP_SIZE;
    /*
    const static int DISPLAY_ID = 1;
    const static int WIREFRAME_DISPLAY_ID = 100;
    int texID;
    */
    float HEIGHT_RATIO; /* Ratio That The Y Is Scaled According To The X And Z */
    unsigned char* heightArray;

    Vector<3,float>* normalArray; /* Holds The Normal Map Data */

    //float scaleValue; /* Scale Value For The Terrain */

    GeometryNode* ConstructGeometry(ITextureResourcePtr texture);

protected:
    Vector<3,float> Point(int X, int Z);
    Vector<3,float> Normal(int X, int Z);
    Vector<3,float> Color(int X, int Z);

    void CalcHeightArray(ITextureResourcePtr hMap);
    void CalcNormalArray();
    float Height(int X, int Z);

public:
    HeightMap(ITextureResourcePtr heightMap, ITextureResourcePtr texture,
	      float scale, float heightRatio);
    virtual ~HeightMap();

    float HeightAt(float x, float z);
    Vector<3,float> HeightAt(Vector<3,float> p);
    Vector<3,float> NormalAt(Vector<3,float> p);

    GeometryNode* GetGeometryNode();
};

#endif
