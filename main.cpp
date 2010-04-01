// Main setup for the OpenEngine DragonPanic project.
// -------------------------------------------------------------------
// Copyright (C) 2008 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "Config.h"

// OpenEngine stuff
#include <Meta/Config.h>

// Core structures
#include <Core/Engine.h>

// Display structures
#include <Display/IFrame.h>
#include <Display/FollowCamera.h>
#include <Display/Frustum.h>
#include <Display/InterpolatedViewingVolume.h>
#include <Display/ViewingVolume.h>
// SDL implementation
#include <Display/SDLEnvironment.h>
#ifdef DRAGON_HUD
#include <Display/HUD.h>
#endif
// Rendering structures
#include <Scene/RenderNode.h>
// OpenGL rendering implementation
#include <Renderers/OpenGL/LightRenderer.h>
#include <Renderers/OpenGL/FBOBufferedRenderer.h>
#include <Renderers/OpenGL/Renderer.h>
#include <Renderers/OpenGL/RenderingView.h>
#include <Renderers/TextureLoader.h>

// Resources
#include <Resources/IModelResource.h>
#include <Resources/File.h>
#include <Resources/DirectoryManager.h>
#include <Resources/ResourceManager.h>
// OBJ and TGA plugins
#include <Resources/RAWResource.h>
#include <Resources/ITexture2D.h>
#include <Resources/SDLImage.h>
#include <Resources/OBJResource.h>

// Scene structures
#include <Scene/ISceneNode.h>
#include <Scene/SceneNode.h>
#include <Scene/GeometryNode.h>
#include <Scene/TransformationNode.h>
#include <Scene/VertexArrayTransformer.h>
#include <Scene/DisplayListTransformer.h>
#include <Scene/PointLightNode.h>

// Utilities and logger
#include <Logging/Logger.h>
#include <Logging/StreamLogger.h>
#include <Utils/EventProfiler.h>
#include <Utils/FPSSurface.h>
#include <Scene/DotVisitor.h>

// OERacer utility files
#include <Utils/QuitHandler.h>

// sound
#ifdef DRAGON_SOUND
#include <Sound/OpenALSoundSystem.h>
#include <Sound/MusicPlayer.h>
#include <Resources/VorbisResource.h>
#endif
#include <Meta/GLUT.h>

// NEW OEPARTICLESYSTEM TEST
#include <ParticleSystem/ParticleSystem.h>
#include <Effects/FireEffect.h>

// from project
#include "LightFader.h"
#include "KeyHandler.h"
#include "TransparentcyNode.h"
#include "GLSettingsNode.h"
#include "GameState.h"
#include "TimeModifier.h"

#include "Modules/Island/Island.h"
#include "Modules/Island/HeightMap.h"
#include "Modules/Target/Target.h"
#include "Modules/OscSurface/OscSurface.h"
#include "Modules/Dragon/Dragon.h"
#include "Modules/Boid/BoidsSystem.h"

#ifdef DRAGON_HUD
#include "HUD/DragonHUD.h"
#endif

// Additional namespaces
using namespace OpenEngine::Core;
using namespace OpenEngine::Logging;
using namespace OpenEngine::Devices;
using namespace OpenEngine::Display;
using namespace OpenEngine::Renderers;
using namespace OpenEngine::Resources;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Sound;

// Configuration structure to pass around to the setup methods
struct Config {
    IEngine&              engine;
    EventProfiler         prof;
    IEnvironment*         env;
    IFrame*               frame;
    Viewport*             viewport;
    IViewingVolume*       viewingvolume;
    FollowCamera*         camera;
    Frustum*              frustum;
    IRenderer*            renderer;
    IMouse*               mouse;
    IKeyboard*            keyboard;
    IJoystick*            joystick;
    ISceneNode*           scene;
    GameState*            gamestate;
#ifdef DRAGON_SOUND    
    ISoundSystem*         soundsystem;
    MusicPlayer*          musicplayer;
#endif
    TimeModifier*         timeModifier;
    Target*               target;
    BoidsSystem*          boids;
    OscSurface*           oscs;
    Dragon*               dragon;
    bool                  resourcesLoaded;
#ifdef DRAGON_HUD
    HUD*                  hud;
#endif
    OpenEngine::Renderers::TextureLoader* textureLoader;
    OpenEngine::ParticleSystem::ParticleSystem* particlesystem;
    OpenEngine::ParticleSystem::ParticleSystemTimer* pstimer;

