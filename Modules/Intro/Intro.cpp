// class header
#include "Intro.h"

// from project
#include "../InputGrabber/InputGrabber.h"

// from OpenEngine Core
#include <Math/Math.h>
#include <Math/Quaternion.h>
#include <Geometry/Face.h>
#include <Geometry/FaceSet.h>
#include <Renderers/IRenderer.h>
#include <Renderers/IRenderingView.h>
#include <Resources/ResourceManager.h>
#include <Resources/IModelResource.h>
#include <Scene/TransformationNode.h>
#include <Utils/Timer.h>

// from Extension OpenGLRenderer
#include <Meta/OpenGL.h>
#include <Renderers/OpenGL/Renderer.h>
#include <Renderers/OpenGL/RenderingView.h>
#include <Renderers/OpenGL/TextureLoader.h>

// from std
#include <iostream>

// from OpenEngine Core
using OpenEngine::Resources::ResourceManager;
using OpenEngine::Renderers::IRenderer;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Math::PI;
using OpenEngine::Math::Quaternion;
using OpenEngine::Utils::Timer;

// from extension OpenGLRenderer
using OpenEngine::Renderers::OpenGL::Renderer;
using OpenEngine::Renderers::OpenGL::RenderingView;
using OpenEngine::Renderers::OpenGL::TextureLoader;

using namespace OpenEngine::Geometry;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Resources;
using namespace std;

Intro::Intro(InputGrabber* inputgrabber) {
  this->inputgrabber =inputgrabber;
    blend = 0.0;
    pos = Vector<3,float>(0.0,0.0,-100.0);
    fadeoutTime = 6.0;
    timepassed = 0.0;
    fadeDone = false;
    curDisplayId = RUNE_DISPLAY_ID;
    startDelay = 5.0; //sec

    rune = LoadIntoDisplaylist("Intro/text-rune.obj", RUNE_DISPLAY_ID);
    cpvc = LoadIntoDisplaylist("Intro/text-cpvc.obj", CPVC_DISPLAY_ID);
    tic = LoadIntoDisplaylist("Intro/text-tic.obj", TIC_DISPLAY_ID);
    cea = LoadIntoDisplaylist("Intro/text-cea.obj", CEA_DISPLAY_ID);
}

Intro::~Intro(){
}


void Intro::Initialize() {
    //@todo remove this, global texture loader should work now

    // load texture
    TextureLoader* tl = new TextureLoader();
    this->Accept(*tl);
}

TransformationNode* Intro::LoadIntoDisplaylist(string filename, int id) {
    IModelResourcePtr resource = ResourceManager<IModelResource>::
      Create(filename);
    if( resource != NULL ) {
        // load geometry
        resource->Load();
        GeometryNode* gNode = (GeometryNode*)resource->GetSceneNode();

	TransformationNode* tpos = new TransformationNode();
	TransformationNode* trotate = new TransformationNode();

        float scale = 5.0f; // cpvc 40.0f;
	tpos->Scale(scale,scale,scale);

	trotate->AddNode(gNode);
	tpos->AddNode(trotate);
	this->AddNode(tpos);
	return trotate;
    } else
        throw Exception("error loading resource: " + filename);
}

void Intro::Deinitialize() {
}

void Intro::Process(const float deltaTime, const float percent) {
    double timeStep = deltaTime/1000.0;
    if( startDelay > 0.0 ) {
        startDelay -= timeStep;
        return;
    }
    timepassed += timeStep;
}

bool Intro::IsTypeOf(const std::type_info& inf) {
    return (typeid(Intro) == inf);
}

void Intro::Apply(IRenderingView* rv) {
  if( !fadeDone ) {
      float percentDone = (timepassed / fadeoutTime);
      
      if( percentDone > 1.0 ) { 
            curDisplayId++;
            if( curDisplayId > CEA_DISPLAY_ID ){
                fadeDone = true;
                // fly camera
		//cpvc inputgrabber->rotateViewAbsolute(360.0,45.0,26.608,1.0);
            }
            timepassed = 0.0f; 
        }
        
        // @todo: fixed color fading
        // Color that goes from lava orange to dragon green
        //cpvc float l = pow(percentDone,0.5f);
        //cpvc glColor4f(0.80f-0.70f*l, 0.25f+0.35f*l, 0.0f+0.1f*l, 1.0-pow(percentDone*1.1,2));

        Vector<3,float> flybyDir;
        if (curDisplayId==RUNE_DISPLAY_ID+0) flybyDir = Vector<3,float>(-100, 6,0);
        if (curDisplayId==RUNE_DISPLAY_ID+1) flybyDir = Vector<3,float>(   0,12,0);
        if (curDisplayId==RUNE_DISPLAY_ID+2) flybyDir = Vector<3,float>(+100, 6,0);
        if (curDisplayId==RUNE_DISPLAY_ID+3) flybyDir = Vector<3,float>(   0, 0,0);
        pos = Vector<3,float>(flybyDir[0]*pow(percentDone,14),
		   -10+(59+flybyDir[1])*sqrt(percentDone),
		   -50+200*pow(percentDone,8));

	TransformationNode* tpos = NULL;
        if (curDisplayId==RUNE_DISPLAY_ID+0) tpos = rune;
        else if (curDisplayId==RUNE_DISPLAY_ID+1) tpos = cpvc;
        else if (curDisplayId==RUNE_DISPLAY_ID+2) tpos = tic;
        else if (curDisplayId==RUNE_DISPLAY_ID+3) tpos = cea;

	if (tpos == NULL) return;
	TransformationNode* trotate = (TransformationNode*)tpos->GetParent();
	tpos->SetPosition(Vector<3,float>(pos[0],pos[1],pos[2]));
	Quaternion<float> q( -sin(percentDone*PI*1.5)*(PI/6),
			    Vector<3,float>(1.0,0.0,0.0) );
	trotate->SetRotation(q);

	//@todo: does the following make a difference?
	//cpvc glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
	trotate->Accept(*rv);
	//cpvc glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    }
}

bool Intro::isDone(){
    return fadeDone;
}

void Intro::disable(){
    fadeDone = true;
    inputgrabber->
      rotateViewAbsolute(360.0,45.0,26.608,1.0);
}
