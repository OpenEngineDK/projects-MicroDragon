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

#include <Core/IModule.h>
#include <Display/Camera.h>
#include <Devices/IKeyboard.h>
#include <Devices/IMouse.h>
#include <Scene/TransformationNode.h>
#include <vector>

using namespace OpenEngine::Core;
using namespace OpenEngine::Display;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Devices;

class KeyHandler : public IListener<KeyboardEventArg> {
private:
    float moveChunkMouse, rotChunkMouse, moveChunkKeyboard, rotChunkKeyboard;
    int mousex_prev, mousey_prev, mousex_orig, mousey_orig;
    bool warping;
    
    void reset();
public:
    KeyHandler();
    ~KeyHandler();
    void Handle(KeyboardEventArg arg);
    void HandleUp(KeyboardEventArg arg);
    void HandleDown(KeyboardEventArg arg);
    void BindToEventSystem();
};

#endif // _KEY_HANDLER_H_
