// Game Factory for an example project.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

// header define to avoid including definitions several times
#ifndef _EXAMPLE_PROJECT_GAME_FACTORY_H_
#define _EXAMPLE_PROJECT_GAME_FACTORY_H_

namespace OpenEngine {
  namespace Display {
    class Camera;
    class IFrame;
  }
  namespace Scene {
    class ISceneNode;
  }
  namespace Renderers {
    class IRenderer;
  }
}

// Namespaces
using namespace OpenEngine::Core;
using namespace OpenEngine::Display;
using namespace OpenEngine::Renderers;

class GameFactory {

private:
    IFrame*      frame;         // member variable for the engine frame
    IRenderer*   renderer;      // member variable for the engine renderer
    Camera*      camera;

public:
    GameFactory();
    bool         SetupEngine(Engine& engine);
    IFrame*      GetFrame();
    IRenderer*   GetRenderer();

};

#endif // _EXAMPLE_PROJECT_GAME_FACTORY_H_
