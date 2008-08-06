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

#include <Core/IModule.h>
#include <Scene/LightNode.h>

using OpenEngine::Core::IModule;
using OpenEngine::Scene::LightNode;

class LightFader : public IModule {
private:
    LightNode* from;
    LightNode* to;
    LightNode* lightNode;
    float time, beginPct, endPct, timeSpend;
    bool done;

public:
    LightFader(LightNode& from, LightNode& to, LightNode* lightNode, float time);
    virtual ~LightFader();

    virtual void Initialize();
    virtual void Process(const float deltaTime, const float percent);
    virtual void Deinitialize();
    virtual bool IsTypeOf(const std::type_info& inf);
};


#endif // _LIGHT_FADER_H_
