// Game Factory for an example project.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

// Class header
#include "GameFactory.h"
#include "LightFader.h"

#include "Modules/Island/Island.h"
#include "Modules/Target/Target.h"
#include "Modules/InputGrabber/InputGrabber.h"
#include "Modules/Intro/Intro.h"
#include "Modules/OscSurface/OscSurface.h"
#include "Modules/Dragon/Dragon.h"
#include "Modules/Particle/ParticleSystem.h"
#include "Modules/Boid/BoidsSystem.h"

#include "KeyHandler.h"

// OpenEngine library
#include <Display/Camera.h>
#include <Display/Frustum.h>
#include <Display/SDLFrame.h>
#include <Devices/SDLInput.h>
#include <Display/Viewport.h>
#include <Display/ViewingVolume.h>
#include <Renderers/IRenderNode.h>
#include <Renderers/OpenGL/Renderer.h>
#include <Renderers/OpenGL/RenderingView.h>
#include <Resources/ResourceManager.h>
#include <Resources/DirectoryManager.h>
#include <Resources/TGAResource.h>
#include <Resources/OBJResource.h>
#include <Scene/ISceneNode.h>
#include <Scene/SceneNode.h>
#include <Scene/PointLightNode.h>
#include <Scene/TransformationNode.h>
#include <Utils/Statistics.h>
#include <Meta/OpenGL.h>
#include <Utils/QuitHandler.h>

#include <Scene/VertexArrayTransformer.cpp>
#include <Scene/DisplayListTransformer.h>

#include <Renderers/OpenGL/TextureLoader.h>

// Additional namespaces (others are in the header).
using namespace OpenEngine::Devices;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Renderers::OpenGL;

/**
 * Factory constructor.
 *
 * Initializes the different components so they are accessible when
 * the setup method is executed.
 */
GameFactory::GameFactory() {

    // Create a frame and viewport.
    this->frame = new SDLFrame(800, 600, 32);

    // Setup camera
    camera = new Camera( *(new ViewingVolume()) );
    Frustum* frustum = new Frustum(*camera);
    frustum->SetFar(2500);
    frustum->SetNear(0.1);

    // Main viewport
    Viewport* viewport = new Viewport(*frame);
    viewport->SetViewingVolume(frustum);

    // Add a rendering view to the renderer
    renderer = new Renderer();
    renderer->initialize.Attach(*(new TextureLoader()));

    RenderingView* rv = new RenderingView(*viewport);
    renderer->process.Attach(*rv);

    DisplayListTransformer* dlt = new DisplayListTransformer(rv);
    renderer->initialize.Attach(*dlt);
}

/**
 * Setup handler.
 *
 * This is the main setup method of the game factory. Here you can add
 * any non-standard modules and perform other setup tasks prior to the
 * engine start up.
 *
 * @param engine The game engine instance.
 */


class GLSettings : public IModule, public IRenderNode {
private:
  float timeSpend,time;
  bool done;
public:
  GLSettings(float time) : time(time) {
      timeSpend = 0.0;
      done = false;
  }
  
  void Initialize() {
  }

  void Deinitialize() {
  }

  bool IsTypeOf(const std::type_info& inf) {
    return (typeid(GLSettings) == inf);
  }

  void Process(const float deltaTime, const float percent) {
    if(done) return;

    if (timeSpend >= time) {
      timeSpend = time;
      done = true;
    }
    else
      timeSpend += deltaTime;
  }
  
  void Apply(IRenderingView* rv) {
    if(!done) {
      double pctDone = timeSpend/time;
      float pFade = 1.4 * pctDone;
      glClearColor( 0.39*pFade, 0.45*pFade, 1.0*pFade, 1.0 );

    }

    /*
    glClearDepth(1.0f); // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
    glEnable( GL_DEPTH_TEST );
    */

    glShadeModel(GL_SMOOTH); // Enable Smooth Shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations

    
    glEnable(GL_NORMALIZE);
    //glEnable(GL_AUTO_NORMAL);

    glEnable(GL_COLOR_MATERIAL);
    VisitSubNodes(*rv);
  }
};