    Config(IEngine& engine)
        : engine(engine)
        , env(NULL)
        , frame(NULL)
        , viewport(NULL)
        , viewingvolume(NULL)
        , camera(NULL)
        , frustum(NULL)
        , renderer(NULL)
        , mouse(NULL)
        , keyboard(NULL)
        , joystick(NULL)
        , scene(NULL)
        , gamestate(NULL)
#ifdef DRAGON_SOUND          
        , soundsystem(NULL)
        , musicplayer(NULL)
#endif
        , timeModifier(NULL)
        , target(NULL)
        , boids(NULL)
        , oscs(NULL)
        , dragon(NULL)
        , resourcesLoaded(false)
#ifdef DRAGON_HUD
        , hud(NULL)
#endif
        , textureLoader(NULL)
        , particlesystem(NULL)
        , pstimer(NULL)
    {}
};

// Forward declaration of the setup methods
void SetupResources(Config&);
void SetupDevices(Config&);
void SetupDisplay(Config&);
void SetupRendering(Config&);
void SetupParticleSystem(Config&);
#ifdef DRAGON_SOUND
void SetupSound(Config&);
#endif
void SetupScene(Config&);
void SetupDebugging(Config&);

int main(int argc, char** argv) {
    // Setup logging facilities.
    Logger::AddLogger(new StreamLogger(&std::cout));

    // Print usage info.
    logger.info << "========= ";
    logger.info << "Running The OpenEngine DragonPanic Project";
    logger.info << " =========" << logger.end;

    logger.info << "This project is a simple testing project for OpenEngine.";
    logger.info << logger.end;

    logger.info << "  for control information see: ";
    logger.info << "KeyboardLayout.txt in the project repository" << logger.end;
    logger.info << logger.end;

    // Create an engine and config object
    Engine* engine = new Engine();
    Config config(*engine);

    // Setup the engine
    SetupResources(config);
    SetupDisplay(config);
    SetupDevices(config);
#ifdef DRAGON_SOUND
    SetupSound(config);
#endif
    SetupParticleSystem(config);
    SetupRendering(config);
    SetupScene(config);
    
    // Possibly add some debugging stuff
    SetupDebugging(config);

    // Start up the engine.
    engine->Start();

// #if OE_DEBUG
    // Print out any profiling info
    config.prof.DumpInfo();
// #endif

    // release event system
    // post condition: scene and modules are not processed
    delete engine;

    // destroy all modules
    // post condition: no one has refs to the scene
    //delete config.boids; //delete sounds
#ifdef DRAGON_SOUND    
    delete config.soundsystem; // close soundsystem
#endif
    delete config.scene;

    // Return when the engine stops.
    return EXIT_SUCCESS;
}


void SetupParticleSystem(Config& config) {
    config.particlesystem = new ParticleSystem::ParticleSystem();
    
    // Add to engine for processing time (with its timer)
    //config.pstimer = 
    //new ParticleSystem::ParticleSystemTimer(*config.particlesystem);
    config.engine.InitializeEvent().Attach(*config.particlesystem);
    //config.engine.ProcessEvent().Attach(*config.pstimer);
    config.engine.DeinitializeEvent().Attach(*config.particlesystem);

}
#ifdef DRAGON_SOUND
void SetupSound(Config& config) {
    config.soundsystem = new OpenALSoundSystem();
    config.musicplayer = new MusicPlayer(NULL,config.soundsystem);
    bool enableSound = true;
    if (enableSound) {
        // setup the sound system
        config.soundsystem->SetMasterGain(1.0);
        config.engine.InitializeEvent().Attach(*config.soundsystem);
        config.engine.DeinitializeEvent().Attach(*config.soundsystem);

        // setup the music player
        config.musicplayer->AddSound("Music/beak.ogg");
//         config.musicplayer->AddSound("Music/defibrilation.ogg");
//         config.musicplayer->AddSound("Music/glow.ogg");
//         config.musicplayer->AddSound("Music/trouble.ogg");
        config.musicplayer->SetGain(0.3);
        config.musicplayer->Shuffle(true);
        config.musicplayer->Next();
        config.musicplayer->Play();
        config.engine.ProcessEvent().Attach(*config.musicplayer);
    }
}
#endif

