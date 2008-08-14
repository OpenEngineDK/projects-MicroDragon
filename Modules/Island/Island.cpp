// class header
#include "Island.h"

// module classes
#include "HeightMap.h"
#include "Tree.h"

#include "../../Common/utilities.h"

#include <Math/Math.h>
#include <iostream>

#include <Logging/Logger.h>
#include <Renderers/IRenderingView.h>
#include <Resources/DirectoryManager.h>
#include <Resources/ITextureResource.h>
#include <Resources/ResourceManager.h>
#include <Resources/RAWResource.h>
#include <Scene/TransformationNode.h>

using OpenEngine::Resources::RAWResource;
using OpenEngine::Resources::DirectoryManager;
using OpenEngine::Resources::ITextureResource;
using OpenEngine::Resources::ITextureResourcePtr;
using OpenEngine::Resources::ResourceManager;

Island::Island() {
    bRender = enabled = renderTrees = enableTexture = true;
    numberOfRenderStates = 5;
    renderState = numberOfRenderStates-1;

    //init HeightMap
    string filename = DirectoryManager::FindFileInPath("Island/Terrain5.raw");
    ITextureResourcePtr hMap = 
      ITextureResourcePtr(new RAWResource(filename, 1024, 1024, 8));
    hMap->Load();
        
    ITextureResourcePtr texture =
      ResourceManager<ITextureResource>::Create("Island/ground.tga");

    heightMap = new HeightMap(hMap, texture, 300.0, 0.25, 16);
    hMap->Unload();
    this->AddNode(heightMap);

    trees = new TransformationNode();
    this->AddNode(trees);

    //initialize trees position
    numberOfTrees = 0;
    for (int xx=-150; xx<150; xx+=10)
        for (int zz=-150; zz<  0; zz+=10) {
            Vector<3,float> pos(xx+fmod(sin((double)(xx+1000*zz))*100000,10),
				0,
				zz+fmod(cos((double)(xx+1000*zz))*100000,10));
            pos = heightMap->HeightAt(pos);

	    // only place tree in lanscape where slope is less than maxSlope
	    float maxSlope = 0.8;
            if (heightMap->
		NormalAt(Vector<3,float>(xx,0,zz))[1]>maxSlope &&
                
		heightMap->
		NormalAt(Vector<3,float>(xx+10,0,zz))[1]>maxSlope &&

                heightMap->
		NormalAt(Vector<3,float>(xx,0,zz+10))[1]>maxSlope &&
                
		heightMap->
		NormalAt(Vector<3,float>(xx+10,0,zz+10))[1]>maxSlope &&

                pos[1]>2 && (xx*xx+zz*zz)>50*50) {
	      trees->AddNode(new Tree(pos));
	      numberOfTrees++;
            }
        }
    logger.info << "number of trees: " << numberOfTrees << logger.end;
}

Island::~Island() {
    delete heightMap;
    // @todo: delete trees
}

void Island::Apply(IRenderingView* rv) {
    // Draw trees
    if (renderTrees)
        trees->Accept(*rv);
    VisitSubNodes(*rv);
}

void Island::toggleRenderState(){
    enabled = true;
    switch ( renderState ){
    case 0: //no trees
        renderTrees = false;
        break;
    case 1: //disable texture
        enableTexture = false;
        break;
    case 2: //grid layout
        cout << "setting GL_LINE render" << endl;
        bRender = false;
        break;
    case 3: //all off
        enabled = false;
        break;
    default: //render all, reset variables
        bRender = true;
        enabled = true;
        renderTrees = true;
        enableTexture = true;
        break;
    }
    renderState++;
    renderState %= numberOfRenderStates;
}

Vector<3,float> Island::normalAt(Vector<3,float> p) {
  return heightMap->NormalAt(p);
}

float Island::heightAt(float x, float z) {
  return heightMap->HeightAt(x,z);
}

Vector<3,float> Island::heightAt(Vector<3,float> p) {
  return heightMap->HeightAt(p);
}

