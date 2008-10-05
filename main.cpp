// Main setup for the OpenEngine DragonPanic project.
// -------------------------------------------------------------------
// Copyright (C) 2008 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

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
#include <Display/SDLFrame.h>
#include <Display/HUD.h>
#include <Devices/SDLInput.h>

// Rendering structures
#include <Renderers/IRenderNode.h>
// OpenGL rendering implementation
#include <Renderers/OpenGL/LightRenderer.h>
#include <Renderers/OpenGL/BufferedRenderer.h>
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
#include <Resources/ITextureResource.h>
#include <Resources/TGAResource.h>
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
#include <Utils/LayerStatistics.h> 
#include <Utils/EventProfiler.h>

// OERacer utility files
#include <Utils/QuitHandler.h>

// sound 
#include <Sound/OpenALSoundSystem.h>
#include <Sound/MusicPlayer.h>
#include <Resources/VorbisResource.h>

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
#include "Modules/Particle/ParticleSystem.h"
#include "Modules/Boid/BoidsSystem.h"

#include "HUD/DragonHUD.h"

// Additional namespaces
using namespace OpenEngine::Core;
using namespace OpenEngine::Logging;
using namespace OpenEngine::Devices;
using namespace OpenEngine::Display;
using namespace OpenEngine::Renderers::OpenGL;
using namespace OpenEngine::Renderers;
using namespace OpenEngine::Resources;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Sound;

// Configuration structure to pass around to the setup methods
struct Config {
    IEngine&              engine;
    EventProfiler         prof;
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
    ISoundSystem*         soundsystem;
    MusicPlayer*          musicplayer;
    TimeModifier*         timeModifier;
    Target*               target;
    BoidsSystem*          boids;
    ParticleSystem*       partsys;
    OscSurface*           oscs;
    Dragon*               dragon;
    bool                  resourcesLoaded;
    HUD*                  hud;
    TextureLoader*        textureLoader;
    Config(IEngine& engine)
        : engine(engine)
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
        , soundsystem(NULL)
        , musicplayer(NULL)
        , timeModifier(NULL)
        , target(NULL)
        , boids(NULL)
        , partsys(NULL)
        , oscs(NULL)
        , dragon(NULL)
        , resourcesLoaded(false)
        , hud(NULL)
        , textureLoader(NULL)
    {}
};

// Forward declaration of the setup methods
void SetupResources(Config&);
void SetupDevices(Config&);
void SetupDisplay(Config&);
void SetupRendering(Config&);
void SetupScene(Config&);
void SetupDebugging(Config&);
void SetupSound(Config&);

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
    logger.info << "KeyboardLayout.txt in the project reposetory" << logger.end;
    logger.info << logger.end;

    // Create an engine and config object
    Engine* engine = new Engine();
    Config config(*engine);

    // Setup the engine
    SetupResources(config);
    SetupDisplay(config);
    SetupDevices(config);
    SetupSound(config);
    SetupRendering(config);
    SetupScene(config);

    // Possibly add some debugging stuff
    SetupDebugging(config);

    // Start up the engine.
    engine->Start();

    // Print out any profiling info
    config.prof.DumpInfo();

    // Return when the engine stops.
    delete engine;

    return EXIT_SUCCESS;
}

void SetupSound(Config& config) {
    config.soundsystem = new OpenALSoundSystem(config.scene,config.camera);
    config.engine.ProcessEvent().Attach(*config.soundsystem);
    config.soundsystem->SetMasterGain(1.0);
    config.musicplayer = new MusicPlayer(config.camera,config.soundsystem);
    config.musicplayer->AddSound("Music/beak.ogg");
    config.musicplayer->AddSound("Music/defibrilation.ogg");
    config.musicplayer->AddSound("Music/glow.ogg");
    config.musicplayer->AddSound("Music/trouble.ogg");
    config.musicplayer->SetGain(0.3);
    config.musicplayer->Shuffle(true);
    config.musicplayer->Next();
    config.musicplayer->Play();
    config.engine.ProcessEvent().Attach(*config.musicplayer);
}

