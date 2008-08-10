#include "ParticleSystem.h"
#include "FireBall.h"

#include "../Boid/BoidsSystem.h"
#include "../Island/Island.h"
//#include "../Dragon/Dragon.h"
//#include "../../Common/gltga.h"

#include "Particle.h"
#include "../../Common/random.h"

#include <Display/IViewingVolume.h>
#include <Meta/OpenGL.h>
#include <Renderers/IRenderingView.h>
#include <Resources/ResourceManager.h>
#include <Resources/ITextureResource.h>
#include <Renderers/OpenGL/TextureLoader.h>

using namespace OpenEngine::Resources;
using namespace OpenEngine::Renderers::OpenGL;

#include <iostream>
using namespace std;

unsigned int textures[10];
int textureIndex;

ParticleSystem::ParticleSystem(Island* island, IViewingVolume* vv,
			       BoidsSystem* boidssystem) {
  this->island = island;
  this->vv = vv;
  this->boidssystem = boidssystem;
}

ParticleSystem::~ParticleSystem(){
}

void ParticleSystem::Initialize(){
    randObject = new RandomGenerator();

    textureIndex = 0;

    ITextureResourcePtr tex1 = 
      ResourceManager<ITextureResource>::Create("Smoke/smoke01.tga");
    tex1->Load();
    TextureLoader::LoadTextureResource(tex1);
    textures[ textureIndex ] = tex1->GetID();
    tex1->Unload();
    textureIndex++;

    ITextureResourcePtr tex2 = 
      ResourceManager<ITextureResource>::Create("Smoke/smoke02.tga");
    tex2->Load();
    TextureLoader::LoadTextureResource(tex2);
    textures[ textureIndex ] = tex2->GetID();
    tex2->Unload();
    textureIndex++;

    ITextureResourcePtr tex3 = 
      ResourceManager<ITextureResource>::Create("Smoke/smoke03.tga");
    tex3->Load();
    TextureLoader::LoadTextureResource(tex3);
    textures[ textureIndex ] = tex3->GetID();
    tex3->Unload();
    textureIndex++;
}

void ParticleSystem::Deinitialize(){
    delete randObject;
}

void ParticleSystem::Process(const float deltaTime, const float percent) {
  OnLogicEnter(deltaTime/1000.0);
}

bool ParticleSystem::IsTypeOf(const std::type_info& inf) {
    return (typeid(ParticleSystem) == inf);
}

void ParticleSystem::Apply(IRenderingView* rv) {
  OnRenderEnter(0.0);
}

struct closerToCamera : public binary_function<Particle*, Particle*, bool> {
    bool operator()(Particle* x, Particle* y) {
      return x->IsCloserToCamera(y);
    }
};

void ParticleSystem::CreateParticles(double time, double prevTime, 
				     float particlesPerSecond,
                                     Vector<3,float> position, 
				     Vector<3,float> velocity, 
				     float velocityRandomness,
                                     double size, double lifeTime) {
    
    // Create particles in the world at arbritrary
    // points in time, independent on frame rate
    int startI = int(prevTime*particlesPerSecond);
    int endI   = int(time    *particlesPerSecond);
    for (int i=startI; i<endI; i++) {
        Vector<3,float> randomVector = Vector<3,float>(
                                 randObject->ran_uniform_0to1()-0.5,
                                 randObject->ran_uniform_0to1()-0.5,
                                 randObject->ran_uniform_0to1()-0.5
                                 );
        Vector<3,float> pos = position;
        if (endI-startI>1) pos = pos-velocity*2/2*(time-prevTime);
        Particle* p = new Particle(island,vv,
                                   pos,
                                   velocity+(randomVector*velocityRandomness),
                                   size,
                                   lifeTime,
                                   randObject->ran_uniform_0to1()
                                   );
        // Update particles based on current time compared to birth time
        if (endI-startI>1)
	  p->update(time-(i/(particlesPerSecond*1.0)));

        tmpParticles.push_back(p);
    }
}

void ParticleSystem::OnLogicEnter(float timeStep){
    // Insert all new particles from tmpParticles list into particles list
    for (vector<Particle*>::iterator i=tmpParticles.begin(); i!=tmpParticles.end(); ++i) {
        particles.push_back(*i);
    }
    tmpParticles.clear();

    // Fire on boids
    for (vector<Particle*>::iterator i=particles.begin(); i!=particles.end(); ++i) {
        boidssystem->HandleFire((*i)->getPosition(),3.0);
    }

    // Gravity and slight randomness
    for (vector<Particle*>::iterator i=particles.begin(); i!=particles.end(); ++i) {
        (*i)->addExternalForce(Vector<3,float>(0,10,0));
        (*i)->addExternalForce(Vector<3,float>(randObject->ran_uniform_0to1()-0.5,
                                    randObject->ran_uniform_0to1()-0.5,
                                    randObject->ran_uniform_0to1()-0.5)*20);
    }

    // Update all particles
    for (vector<Particle*>::iterator i=particles.begin(); i!=particles.end(); ++i) {
        (*i)->update(timeStep);
    }

    // Delete dead particles
    for (vector<Particle*>::iterator i=particles.begin(); i!=particles.end(); ++i) {
        if ((*i)->isDead()) { particles.erase(i); --i; }
    }
}

void ParticleSystem::CreateFireball(Vector<3,float> position, Vector<3,float> velocity, float size) {
    Vector<3,float> randomVector = Vector<3,float>(
                             randObject->ran_uniform_0to1()-0.5,
                             randObject->ran_uniform_0to1()-0.5,
                             randObject->ran_uniform_0to1()-0.5
                             );
    FireBall* p = new FireBall( island, vv, boidssystem, this, position, velocity, size, 2.0, randObject->ran_uniform_0to1()*1.0 );
    particles.push_back(p);
}

void ParticleSystem::OnRenderEnter(float timeSte){
    // Sort particles for depth
    sort(particles.begin(),particles.end(),closerToCamera());

    // Draw particles
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    for (vector<Particle*>::iterator i=particles.begin(); i!=particles.end(); ++i) {
        (*i)->draw();
    }
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

}
