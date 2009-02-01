// Boid fire effect
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "BoidFire.h"

#include <Math/Vector.h>
#include <ParticleSystem/ParticleSystem.h>
#include <Renderers/TextureLoader.h>
#include <Resources/ResourceManager.h>
#include <Resources/ITextureResource.h>

using OpenEngine::Math::Vector;
using OpenEngine::Resources::ResourceManager;
using OpenEngine::Resources::ITextureResource;

BoidFire::BoidFire(OpenEngine::ParticleSystem::ParticleSystem& system,
                   TextureLoader& textureLoader): 
    FireEffect(system,
               20,     //numParticles
               4.0,     //number 
               2.0,     //numberVar
               300.0,  //life
               100.0,  //lifeVar
               .5,     //size
               0.1,     //sizeVar
               2.0,     //maxSize
               0.4,    //angle
               0.09,    //spin
               0.1,     //spinVar
               -.5,     //speed
               0.25,    //speedVar
               Vector<4,float>(.9,.9,0.0,.9),  //startColor
               Vector<4,float>(.8,0.0,0.0,.3), //endColor
               Vector<3,float>(0,0.182,0),     //antigravity
               textureLoader)    
{
    ITextureResourcePtr tex1 = 
        ResourceManager<ITextureResource>::Create("Smoke/smoke01.tga");
    AddTexture(tex1);

    system.ProcessEvent().Attach(*this);
}

BoidFire::~BoidFire() {
    system.ProcessEvent().Detach(*this);
} 

// void BoidFire::Handle(ParticleEventArg e) {
//     FireEffect::Handle(e);
//     for (particles->iterator.Reset(); 
//          particles->iterator.HasNext(); 
//          particles->iterator.Next()) {
    
//         TYPE& particle = particles->iterator.Element();
//         heightMod.Process(particle);
//         boidsMod.Process(particle);
        
//     }

// }
