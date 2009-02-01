// Fire ball effect
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "OEFireBall.h"

#include <Math/Vector.h>
#include <ParticleSystem/ParticleSystem.h>
#include <Renderers/TextureLoader.h>
#include <Resources/ResourceManager.h>
#include <Resources/ITextureResource.h>

using OpenEngine::Math::Vector;
using OpenEngine::Resources::ResourceManager;
using OpenEngine::Resources::ITextureResource;

OEFireBall::OEFireBall(OpenEngine::ParticleSystem::ParticleSystem& system,
                       TextureLoader& textureLoader,
                       HeightMap& heightMap,
                       BoidsSystem& boidsSystem): 
    FireEffect(system,
               50,     //numParticles
               10.0,   //number 
               2.0,    //numberVar
               350.0,  //life
               100.0,  //lifeVar
               3.0,    //size
               0.1,    //sizeVar
               1.5,    //maxSize
               2*PI,   //angle
               0.1,    //spin
               0.1,    //spinVar
               .30,    //speed
               0.15,   //speedVar
               Vector<4,float>(.9,.9,0.0,.9),  //startColor
               Vector<4,float>(.9,0.0,0.0,.9), //endColor
               Vector<3,float>(0.0,0.0,0.0),   //antigravity
               textureLoader),                 
    exp(Explosion(system, textureLoader, boidsSystem)),
    transMod(*this, 3.0, heightMap, exp),
    charging(false),
    firing(false),
    charge(0.0), chargeStep(0.01), initLife(life), initSize(sizemod.GetMaxSizeVar()),
    initSpeed(3.0)
    
{
    // receive processing time
    system.ProcessEvent().Attach(*this);

    // load textures
    ITextureResourcePtr tex1 = 
        ResourceManager<ITextureResource>::Create("Smoke/smoke01.tga");
    AddTexture(tex1);
//     ITextureResourcePtr tex2 = 
//         ResourceManager<ITextureResource>::Create("Smoke/smoke02.tga");
//     AddTexture(tex2);
//     ITextureResourcePtr tex3 = 
//         ResourceManager<ITextureResource>::Create("Smoke/smoke03.tga");
//     AddTexture(tex3);

    // attach explosion rendernode 
    GetSceneNode()->AddNode(exp.GetSceneNode());
}

OEFireBall::~OEFireBall() {
    system.ProcessEvent().Detach(*this);
} 

void OEFireBall::Charge() {
    if (charging || firing)
        return;
    sizemod.SetMaxSizeVar(initSize);
    life = initLife;
    charge = 0.0;
    charging = true;
    SetActive(true);
}

void OEFireBall::Fire() {
    charging = false;
    if (charge < 0.1) {
        SetActive(false);
        return;
    }
    firing = true;
    exp.SetCharge(charge);
    transMod.SetSpeed(initSpeed + 2*initSpeed * charge);
    transMod.SetActive(true);
}

void OEFireBall::Handle(ParticleEventArg e) {
    if (charging) {
        charge += chargeStep;
        if (charge > 1.0) {
            charge = 1.0;
        }
        life = initLife + 100 * charge;
        sizemod.SetMaxSizeVar(initSize + 10 * charge);
    }
    FireEffect::Handle(e);
//     for (particles->iterator.Reset(); 
//          particles->iterator.HasNext(); 
//          particles->iterator.Next()) {
//         TYPE& particle = particles->iterator.Element();
//     }
    transMod.Process();
        
}

void OEFireBall::Reset() {
    FireEffect::Reset();
    firing = charging = false;
};
