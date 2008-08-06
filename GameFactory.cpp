// Game Factory for an example project.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "Common/Vec3.h"

// Class header
#include "GameFactory.h"

#include "Modules/InputGrabber/InputGrabber.h"
#include "Modules/Island/Island.h"
#include "Modules/OscSurface/OscSurface.h"
#include "Modules/Intro/Intro.h"
#include "Modules/Target/Target.h"
#include "Modules/Boid/BoidsSystem.h"
#include "Modules/Particle/ParticleSystem.h"
#include "Modules/Dragon/Dragon.h"
#include "Modules/InputGrabber/InputGrabber.h"

// OpenEngine library
#include <Display/Viewport.h>
#include <Display/ViewingVolume.h>
#include <Display/SDLFrame.h>
#include <Devices/SDLInput.h>
#include <Renderers/IRenderNode.h>
#include <Renderers/OpenGL/Renderer.h>
#include <Renderers/OpenGL/RenderingView.h>
#include <Resources/ResourceManager.h>
#include <Resources/DirectoryManager.h>
#include <Resources/TGAResource.h>
#include <Resources/OBJResource.h>
#include <Scene/SceneNode.h>
#include <Scene/PointLightNode.h>
#include <Scene/TransformationNode.h>
#include <Utils/Statistics.h>
#include <Meta/OpenGL.h>
#include <Utils/MoveHandler.h>
#include <Utils/QuitHandler.h>
#include "KeyHandler.h"

#include <Scene/VertexArrayTransformer.cpp>
#include <Scene/DisplayListTransformer.h>

#include <Renderers/AcceleratedRenderingView.h>
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

    // Main viewport
    Viewport* viewport = new Viewport(*frame);

    // Bind the camera to the viewport
    camera = new Camera( *(new ViewingVolume()) );
    camera->SetPosition(Vector<3,float>(0,40,100));
    camera->LookAt(Vector<3,float>(0,0,0));
    viewport->SetViewingVolume(camera);
    camera->SetFar(2500);
    camera->SetNear(0.1);

    // Create a renderer.
    renderer = new Renderer();
    
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


class GLSettings : public IRenderNode {
public:
  void Apply(IRenderingView* rv) {
    /*
        GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
        GLfloat lm_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    */
    /*
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    */

    //glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );

    //OpenGL settings
    /*
    glShadeModel(GL_SMOOTH); // Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Black Background
    glClearDepth(1.0f); // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
*/
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations

    /*
    glShadeModel( GL_SMOOTH );

    glEnable( GL_DEPTH_TEST );
    glEnable( GL_NORMALIZE );
    glDisable(GL_COLOR);
*/
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_AUTO_NORMAL);

    //glDisable(GL_BLEND);

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
    ISceneNode* scene = new GLSettings();
    // Supply the scene to the renderer
    this->renderer->SetSceneRoot(scene);

    // Set scene lighting
    PointLightNode* light = new PointLightNode();
    TransformationNode* lightPosition = new TransformationNode();
    lightPosition->SetPosition(Vector<3,float>(0,0,100));
    lightPosition->AddNode(light);
    scene->AddNode(lightPosition);

    // Bind the quit handler (the keyboard module needs to work for
    // the handler to actually quit).
    QuitHandler* quit_h = new QuitHandler();
    quit_h->BindToEventSystem();

    KeyHandler* key_h = new KeyHandler();
    key_h->BindToEventSystem();

    // Register movement handler to be able to move the camera
    MoveHandler* move = new MoveHandler(*camera);
    move->BindToEventSystem();
    engine.AddModule(*move);

    // set the resources directory
    string resources = "projects/DragonPanic/data/";
    DirectoryManager::AppendPath(resources);
    logger.info << "Resource directory: " << resources << logger.end;

    ResourceManager<IModelResource>::AddPlugin(new OBJPlugin());
    ResourceManager<ITextureResource>::AddPlugin(new TGAPlugin());


    InputGrabber* inputgrabber = InputGrabber::getInstance();
    scene->AddNode(inputgrabber);
    engine.AddModule(*inputgrabber);
    //scene = inputgrabber;


    // GLLight module missing
    Intro* intro = Intro::getInstance();
    scene->AddNode(intro);
    engine.AddModule(*intro);

    Island* island = Island::getInstance();
    scene->AddNode(island);

    Target* target = Target::getInstance();
    scene->AddNode(target);
    engine.AddModule(*target);

    Dragon* dragon = Dragon::getInstance();
    scene->AddNode(dragon);
    engine.AddModule(*dragon);

    // Boids have transparent shadows and must be second last
    BoidsSystem* boids = BoidsSystem::getInstance();
    scene->AddNode(boids);
    engine.AddModule(*boids);


    TransparencyNode* tpNode = new TransparencyNode();
    scene->AddNode(tpNode);

    OscSurface* oscs = OscSurface::getInstance();
    tpNode->AddNode(oscs);
    engine.AddModule(*oscs);

    // Particles are transparent and must be last
    ParticleSystem* pat = ParticleSystem::getInstance();
    tpNode->AddNode(pat);
    engine.AddModule(*pat);

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
