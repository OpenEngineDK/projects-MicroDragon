// Breath weapon fire effect
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "BreathWeapon.h"

#include <Math/Vector.h>
#include <ParticleSystem/ParticleSystem.h>
#include <Renderers/TextureLoader.h>
#include <Resources/ResourceManager.h>
#include <Resources/ITextureResource.h>

using OpenEngine::Math::Vector;
using OpenEngine::Resources::ResourceManager;
using OpenEngine::Resources::ITextureResource;

BreathWeapon::BreathWeapon(OpenEngine::ParticleSystem::ParticleSystem& system,
                           TextureLoader& textureLoader,
                           HeightMap& heightMap,
                           BoidsSystem& boidssystem): 
    FireEffect(system,
               300,     //numParticles
               6.0,     //number 
               2.0,     //numberVar
               2100.0,  //life
               1000.0,  //lifeVar
               3.0,     //size
               0.5,     //sizeVar
               5.0,     //maxSize
               0.05,    //angle
               0.2,    //spin
               0.1,     //spinVar
               2.0,     //speed
               0.25,    //speedVar
               Vector<4,float>(1.0,1.0,1.0,.4),  //startColor
               Vector<4,float>(1.,1.0,1.0,.3), //endColor
               Vector<3,float>(0,0.394,0),     //antigravity
               textureLoader),    
    heightMod(heightMap),
    boidsMod(boidssystem)
{
    ITextureResourcePtr tex1 = 
        ResourceManager<ITextureResource>::Create("Smoke/smoke01.tga");
    AddTexture(tex1);
    
    system.ProcessEvent().Attach(*this);

//     ITextureResourcePtr tex2 = 
//         ResourceManager<ITextureResource>::Create("Smoke/smoke02.tga");
//     textureLoader.Load(tex2);
//     AddTexture(tex2);

//     ITextureResourcePtr tex3 = 
//         ResourceManager<ITextureResource>::Create("Smoke/smoke03.tga");
//     textureLoader.Load(tex3);
//     AddTexture(tex3);

//     ITextureResourcePtr tex1 = 
//         ResourceManager<ITextureResource>::Create("textures/RealFlame_10.tga");
//     textureLoader.Load(tex1);
//     AddTexture(tex1);
    
}

BreathWeapon::~BreathWeapon() {
    system.ProcessEvent().Detach(*this);
} 

void BreathWeapon::Handle(ParticleEventArg e) {
    FireEffect::Handle(e);
    for (particles->iterator.Reset(); 
         particles->iterator.HasNext(); 
         particles->iterator.Next()) {
    
        TYPE& particle = particles->iterator.Element();
        heightMod.Process(particle);
        boidsMod.Process(particle);
        
    }

}
