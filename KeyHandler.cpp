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
#include "Modules/InputGrabber/InputGrabber.h"
//#include "Modules/Particle/ParticleSystem.h"

#include <Logging/Logger.h>
#include <Devices/IMouse.h>
#include <Math/Vector.h>

using OpenEngine::Math::Vector;
using OpenEngine::Scene::TransformationNode;

KeyHandler::KeyHandler(FollowCamera& camera,
                       TransformationNode& target,
                       Island* island,
                       Dragon* dragon,
                       BoidsSystem* boidssystem)
    : camera(camera)
    , target(target) {

  this->island = island;
  this->dragon = dragon;
  this->boidssystem = boidssystem;

  camera.Follow(&target);
  camera.Move(-100,0,50);
  camera.LookAt(0,target.GetPosition()[1],0);

  reset();
}

void KeyHandler::reset() {
    //inputgrabber->reset();
    //mousex_prev = mousey_prev = 0;
    rotChunkMouse = 0.05;
    moveChunkMouse = 0.0015;
    rotChunkKeyboard = 0.2; //rotChunkMouse*200;
    moveChunkKeyboard = 2; //moveChunkMouse*400;
    warping = false;
}

KeyHandler::~KeyHandler() {}

// set state of keys on up/down events
void KeyHandler::Handle(KeyboardEventArg arg) {
    if(arg.type == KeyboardEventArg::PRESS)
      keysPressed.push_back(arg.sym);
    else {
      keysPressed.remove(arg.sym);
      HandleUp(arg.sym);
    }
}

void KeyHandler::Handle(ProcessEventArg arg) {
    list<Key>::iterator key;
    for(key=keysPressed.begin(); key != keysPressed.end(); ++key) {
      HandleDown(*key);
    }
    //keysPressed.clear(); //to disable repeatition when holding down a key
}

void KeyHandler::HandleDown(Key key) {

    switch (key) {
    case KEY_1:
      boidssystem->toggleRenderState();
        break;
    case KEY_2:
        dragon->toggleRenderState();
        break;
    case KEY_3:
        island->toggleRenderState();
        break;
    case KEY_4:
      //OscSurface::getInstance()->enableDisable();
        break;
    case KEY_5:
      //target->enableDisable();
        break;
    case KEY_6:
      //ParticleSystem::getInstance()->enableDisable();
        break;
    case KEY_8:
      boidssystem->DecNumberOfShownBoids();
        break;
    case KEY_9:
        boidssystem->IncNumberOfShownBoids();
        break;
    case KEY_a:
        target.Move(-1*moveChunkKeyboard, 0 , 0);
        break;
    case KEY_d:
        target.Move(1*moveChunkKeyboard, 0 , 0);
        break;
    case KEY_w:
        target.Move(0, 0, -1*moveChunkKeyboard);
        break;
    case KEY_s:
        target.Move(0, 0, 1*moveChunkKeyboard);
        break;
	/*
    case KEY_z:
      inputgrabber->rotZ = inputgrabber->rotZ - rotChunkKeyboard;
        break;
    case KEY_x:
      inputgrabber->rotZ = inputgrabber->rotZ + rotChunkKeyboard;
        break;
	*/
    case KEY_p:
        //inputgrabber->togglePause();
        break;
    case KEY_f:
        //inputgrabber->printLocation();
        break;
    case KEY_SPACE:
        if( !intro->isDone() )
            intro->disable();
        break;
    case KEY_r:
      reset();
        break;
    case KEY_b:
        boidssystem->ResetBoids();
        break;
    case KEY_t:
        boidssystem->DecAlignment();
        break;
    case KEY_y:
        boidssystem->IncAlignment();
        break;
    case KEY_e:
        dragon->useBreathWeapon( true );
        break;
    case KEY_q:
        dragon->chargeFireball( true );
        break;


//     case KEY_F1:
//         //inputgrabber->rotateViewAbsolute( 30, 5, 50, 1.0 );
//         target->setTarget(0, 0, 0);
//         break;
//     case KEY_F2: // ved siden af hovedet
//         //inputgrabber->rotateViewAbsolute( 25.8872, 257.27, 13.8784, 1.0 );
//         target->setTarget( -10.5859, 5.4856, -10.0325 );
//         break;
//     case KEY_F3:
//         //inputgrabber->rotateViewAbsolute( 16.1391, 335, 500, 1.0 );
//         target->setTarget(-10.5859, 5.4856, -10.0325);
//         break;
//     case KEY_F4: // inde i munden
//         //inputgrabber->rotateViewAbsolute( 54.4593, 266.533, 7.75, 1.0 );
//         target->setTarget( -2.77879, 3.63685, 0.168352);
//         break;
//     case KEY_F5: //Mellem bjergene
//         //inputgrabber->rotateViewAbsolute( 340, 245.064, 68.548, 1.0 );
//         target->setTarget( 39.9721, 6.39716, -27.3046 );
//         break;
//     case KEY_F9:
//         //inputgrabber->scaleGlobal( -0.05 );
//         break;
//     case KEY_F10:
//         //inputgrabber->scaleGlobal( 0.05 );
//         break;
//     case KEY_F11:
//       //GLUTClock::getInstance()->decTimeFactor();
//         break;
//     case KEY_F12:
//       //GLUTClock::getInstance()->incTimeFactor();
//         break;
    case KEY_PAGEUP:
        camera.Move(moveChunkKeyboard,0,0);
        break;
    case KEY_PAGEDOWN:
        camera.Move(-moveChunkKeyboard,0,0);
        break;
    case KEY_HOME:
        //inputgrabber->incMultiplier();
        break;
    case KEY_END:
        //inputgrabber->decMultiplier();
        break;
    case KEY_UP:
        camera.Move(0, 0, moveChunkKeyboard);
        camera.LookAt(0,target.GetPosition()[1],0);
        break;
    case KEY_DOWN:
        camera.Move(0, 0, -moveChunkKeyboard);
        camera.LookAt(0,target.GetPosition()[1],0);
        break;
    case KEY_LEFT:
        target.Rotate(0, -rotChunkKeyboard, 0);
        break;
    case KEY_RIGHT:
        target.Rotate(0, rotChunkKeyboard, 0);
        break;
    default:
        break;
    }
}

void KeyHandler::HandleUp(Key key) {
    switch (key) {
    case KEY_e:
        dragon->useBreathWeapon( false );
        break;
    case KEY_q:
        dragon->chargeFireball( false );
        break;
    default:
        break;
    }
}
