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
               0.04,    //emitRate
               6.0,     //number 
               2.0,     //numberVar
               2.1,     //life
               0.5,     //lifeVar
               0.09,    //angle
               250.0,   //spin
               100.0,   //spinVar
               35.0,    //speed
               5.0,     //speedVar
               Vector<3,float>(0,0.295,0),       //antigravity
               textureLoader),    
    heightMod(heightMap),
    boidsMod(boidssystem, 3000) {

    ITextureResourcePtr tex1 = 
        ResourceManager<ITextureResource>::Create("Smoke/smoke01.tga");
    AddTexture(tex1);


    // color modifier
    colormod.AddValue( .9, Vector<4,float>(0.1, 0.01, .01, .4)); // blackish
    colormod.AddValue( .7, Vector<4,float>( .4,  0.3,  .1, .6)); // redish
    colormod.AddValue( .4, Vector<4,float>( .9, 0.75,  .2, .7)); // orangeish
    colormod.AddValue( .0, Vector<4,float>(0.2,  0.2,  .4, .3)); // blueish

    // size variations 
    sizem.AddValue(1.0, 2); 
    sizem.AddValue(.65, 6);
    sizem.AddValue( .2, 4);    
    sizem.AddValue( .0, 3);    
    
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