void SetupResources(Config& config) {
    // set the resources directory
    // @todo we should check that this path exists
    // set the resources directory
    string resources = "projects/DragonPanic/data/";
    DirectoryManager::AppendPath(resources);

    // load resource plug-ins
    ResourceManager<IModelResource>::AddPlugin(new OBJPlugin());
    ResourceManager<ITextureResource>::AddPlugin(new TGAPlugin());
    ResourceManager<ISoundResource>::AddPlugin(new VorbisResourcePlugin());

    config.resourcesLoaded = true;
}

void SetupDisplay(Config& config) {
    if (config.frame         != NULL ||
        config.viewingvolume != NULL ||
        config.camera        != NULL ||
        config.frustum       != NULL ||
        config.viewport      != NULL)
        throw Exception("Setup display dependencies are not satisfied.");

    //config.frame         = new SDLFrame(1440, 900, 32, FRAME_FULLSCREEN);
    config.frame         = new SDLFrame(800, 600, 32);
    config.viewingvolume = new InterpolatedViewingVolume(*(new ViewingVolume()));
    config.camera        = new FollowCamera( *config.viewingvolume );
    //config.frustum       = new Frustum(*config.camera, 20, 3000);
    config.viewport      = new Viewport(*config.frame);
    config.viewport->SetViewingVolume(config.camera);

    config.engine.InitializeEvent().Attach(*config.frame);
    config.engine.ProcessEvent().Attach(*config.frame);
    config.engine.DeinitializeEvent().Attach(*config.frame);
}

void SetupDevices(Config& config) {
    if (config.keyboard != NULL ||
        config.mouse    != NULL)
        throw Exception("Setup devices dependencies are not satisfied.");
    // Create the mouse and keyboard input modules
    SDLInput* input = new SDLInput();
    config.keyboard = input;
    config.mouse = input;
    config.joystick = input;

    // Bind the quit handler
    QuitHandler* quit_h = new QuitHandler(config.engine);
    config.keyboard->KeyEvent().Attach(*quit_h);

    // Bind to the engine for processing time
    config.engine.InitializeEvent().Attach(*input);
    config.engine.ProcessEvent().Attach(*input);
    config.engine.DeinitializeEvent().Attach(*input);
}

void SetupRendering(Config& config) {
    if (config.viewport == NULL ||
        config.renderer != NULL ||
        config.camera == NULL ||
        config.soundsystem == NULL )
        throw Exception("Setup renderer dependencies are not satisfied.");

    // Create a renderer
    config.renderer = new Renderer();
    //config.renderer = new BufferedRenderer();

    // Setup a rendering view
    RenderingView* rv = new RenderingView(*config.viewport);
    config.renderer->ProcessEvent().Attach(*rv);

    // Add rendering initialization tasks
    config.textureLoader = new TextureLoader(*config.renderer);

    DisplayListTransformer* dlt = new DisplayListTransformer(rv);
    config.renderer->InitializeEvent().Attach(*dlt);

    config.renderer->PreProcessEvent()
      .Attach( *(new LightRenderer()) );

    config.engine.InitializeEvent().Attach(*config.renderer);
    config.engine.ProcessEvent().Attach(*config.renderer);
    config.engine.DeinitializeEvent().Attach(*config.renderer);

    config.hud = new HUD();
    config.renderer->PostProcessEvent().Attach( *config.hud );
}

