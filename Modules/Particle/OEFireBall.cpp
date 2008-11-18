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
                       TextureLoader& textureLoader): 
    FireEffect(system,
               100,     //numParticles
               10.0,     //number 
               2.0,     //numberVar
               300.0,  //life
               100.0,  //lifeVar
               .7,     //size
               0.1,     //sizeVar
               2.0,     //maxSize
               2*PI,    //angle
               0.1,    //spin
               0.1,     //spinVar
               .25,     //speed
               0.15,    //speedVar
               Vector<4,float>(.9,.9,0.0,.9),  //startColor
               Vector<4,float>(.9,0.0,0.0,.9), //endColor
               Vector<3,float>(0.0,0.0,0.0)),    //antigravity
    transMod(this,2.0),
    doFire(false)

{
    ITextureResourcePtr tex1 = 
        ResourceManager<ITextureResource>::Create("Smoke/smoke01.tga");
    textureLoader.Load(tex1);
    AddTexture(tex1);
    
    ITextureResourcePtr tex2 = 
        ResourceManager<ITextureResource>::Create("Smoke/smoke02.tga");
    textureLoader.Load(tex2);
    AddTexture(tex2);
    
    ITextureResourcePtr tex3 = 
        ResourceManager<ITextureResource>::Create("Smoke/smoke03.tga");
    textureLoader.Load(tex3);
    AddTexture(tex3);
}

OEFireBall::~OEFireBall() {} 

void OEFireBall::Fire() {
    doFire = true;
}

void OEFireBall::Handle(ParticleEventArg e) {
    FireEffect::Handle(e);
    for (particles->iterator.Reset(); 
         particles->iterator.HasNext(); 
         particles->iterator.Next()) {
    
        TYPE& particle = particles->iterator.Element();
        if (doFire)
            transMod.Process(particle);
    }

}