void SetupResources(Config& config) {
    // set the resources directory
    // @todo we should check that this path exists
    // set the resources directory
    string resources = "projects/DragonPanic/data/";
    DirectoryManager::AppendPath(resources);

    // load resource plug-ins
    ResourceManager<IModelResource>::AddPlugin(new OBJPlugin());
    ResourceManager<ITexture2D>::AddPlugin(new SDLImagePlugin());
#ifdef DRAGON_SOUND
    ResourceManager<ISoundResource>::AddPlugin(new VorbisResourcePlugin());
#endif
    config.resourcesLoaded = true;
}

void SetupDisplay(Config& config) {
    if (config.frame         != NULL ||
        config.viewingvolume != NULL ||
        config.camera        != NULL ||
        config.frustum       != NULL ||
        config.viewport      != NULL)
        throw Exception("Setup display dependencies are not satisfied.");

    //config.env = new SDLEnvironment(1280, 960, 32, FRAME_FULLSCREEN);
    config.env = new SDLEnvironment(800, 600);
    //config.frame         = new SDLFrame(1024, 768, 32, FRAME_FULLSCREEN);    
    config.frame         = &config.env->GetFrame();
    config.viewingvolume = new InterpolatedViewingVolume(*(new ViewingVolume()));
    config.camera        = new FollowCamera( *config.viewingvolume );
    //config.frustum       = new Frustum(*config.camera, 20, 3000);
    config.viewport      = new Viewport(*config.frame);
    config.viewport->SetViewingVolume(config.camera);

    // config.engine.InitializeEvent().Attach(*config.frame);
    // config.engine.ProcessEvent().Attach(*config.frame);
    // config.engine.DeinitializeEvent().Attach(*config.frame);
}

void SetupDevices(Config& config) {
    if (config.keyboard != NULL ||
        config.mouse    != NULL ||
        config.env      == NULL)
        throw Exception("Setup devices dependencies are not satisfied.");
    // Create the mouse and keyboard input modules
    
    config.keyboard = config.env->GetKeyboard();
    config.mouse = config.env->GetMouse();
    config.joystick = config.env->GetJoystick();

    // Bind the quit handler
    QuitHandler* quit_h = new QuitHandler(config.engine);
    config.keyboard->KeyEvent().Attach(*quit_h);

    // Bind to the engine for processing time
    config.engine.InitializeEvent().Attach(*config.env);
    config.engine.ProcessEvent().Attach(*config.env);
    config.engine.DeinitializeEvent().Attach(*config.env);
}

void SetupRendering(Config& config) {
    if (config.viewport == NULL ||
        config.renderer != NULL ||
        config.camera == NULL 
#ifdef DRAGON_SOUND    
        ||
        config.soundsystem == NULL 
#endif
        )
        throw Exception("Setup renderer dependencies are not satisfied.");

    // Create a renderer
    config.renderer = new OpenGL::Renderer(config.viewport);
    //config.renderer = new FBOBufferedRenderer(config.viewport);

    // Setup a rendering view
    IRenderingView* rv = new OpenGL::RenderingView(*config.viewport);
    config.renderer->ProcessEvent().Attach(*rv);

    // Add rendering initialization tasks
    config.textureLoader = new Renderers::TextureLoader(*config.renderer);
    config.renderer->PreProcessEvent().Attach(*config.textureLoader);

    DisplayListTransformer* dlt = new DisplayListTransformer(rv);
    config.renderer->InitializeEvent().Attach(*dlt);

    config.renderer->PreProcessEvent()
        .Attach( *(new Renderers::OpenGL::LightRenderer(*config.viewport)) );

    config.engine.InitializeEvent().Attach(*config.renderer);
    config.engine.ProcessEvent().Attach(*config.renderer);
    config.engine.DeinitializeEvent().Attach(*config.renderer);

#ifdef DRAGON_HUD

    config.hud = new HUD();
    config.renderer->PostProcessEvent().Attach( *config.hud );
    config.renderer->PreProcessEvent().Attach(*config.soundsystem);
#endif
}

