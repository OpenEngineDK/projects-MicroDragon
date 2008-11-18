// Boid fire effect
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _DRAGON_PANIC_BOID_FIRE_
#define _DRAGON_PANIC_BOID_FIRE_

#include <Effects/FireEffect.h>


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
 * Boid fire particle system using OEParticleSystem
 * and Effect/FireEffect
 *
 * @class BoidFire BoidFire.h /DragonPanic/Modules/Particle/BoidFire.h
 */
class BoidFire : public FireEffect {
private:
public:
    BoidFire(OpenEngine::ParticleSystem::ParticleSystem& system,
             TextureLoader& textureLoader);

    virtual ~BoidFire();

//     void Handle(ParticleEventArg e);

};

#endif
