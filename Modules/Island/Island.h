#ifndef _ISLAND_H_
#define _ISLAND_H_

#include "../../Common/Vec3.h"

#include <Renderers/IRenderNode.h>
#include <Renderers/IRenderingView.h>
#include <Resources/ITextureResource.h>
#include <Geometry/Face.h>

using namespace OpenEngine::Geometry;
using OpenEngine::Renderers::IRenderNode;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Resources::ITextureResourcePtr;
using namespace std;

class Island : public IRenderNode {
private:
  bool enabled; //original inherited from MTree - Module

    static Island* instance;
    Island();
    bool bRender; /* Polygon Flag Set To TRUE By Default */
    void RenderLines();
    const static int IMAGE_SIZE = 1024; /* Size Of Our .RAW Height Map */
    const static int STEP_SIZE = 16; /* Width And Height Of Each Quad */
    const static int MAP_SIZE = IMAGE_SIZE/STEP_SIZE;
    const static int DISPLAY_ID = 1;
    const static int WIREFRAME_DISPLAY_ID = 100;
    int texID;

    float HEIGHT_RATIO; /* Ratio That The Y Is Scaled According To The X And Z */
    Vec3 translate;
    float scale;

    unsigned char heightArray[(IMAGE_SIZE/STEP_SIZE)*(IMAGE_SIZE/STEP_SIZE)];
    Vec3 normalArray[(IMAGE_SIZE/STEP_SIZE)*(IMAGE_SIZE/STEP_SIZE)]; /* Holds The Normal Map Data */
    //float scaleValue; /* Scale Value For The Terrain */
    void drawTrees( Vec3 pos );
    Vec3** treePositions;
    int numberOfTrees;
    bool renderTrees;
    bool enableTexture;
    int renderState, numberOfRenderStates;

public:
    static Island* getInstance();
    ~Island();
    void toggleRenderState();

    virtual void Apply(IRenderingView* rv);

    GeometryNode* RenderHeightMap(ITextureResourcePtr texture);

    void LoadRawFile(const char* strName, int nSize,unsigned char *pHeightMap);
    void calcHeightArray(unsigned char *pHeightMap);
    void calcNormalArray();

    float Height(int X, int Z);
    float heightAt(float x, float z);
    Vec3 Point(int X, int Z);
    Vec3 heightAt(Vec3 p);
    Vec3 normalAt(Vec3 p);
    Vec3 Normal(int X, int Z);
    Vec3 Color(int X, int Z);
};

#endif
