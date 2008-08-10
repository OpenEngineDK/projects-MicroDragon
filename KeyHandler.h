// Key handler.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _KEY_HANDLER_H_
#define _KEY_HANDLER_H_

// inherits from
#include <Core/IModule.h>

class BoidsSystem;
class Dragon;
class InputGrabber;
class Intro;
class Island;
class Target;
namespace OpenEngine {
    namespace Devices {
        class IKeyboard;
        class IMouse;
    }
    namespace Display {
        class Camera;
    }
    namespace Scene {
        class TransformationNode;
    }
}
/*
#include <Display/Camera.h>
#include <Devices/IMouse.h>
#include <Scene/TransformationNode.h>
*/
#include <Devices/IKeyboard.h>

#include <list>

using namespace OpenEngine::Core;
using namespace OpenEngine::Display;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Devices;
using std::list;

class KeyHandler : public IModule, public IListener<KeyboardEventArg> {
private:
    InputGrabber* inputgrabber;
    Intro* intro;
    Island* island;
    Target* target;
    Dragon* dragon;
    BoidsSystem* boidssystem;

    list<Key> keysPressed;

    float moveChunkMouse, rotChunkMouse, moveChunkKeyboard, rotChunkKeyboard;
    int mousex_prev, mousey_prev, mousex_orig, mousey_orig;
    bool warping;
    
    void reset();
public:
    KeyHandler(InputGrabber* inputgrabber, Intro* intro, Island* island,
	       Target* target, Dragon* dragon, BoidsSystem* boidssystem);
    ~KeyHandler();
    void Handle(KeyboardEventArg arg);
    void HandleUp(Key key);
    void HandleDown(Key key);
    void BindToEventSystem();

    void Initialize();
    void Deinitialize();
    void Process(const float deltaTime, const float percent);
    bool IsTypeOf(const std::type_info& inf);
};

#endif // _KEY_HANDLER_H_
