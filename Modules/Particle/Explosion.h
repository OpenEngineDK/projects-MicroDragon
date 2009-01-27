// fire ball effect
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _DRAGON_PANIC_EXPLOSION_
#define _DRAGON_PANIC_EXPLOSION_

#include <Effects/FireEffect.h>
#include <Math/Vector.h>

namespace OpenEngine {
    namespace Scene {
        class TransformationNode;
    }
    
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
using OpenEngine::Scene::TransformationNode;
using OpenEngine::Math::Vector;

//using OpenEngine::ParticleSystem::ParticleSystem;
using OpenEngine::ParticleSystem::ParticleEventArg;
/**
 * Fire ball explosion particle system using OEParticleSystem
 * and Effect/FireEffect
 *
 * @class Explosion Explosion.h /DragonPanic/Modules/Particle/Explosion.h
 */
class Explosion : public FireEffect {
private:
    TransformationNode tnode;

public:
    Explosion(OpenEngine::ParticleSystem::ParticleSystem& system,
               TextureLoader& textureLoader);
     
    virtual ~Explosion();

    void Handle(ParticleEventArg e);
    void Fire(Vector<3,float> pos);
};

#endif