class TransparencyNode : public IRenderNode {
public:
  void Apply(IRenderingView* rv) {
      glEnable(GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDepthMask(GL_FALSE);
      glDisable(GL_LIGHTING);

      VisitSubNodes(*rv);

      glEnable(GL_LIGHTING);
      glDisable(GL_BLEND);
      glDepthMask(GL_TRUE);
  }
};


bool GameFactory::SetupEngine(IGameEngine& engine) {

    // Add your mouse and keyboard module here
    SDLInput* input = new SDLInput();
    engine.AddModule(*input);

    // Create a root scene node
    float fadetime = 3000.0;
    GLSettings* scene = new GLSettings(fadetime);
    engine.AddModule(*scene);

    // Supply the scene to the renderer
    this->renderer->SetSceneRoot(scene);

    // Set scene lighting
    float pFade = 1.4;

    PointLightNode* light1 = new PointLightNode();
    light1->ambient = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    light1->diffuse = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    light1->specular = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    TransformationNode* light1Position = new TransformationNode();
    light1Position->SetPosition(Vector<3,float>(0.8, 0.7, 0.4));
    light1Position->AddNode(light1);
    scene->AddNode(light1Position);
    PointLightNode* to1 = new PointLightNode();
    to1->ambient = Vector<4,float>(0.1*pFade, 0.1*pFade, 0.1*pFade, 1.0);
    to1->diffuse = Vector<4,float>(0.4*pFade, 0.4*pFade, 0.7*pFade, 1.0);
    to1->specular = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    LightFader* light1Fader = new LightFader(*light1, *to1, light1, fadetime);
    engine.AddModule(*light1Fader);

    PointLightNode* light2 = new PointLightNode();
    light2->ambient = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    light2->diffuse = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    light2->specular = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    TransformationNode* light2Position = new TransformationNode();
    light2Position->SetPosition(Vector<3,float>(-1.0, 1.0, 0.7));
    light2Position->AddNode(light2);
    scene->AddNode(light2Position);
    PointLightNode* to2 = new PointLightNode();
    to2->ambient = Vector<4,float>(0.1*pFade, 0.1*pFade, 0.1*pFade, 1.0);
    to2->diffuse = Vector<4,float>(0.8*pFade, 0.8*pFade, 0.7*pFade, 1.0);
    to2->specular = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    LightFader* light2Fader = new LightFader(*light2, *to2, light2, fadetime);
    engine.AddModule(*light2Fader);
    
    // Bind the quit handler (the keyboard module needs to work for
    // the handler to actually quit).
    QuitHandler* quit_h = new QuitHandler();
    quit_h->BindToEventSystem();


    // set the resources directory
    string resources = "projects/DragonPanic/data/";
    DirectoryManager::AppendPath(resources);

    ResourceManager<IModelResource>::AddPlugin(new OBJPlugin());
    ResourceManager<ITextureResource>::AddPlugin(new TGAPlugin());

    Island* island = new Island();
    scene->AddNode(island);

    Target* target = new Target(island);
    scene->AddNode(target);
    engine.AddModule(*target);

    TransparencyNode* tpNode = new TransparencyNode();
    scene->AddNode(tpNode);

    OscSurface* oscs = new OscSurface(island);
    tpNode->AddNode(oscs);
    engine.AddModule(*oscs);

    InputGrabber* inputgrabber = new InputGrabber(camera,island,target);
    engine.AddModule(*inputgrabber);

    Intro* intro = new Intro(inputgrabber);
    scene->AddNode(intro);
    engine.AddModule(*intro);

    //@todo: Boids have transparent shadows
    BoidsSystem* boids = new BoidsSystem(island, oscs);
    tpNode->AddNode(boids);
    engine.AddModule(*boids);

    ParticleSystem* pat = new ParticleSystem(island,camera,boids);
    tpNode->AddNode(pat);
    engine.AddModule(*pat);

    // %todo replace island with target
    Dragon* dragon = new Dragon(island,target,pat);
    scene->AddNode(dragon);
    engine.AddModule(*dragon);

    KeyHandler* key_h = new KeyHandler(inputgrabber, intro, island, target,
				       dragon, boids);
    key_h->BindToEventSystem();
    engine.AddModule(*key_h);

    engine.AddModule(*(new OpenEngine::Utils::Statistics(1000)));

    // Replace all GeometryNodes with VertexArrayNodes
    VertexArrayTransformer vat;
    vat.Transform(*scene);

    // Return true to signal success.
    return true;
}

// Other factory methods. The returned objects are all created in the
// factory constructor.
IFrame*      GameFactory::GetFrame()    { return this->frame;    }
IRenderer*   GameFactory::GetRenderer() { return this->renderer; }
