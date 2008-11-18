// fire ball effect
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _DRAGON_PANIC_OE_FIRE_BALL_
#define _DRAGON_PANIC_OE_FIRE_BALL_

#include <Effects/FireEffect.h>

#include "TransformationModifier.h"

namespace OpenEngine {
    namespace ParticleSystem {
        class ParticleSystem;
        class ParticleEventArg;
    }

    namespace Renderers {
        class TextureLoader;
    }
}

using OpenEngine::Effects::FireEffect;
using OpenEngine::Renderers::TextureLoader;

//using OpenEngine::ParticleSystem::ParticleSystem;
using OpenEngine::ParticleSystem::ParticleEventArg;
/**
 * Fire ball particle system using OEParticleSystem
 * and Effect/FireEffect
 *
 * @class OEFireBall OEFireBall.h /DragonPanic/Modules/Particle/OEFireBall.h
 */
class OEFireBall : public FireEffect {
private:
    TransformationModifier<TYPE> transMod;
    bool doFire;
public:
    OEFireBall(OpenEngine::ParticleSystem::ParticleSystem& system,
               TextureLoader& textureLoader);

    virtual ~OEFireBall();

    void Handle(ParticleEventArg e);

    void Fire();
};

#endif
