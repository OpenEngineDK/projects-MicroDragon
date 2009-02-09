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
                     TextureLoader& textureLoader, BoidsSystem& boidsSystem): 
    FireEffect(system,
               100,     //numParticles
               20.0,    //number 
               5.0,     //numberVar
               0.2,     //life
               0.1,     //lifeVar
               3.0,     //size
               0.1,     //sizeVar
               6.0,     //maxSize
               2*PI,    //angle
               230,     //spin
               100,     //spinVar
               15,      //speed
               5,       //speedVar
               Vector<4,float>(.9,.9,0.0,.9),  //startColor
               Vector<4,float>(.9,0.0,0.0,.9), //endColor
               Vector<3,float>(0.0,0.0,0.0),   //antigravity
               textureLoader),
    charge(1.0), initLife(life), initSize(size), initSpeed(speed), 
    boidsMod(boidsSystem, 700), maxEmits(150)
{
    system.ProcessEvent().Attach(*this);
    ITextureResourcePtr tex1 = 
        ResourceManager<ITextureResource>::Create("Smoke/smoke01.tga");
    AddTexture(tex1);
    
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
    if (GetTotalEmits() >= maxEmits) {
        SetActive(false);
        Reset();
    }
    for (particles->iterator.Reset(); 
         particles->iterator.HasNext(); 
         particles->iterator.Next()) {
        TYPE& particle = particles->iterator.Element();
        boidsMod.Process(e.dt, particle);
    }
}

void Explosion::SetCharge(float p) {
    life = initLife + initLife * charge;
    size = initSize + initSize * charge;
    speed = initSpeed + initSpeed * charge;
    boidsMod.SetStrength(700*charge);
}

float Explosion::GetCharge() {
    return charge;
}