void SetupScene(Config& config) {
    if (config.scene  != NULL ||
        config.mouse  == NULL ||
        config.keyboard == NULL ||
        config.particlesystem == NULL 
#ifdef DRAGON_SOUND    
        ||
        config.soundsystem == NULL 
#endif
        ||
        config.resourcesLoaded == false)
        throw Exception("Setup scene dependencies are not satisfied.");

    // Create a root scene node

    RenderStateNode* renderStateNode = new RenderStateNode();
    renderStateNode->EnableOption(RenderStateNode::LIGHTING);
    config.scene = renderStateNode;

    float fadetime = 3000.0 * 3.5;
    GLSettingsNode* glNode = new GLSettingsNode(fadetime);
    config.engine.ProcessEvent().Attach(*glNode);
    renderStateNode->AddNode(glNode);

    // attach scene to soundsystem
    // config.soundsystem->SetRoot(config.scene);
    
    // Set scene lighting
    float pFade = 1.4;

    PointLightNode* light1 = new PointLightNode();
    light1->ambient = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    light1->diffuse = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    light1->specular = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    TransformationNode* light1Position = new TransformationNode();
    light1Position->SetPosition(Vector<3,float>(0.8, 0.7, 0.4));
    light1Position->AddNode(light1);
    config.scene->AddNode(light1Position);
    PointLightNode* to1 = new PointLightNode();
    to1->ambient = Vector<4,float>(0.1*pFade, 0.1*pFade, 0.1*pFade, 1.0);
    to1->diffuse = Vector<4,float>(0.4*pFade, 0.4*pFade, 0.7*pFade, 1.0);
    to1->specular = Vector<4,float>(0.0, 0.0, 0.0, 1.0);

    LightFader* light1Fader = new LightFader(light1, to1, light1, fadetime);
    config.engine.ProcessEvent().Attach(*light1Fader);

    PointLightNode* light2 = new PointLightNode();
    light2->ambient = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    light2->diffuse = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    light2->specular = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    TransformationNode* light2Position = new TransformationNode();
    light2Position->SetPosition(Vector<3,float>(-1.0, 1.0, 0.7));
    light2Position->AddNode(light2);
    config.scene->AddNode(light2Position);
    PointLightNode* to2 = new PointLightNode();
    to2->ambient = Vector<4,float>(0.1*pFade, 0.1*pFade, 0.1*pFade, 1.0);
    to2->diffuse = Vector<4,float>(0.8*pFade, 0.8*pFade, 0.7*pFade, 1.0);
    to2->specular = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    LightFader* light2Fader = new LightFader(light2, to2, light2, fadetime);
    config.engine.ProcessEvent().Attach(*light2Fader);

    TimeModifier* timeModifier = 
        new TimeModifier(config.engine.ProcessEvent(),1.00f);
    config.timeModifier = timeModifier;
    //timeModifier->SetFactor(1.23);

    timeModifier->ProcessEvent().Attach(*config.particlesystem);

    //init HeightMap
    string filename = DirectoryManager::FindFileInPath("Island/Terrain5.raw");
    UCharTexture2DPtr hMap = 
        UCharTexture2DPtr(new RAWResource(filename, 1024, 1024, 1));
    hMap->Load();
        
    ITexture2DPtr texture =
      ResourceManager<ITexture2D>::Create("Island/ground.tga");

    HeightMap* heightMap = new HeightMap(hMap, texture, 300.0, 0.25, 2);

    Island* island = new Island(heightMap);
    config.scene->AddNode(island);

    Target* target = config.target = new Target(*heightMap);
    TransformationNode* targetNode = target->GetTargetNode();
    config.scene->AddNode(targetNode);
    config.engine.ProcessEvent().Attach(*target);

    TransparencyNode* tpNode = new TransparencyNode();
    config.scene->AddNode(tpNode);

    Vector<4,float> oscsColor(0.8f,0.25f,0.0f,0.7f); // lava
    //Vector<4,float> oscsColor(0.1f,0.25f,0.7f,0.7f); // water
    OscSurface* oscs = config.oscs = new OscSurface(heightMap,oscsColor);
    timeModifier->ProcessEvent().Attach(*oscs);
    tpNode->AddNode(oscs);

    //@todo: Boids have transparent shadows
    BoidsSystem* boids = config.boids = new BoidsSystem(heightMap, oscs,
#ifdef DRAGON_SOUND                                                            
                                                        *config.soundsystem,
#endif
                                                        *config.particlesystem,
                                                        *config.textureLoader,
                                                        config.scene);
    config.engine.InitializeEvent().Attach(*boids);
    timeModifier->ProcessEvent().Attach(*boids);
    tpNode->AddNode(boids);


    Dragon* dragon = config.dragon = new Dragon(heightMap, *config.boids, target,
                                                *config.textureLoader,
                                                *config.particlesystem, config.scene);
    config.scene->AddNode(dragon);
    config.engine.InitializeEvent().Attach(*dragon);
    config.engine.ProcessEvent().Attach(*dragon);

    
    // game state logic
    config.gamestate = new GameState(120);
    boids->BoidSystemEvent().Attach(*config.gamestate);

    KeyHandler* key_h = new KeyHandler(*config.camera,
                                       *targetNode,
                                       *heightMap,
                                       *config.mouse,
                                       island, 
                                       dragon, 
                                       boids,
                                       *timeModifier,
                                       *config.gamestate,
#ifdef DRAGON_SOUND    
                                       *config.musicplayer,
#endif
                                       *config.frame,
                                       renderStateNode);
    //    KeyHandler* key_h = new KeyHandler(*config.camera, *targetNode, *heightMap, island, dragon, boids, *timeModifier, *config.gamestate);

    config.engine.ProcessEvent().Attach(*key_h);
    config.keyboard->KeyEvent().Attach(*key_h);

    config.joystick->JoystickButtonEvent().Attach(*key_h);
    config.joystick->JoystickAxisEvent().Attach(*key_h);

    config.textureLoader->Load(*config.scene);

    // Transform the scene to use vertex arrays
    VertexArrayTransformer vaT;
    vaT.Transform(*config.scene);

    // Supply the scene to the renderer
    config.renderer->SetSceneRoot(config.scene);

    //HUD
#ifdef DRAGON_HUD

    config.textureLoader->SetDefaultReloadPolicy(Renderers::TextureLoader::RELOAD_QUEUED);
    DragonHUD* hud = new DragonHUD(*config.frame, *config.gamestate,
                                   *config.hud, *config.textureLoader);
    config.textureLoader->SetDefaultReloadPolicy(Renderers::TextureLoader::RELOAD_NEVER);
    //config.scene->AddNode(hud->GetLayerNode());
    config.engine.ProcessEvent().Attach(*hud);
#endif
}

