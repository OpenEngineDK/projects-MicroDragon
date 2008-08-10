// class header
#include "Island.h"

// module classes
#include "HeightMap.h"

#include "../../Common/OpenGLUtil.h"
#include "../../Common/utilities.h"

#include <Meta/OpenGL.h>
#include <Math/Math.h>
#include <iostream>
#include <Renderers/IRenderingView.h>
#include <Resources/DirectoryManager.h>
#include <Resources/ITextureResource.h>
#include <Resources/ResourceManager.h>
#include <Resources/RAWResource.h>

using OpenEngine::Resources::RAWResource;
using OpenEngine::Resources::DirectoryManager;
using OpenEngine::Resources::ITextureResource;
using OpenEngine::Resources::ITextureResourcePtr;
using OpenEngine::Resources::ResourceManager;

Island::Island() {
    bRender = true;
    enabled = true;
    renderTrees = true;
    enableTexture = true;
    numberOfRenderStates = 5;
    renderState = numberOfRenderStates-1;

    //init HeightMap
    string filename = DirectoryManager::FindFileInPath("Island/Terrain5.raw");
    ITextureResourcePtr hMap = 
      ITextureResourcePtr(new RAWResource(filename, 1024, 1024, 8));
    hMap->Load();
        
    ITextureResourcePtr texture =
      ResourceManager<ITextureResource>::Create("Island/ground.tga");

    heightMap = new HeightMap(hMap, texture, 300.0, 0.25);
    hMap->Unload();

    this->AddNode(heightMap->GetGeometryNode());

    //initialize trees position
    numberOfTrees = 0;
    treePositions = new Vector<3,float>*[15*15];
    for (int xx=-150; xx<150; xx+=10)
        for (int zz=-150; zz<  0; zz+=10) {
            Vector<3,float> pos = Vector<3,float>(xx+fmod(sin((double)(xx+1000*zz))*100000,10),0,zz+fmod(cos((double)(xx+1000*zz))*100000,10));
            pos = heightMap->HeightAt(pos);
            if (
                heightMap->NormalAt(Vector<3,float>(xx   ,0,zz   ))[1]>0.8 &&
                heightMap->NormalAt(Vector<3,float>(xx+10,0,zz   ))[1]>0.8 &&
                heightMap->NormalAt(Vector<3,float>(xx   ,0,zz+10))[1]>0.8 &&
                heightMap->NormalAt(Vector<3,float>(xx+10,0,zz+10))[1]>0.8 &&
                pos[1]>2 &&
                (xx*xx+zz*zz)>50*50
                ) {
                treePositions[numberOfTrees++] = new Vector<3,float>( pos[0], pos[1], pos[2] );
            }
        }
}

Island::~Island() {
    delete heightMap;

    while(numberOfTrees > 0)
        delete treePositions[--numberOfTrees];
    delete treePositions;
}

void Island::Apply(IRenderingView* rv) {
    // Draw trees
    if (renderTrees)
        for(int i=0; i<numberOfTrees; i++)
            drawTrees( *treePositions[i] );

    VisitSubNodes(*rv);
}

void Island::drawTrees( Vector<3,float> pos ) {
    glColor3f( 0.25, 0.35, 0.10 );
    glPushMatrix();
    glTranslatef( pos[0], pos[1]+2, pos[2] );
    glScalef(1,1.5,1);
    OpenGLUtil::GLSolidSphere( 1.0, 8, 4 );
    glPopMatrix();

    glColor3f( 0.33, 0.22, 0.11 );
    glPushMatrix();
    glTranslatef( pos[0], pos[1]+0.4, pos[2] );
    glScalef(1,1.5,1);
    OpenGLUtil::GLSolidCube( 0.6 );
    glPopMatrix();
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

