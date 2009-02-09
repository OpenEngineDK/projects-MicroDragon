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
               200,     //numParticles
               6.0,     //number 
               2.0,     //numberVar
               2.1,     //life
               0.5,     //lifeVar
               3.0,     //size
               0.5,     //sizeVar
               9.0,     //maxSize
               0.11,    //angle
               230.0,     //spin
               100.0,     //spinVar
               45.0,    //speed
               10.0,    //speedVar
               Vector<4,float>(1.0,1.0,1.0,.4),  //startColor
               Vector<4,float>(1.,1.0,1.0,.3),   //endColor
               Vector<3,float>(0,0.295,0),       //antigravity
               textureLoader),    
    heightMod(heightMap),
    boidsMod(boidssystem, 300) {

    ITextureResourcePtr tex1 = 
        ResourceManager<ITextureResource>::Create("Smoke/smoke01.tga");
    AddTexture(tex1);
    
    system.ProcessEvent().Attach(*this);
    
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
        boidsMod.Process(e.dt, particle);
        
    }

}
