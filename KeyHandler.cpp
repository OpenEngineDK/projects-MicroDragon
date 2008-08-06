// Key handler.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "KeyHandler.h"

#include "Modules/Boid/BoidsSystem.h"
#include "Modules/Dragon/Dragon.h"
#include "Modules/Island/Island.h"
#include "Modules/Intro/Intro.h"
#include "Modules/OscSurface/OscSurface.h"
#include "Modules/Target/Target.h"
#include "Modules/Particle/ParticleSystem.h"
#include "Modules/Intro/Intro.h"
#include "Modules/InputGrabber/InputGrabber.h"

#include <Logging/Logger.h>
#include <Core/IGameEngine.h>
#include <Devices/IMouse.h>
#include <Math/Vector.h>

using OpenEngine::Core::IGameEngine;
using OpenEngine::Math::Vector;
using OpenEngine::Scene::TransformationNode;

KeyHandler::KeyHandler() {
  reset();
}

void KeyHandler::reset() {
  //InputGrabber::reset();
    //mousex_prev = mousey_prev = 0;
    rotChunkMouse = 0.05;
    moveChunkMouse = 0.0015;
    rotChunkKeyboard = rotChunkMouse*200;
    moveChunkKeyboard = moveChunkMouse*400;
    warping = false;
}

KeyHandler::~KeyHandler() {}

// set state of keys on up/down events
void KeyHandler::Handle(KeyboardEventArg arg) {
    if(arg.type == KeyboardEventArg::PRESS)
      HandleDown(arg);
    else
      HandleUp(arg);
}

void KeyHandler::HandleDown(KeyboardEventArg arg) {
    switch (arg.sym) {
    case KEY_1:
        BoidsSystem::getInstance()->toggleRenderState();
        break;
    case KEY_2:
        Dragon::getInstance()->toggleRenderState();
        break;
    case KEY_3:
        Island::getInstance()->toggleRenderState();
        break;
    case KEY_4:
      //OscSurface::getInstance()->enableDisable();
        break;
    case KEY_5:
      //Target::getInstance()->enableDisable();
        break;
    case KEY_6:
      //ParticleSystem::getInstance()->enableDisable();
        break;
    case KEY_8:
        BoidsSystem::getInstance()->DecNumberOfShownBoids();
        break;
    case KEY_9:
        BoidsSystem::getInstance()->IncNumberOfShownBoids();
        break;
    case KEY_a:
      InputGrabber::getInstance()->moveTarget( -1*moveChunkKeyboard, 0 );
        break;
    case KEY_d:
      InputGrabber::getInstance()->moveTarget( 1*moveChunkKeyboard, 0 );
        break;
    case KEY_w:
      InputGrabber::getInstance()->moveTarget( 0, -1*moveChunkKeyboard );
        break;
    case KEY_s:
      InputGrabber::getInstance()->moveTarget( 0, 1*moveChunkKeyboard );
        break;
    case KEY_z:
      InputGrabber::getInstance()->rotZ = InputGrabber::getInstance()->rotZ - rotChunkKeyboard;
        break;
    case KEY_x:
      InputGrabber::getInstance()->rotZ = InputGrabber::getInstance()->rotZ + rotChunkKeyboard;
        break;
    case KEY_p:
      InputGrabber::getInstance()->togglePause();
        break;
    case KEY_f:
      InputGrabber::getInstance()->printLocation();
        break;
    case KEY_SPACE:
        if( !Intro::getInstance()->isDone() )
            Intro::getInstance()->disable();
        break;
    case KEY_r:
      reset();
        break;
    case KEY_b:
        BoidsSystem::getInstance()->Initialize();
        break;
    case KEY_t:
        BoidsSystem::getInstance()->DecAlignment();
        break;
    case KEY_y:
        BoidsSystem::getInstance()->IncAlignment();
        break;
    case KEY_e:
        Dragon::getInstance()->useBreathWeapon( true );
        break;
    case KEY_q:
        Dragon::getInstance()->chargeFireball( true );
        break;
    default:
        break;
    }
}

void KeyHandler::HandleUp(KeyboardEventArg arg) {
    switch (arg.sym) {
    case KEY_e:
        Dragon::getInstance()->useBreathWeapon( false );
        break;
    case KEY_q:
        Dragon::getInstance()->chargeFireball( false );
        break;
    default:
        break;
    }
}

void KeyHandler::BindToEventSystem() {
    IKeyboard::keyEvent.Attach(*this);
}