void SetupScene(Config& config) {
    if (config.scene  != NULL ||
        config.mouse  == NULL ||
        config.keyboard == NULL ||
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
    config.soundsystem->SetRoot(config.scene);
    
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

    //init HeightMap
    string filename = DirectoryManager::FindFileInPath("Island/Terrain5.raw");
    ITextureResourcePtr hMap = 
      ITextureResourcePtr(new RAWResource(filename, 1024, 1024, 8));
    hMap->Load();
        
    ITextureResourcePtr texture =
      ResourceManager<ITextureResource>::Create("Island/ground.tga");

    HeightMap* heightMap = new HeightMap(hMap, texture, 300.0, 0.25, 16);

    Island* island = new Island(heightMap);
    config.scene->AddNode(island);
    hMap->Unload();

    Target* target = config.target = new Target(*heightMap);
    TransformationNode* targetNode = &target->GetTargetNode();
    config.scene->AddNode(targetNode);
    config.engine.ProcessEvent().Attach(*target);

    TransparencyNode* tpNode = new TransparencyNode();
    config.scene->AddNode(tpNode);

    Vector<4,float> oscsColor(0.8f,0.25f,0.0f,0.7f); // lava
    //Vector<4,float> oscsColor(0.1f,0.25f,0.7f,0.7f); // water
    OscSurface* oscs = config.oscs = new OscSurface(heightMap,oscsColor);
    //tpNode->AddNode(oscs); // Moved down!
    config.engine.InitializeEvent().Attach(*oscs);
    timeModifier->ProcessEvent().Attach(*oscs);
    config.engine.DeinitializeEvent().Attach(*oscs);

    //@todo: Boids have transparent shadows
    BoidsSystem* boids = config.boids = new BoidsSystem(heightMap, oscs,*config.soundsystem);
    //tpNode->AddNode(boids); // moved down!
    config.engine.InitializeEvent().Attach(*boids);
    timeModifier->ProcessEvent().Attach(*boids);

    ParticleSystem* pat = config.partsys = new ParticleSystem(heightMap,config.camera,boids, *config.textureLoader);

    tpNode->AddNode(pat);
    tpNode->AddNode(boids);
    tpNode->AddNode(oscs);

    config.engine.InitializeEvent().Attach(*pat);
    timeModifier->ProcessEvent().Attach(*pat);

    pat->ParticleSystemEvent().Attach(*boids);
    boids->SetParticleSystem(pat);

    Dragon* dragon = config.dragon = new Dragon(heightMap,target,pat,*config.textureLoader);
    config.scene->AddNode(dragon);
    config.engine.InitializeEvent().Attach(*dragon);
    config.engine.ProcessEvent().Attach(*dragon);

    
    // game state logic
    config.gamestate = new GameState(120);
    boids->BoidSystemEvent().Attach(*config.gamestate);

    KeyHandler* key_h = new KeyHandler(*config.camera, *targetNode, *heightMap,
                                       *config.mouse, island, dragon, boids, *timeModifier, *config.gamestate, *config.musicplayer, *config.frame);
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
    DragonHUD* hud = new DragonHUD(*config.frame, *config.gamestate,
                                   *config.hud, *config.textureLoader);
    //config.scene->AddNode(hud->GetLayerNode());
    config.engine.ProcessEvent().Attach(*hud);
}

void SetupDebugging(Config& config) {
    // main engine events
    config.prof.Profile<ProcessEventArg>
        ("Input System", config.engine.ProcessEvent(), *config.mouse);
    config.prof.Profile<ProcessEventArg>
        ("Sound System", config.engine.ProcessEvent(), *config.soundsystem);
    config.prof.Profile<ProcessEventArg>
        ("Music Player", config.engine.ProcessEvent(), *config.musicplayer);
    config.prof.Profile<ProcessEventArg>
        ("Target",       config.engine.ProcessEvent(), *config.target);
    config.prof.Profile<ProcessEventArg>
        ("Renderer",     config.engine.ProcessEvent(), *config.renderer);
    config.prof.Profile<ProcessEventArg>
        ("Dragon",       config.engine.ProcessEvent(), *config.dragon);
    // time modified events
    config.prof.Profile<ProcessEventArg>
        ("Particle System", config.timeModifier->ProcessEvent(), *config.partsys);
    config.prof.Profile<ProcessEventArg>
        ("Boids System",    config.timeModifier->ProcessEvent(), *config.boids);
    config.prof.Profile<ProcessEventArg>
        ("Osc Surface",     config.timeModifier->ProcessEvent(), *config.oscs);

    // Visualization of the frustum
    if (config.frustum != NULL) {
        config.frustum->VisualizeClipping(true);
        config.scene->AddNode(config.frustum->GetFrustumNode());
    }

    // FPS layer with cairo
    //@todo: added a fps via CairoResource and CairoTextTool
}

