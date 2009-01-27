// Fire ball effect
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "Explosion.h"

#include <ParticleSystem/ParticleSystem.h>
#include <Renderers/TextureLoader.h>
#include <Resources/ResourceManager.h>
#include <Resources/ITextureResource.h>


using OpenEngine::Resources::ResourceManager;
using OpenEngine::Resources::ITextureResource;

Explosion::Explosion(OpenEngine::ParticleSystem::ParticleSystem& system,
                       TextureLoader& textureLoader): 
    FireEffect(system,
               100,     //numParticles
               14.0,     //number 
               2.0,     //numberVar
               400.0,  //life
               100.0,  //lifeVar
               2.5,     //size
               0.1,     //sizeVar
               3.0,     //maxSize
               2*PI,    //angle
               0.1,    //spin
               0.1,     //spinVar
               1.0,     //speed
               0.15,    //speedVar
               Vector<4,float>(.9,.9,0.0,.9),  //startColor
               Vector<4,float>(.9,0.0,0.0,.9), //endColor
               Vector<3,float>(0.0,0.0,0.0),   //antigravity
               textureLoader) {
    system.ProcessEvent().Attach(*this);
    ITextureResourcePtr tex1 = 
        ResourceManager<ITextureResource>::Create("Smoke/smoke01.tga");
    AddTexture(tex1);
    
    ITextureResourcePtr tex2 = 
        ResourceManager<ITextureResource>::Create("Smoke/smoke02.tga");
    AddTexture(tex2);
    
    ITextureResourcePtr tex3 = 
        ResourceManager<ITextureResource>::Create("Smoke/smoke03.tga");
    AddTexture(tex3);

    SetTransformationNode(&tnode);
}

Explosion::~Explosion() {
    system.ProcessEvent().Detach(*this);
} 

void Explosion::Fire(Vector<3,float> pos) {
    GetTransformationNode()->SetPosition(pos);
    SetActive(true);
}

void Explosion::Handle(ParticleEventArg e) {
    FireEffect::Handle(e);
    for (particles->iterator.Reset(); 
         particles->iterator.HasNext(); 
         particles->iterator.Next()) {
        //TYPE& particle = particles->iterator.Element();
    }
}
