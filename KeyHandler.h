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
#include <Core/IListener.h>
#include <Core/EngineEvents.h>

#include <Display/FollowCamera.h>
/*
#include <Devices/IMouse.h>
#include <Scene/TransformationNode.h>
*/
#include <Devices/IKeyboard.h>

#include <list>

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

using namespace OpenEngine::Core;
using namespace OpenEngine::Display;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Devices;
using std::list;

class KeyHandler : public IListener<ProcessEventArg>,
  public IListener<KeyboardEventArg> {
private:
    Intro* intro;
    Island* island;
    Dragon* dragon;
    BoidsSystem* boidssystem;
    FollowCamera& camera;
    TransformationNode& target;

    list<Key> keysPressed;

    float moveChunkMouse, rotChunkMouse, moveChunkKeyboard, rotChunkKeyboard;
    int mousex_prev, mousey_prev, mousex_orig, mousey_orig;
    bool warping;
    
    void reset();
public:
    KeyHandler(FollowCamera& camera,
               TransformationNode& target,
               Island* island,
               Dragon* dragon,
               BoidsSystem* boidssystem);
    ~KeyHandler();

    void Handle(KeyboardEventArg arg);
    void HandleUp(Key key);
    void HandleDown(Key key);

    void Handle(ProcessEventArg arg);

};

#endif // _KEY_HANDLER_H_