void SetupDebugging(Config& config) {
// #if OE_DEBUG
    // main engine events
    config.prof.Profile<ProcessEventArg>
        ("Environment", config.engine.ProcessEvent(), *config.env);
#ifdef DRAGON_SOUND
    config.prof.Profile<ProcessEventArg>
        ("Sound System", config.engine.ProcessEvent(), *config.soundsystem);
    config.prof.Profile<ProcessEventArg>
        ("Music Player", config.engine.ProcessEvent(), *config.musicplayer);
#endif
    config.prof.Profile<ProcessEventArg>
        ("Target",       config.engine.ProcessEvent(), *config.target);
    config.prof.Profile<ProcessEventArg>
        ("Renderer",     config.engine.ProcessEvent(), *config.renderer);
    config.prof.Profile<ProcessEventArg>
        ("Dragon",       config.engine.ProcessEvent(), *config.dragon);
    // renderer events
#ifdef DRAGON_HUD
    config.prof.Profile<RenderingEventArg>
        ("HUD", config.renderer->ProcessEvent(), *config.hud);
#endif
    // time modified events
    config.prof.Profile<ProcessEventArg>
        ("Boids System",    config.timeModifier->ProcessEvent(), *config.boids);
    config.prof.Profile<ProcessEventArg>
        ("Osc Surface",     config.timeModifier->ProcessEvent(), *config.oscs);
    config.prof.Profile<ProcessEventArg>
        ("OE Particle System", config.timeModifier->ProcessEvent(), *config.particlesystem);

    // Visualization of the frustum
    if (config.frustum != NULL) {
        config.frustum->VisualizeClipping(true);
        config.scene->AddNode(config.frustum->GetFrustumNode());
    }

    ofstream dotfile("scene.dot", ofstream::out);
    if (!dotfile.good()) {
        logger.error << "Can not open 'scene.dot' for output"
                     << logger.end;
    } else {
        DotVisitor dot;
        dot.Write(*config.scene, &dotfile);
        logger.info << "Saved scene graph to 'scene.dot'"
                    << logger.end
                    << "To create a SVG image run: "
                    << "dot -Tsvg scene.dot > scene.svg"
                    << logger.end;
    }
// #endif
#ifdef DRAGON_HUD
    // FPS layer with cairo
    FPSSurfacePtr fps = FPSSurface::Create();
    config.textureLoader->Load(fps, TextureLoader::RELOAD_QUEUED);
    config.engine.ProcessEvent().Attach(*fps);
    HUD::Surface* fpshud = config.hud->CreateSurface(fps);
    fpshud->SetPosition(HUD::Surface::LEFT, HUD::Surface::TOP); 
#endif    
}

