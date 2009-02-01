// Breath weapon fire effect
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _DRAGON_PANIC_BREATH_WEAPON_
#define _DRAGON_PANIC_BREATH_WEAPON_

#include <Effects/FireEffect.h>

#include "HeightMapModifier.h"
#include "BoidsModifier.h"
#include "../Boid/BoidsSystem.h"
#include "../Island/HeightMap.h"

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
 * Breath Weapon particle system using OEParticleSystem
 * and Effect/FireEffect
 *
 * @class BreathWeapon BreathWeapon.h /DragonPanic/Modules/Particle/BreathWeapon.h
 */
class BreathWeapon : public FireEffect {
private:
    HeightMapModifier<FireEffect::TYPE> heightMod;
    BoidsModifier<FireEffect::TYPE> boidsMod;

public:
    BreathWeapon(OpenEngine::ParticleSystem::ParticleSystem& system,
                 TextureLoader& textureLoader,
                 HeightMap& heightMap,
                 BoidsSystem& boidsSystem);
    
    virtual ~BreathWeapon();

    void Handle(ParticleEventArg e);

};

#endif
