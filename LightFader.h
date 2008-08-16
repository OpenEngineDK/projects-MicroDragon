// Light fader
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _LIGHT_FADER_H_
#define _LIGHT_FADER_H_

// inherits from
#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Scene/LightNode.h>

#include <Utils/Timer.h>

using OpenEngine::Core::IListener;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Scene::LightNode;
using OpenEngine::Utils::Timer;

class LightFader : public IListener<ProcessEventArg> {
private:
    LightNode* from;
    LightNode* to;
    LightNode* lightNode;
    float time, beginPct, endPct, timeSpend;
    bool done;

    Timer timer;

public:
    LightFader(LightNode& from, LightNode& to, LightNode* lightNode, float time);
    virtual ~LightFader();

    void Handle(ProcessEventArg arg);
};


#endif // _LIGHT_FADER_H_
