#include "BoidsSystem.h"

#include <Math/Math.h>
#include <Logging/Logger.h>

using OpenEngine::Math::PI;

BoidsSystem* BoidsSystem::instance = NULL;

BoidsSystem* BoidsSystem::getInstance()
{
    if( instance == NULL )
        instance = new BoidsSystem();
    return instance;
}

BoidsSystem::BoidsSystem() {
  enabled = true;
    numberOfRenderStates = 3;
    renderState = numberOfRenderStates-1;
    numberOfShownBoids = 0;
    disableLogic = false;
}

BoidsSystem::~BoidsSystem(){
}

void BoidsSystem::Initialize(){
    numberOfShownBoids = numberOfBoids;
    alignment = 0.3;
    // Init objects
    int gridSize = (int)sqrt((double)numberOfBoids);
    for (int i=0; i<gridSize; i++) {
        for (int j=0; j<gridSize; j++) {
            float val = (i+gridSize*j)*1.0/numberOfBoids;
            boids[i*gridSize+j] = new Boid(
                                           Vec3(i*2-(gridSize-1),0,j*2-(gridSize-1)),
                                           Vec3(0,0,1),
                                           Vec3(0,0,0),
                                           Vec3(sin(2*PI*(0.0/3+val))*0.5+0.5,sin(2*PI*(1.0/3+val))*0.5+0.5, 
                                                sin(2*PI*(2.0/3+val))*0.5+0.5).normalize());
        }
    }
}

void BoidsSystem::Deinitialize() {
}

void BoidsSystem::Process(const float deltaTime, const float percent) {
  OnLogicEnter(deltaTime/1000.0);
}

bool BoidsSystem::IsTypeOf(const std::type_info& inf) {
    return (typeid(BoidsSystem) == inf);
}

void BoidsSystem::Apply(IRenderingView* rv) {
  OnRenderEnter(0.0);
}


void BoidsSystem::OnLogicEnter(float timeStep){
  
    if (disableLogic) return;
    
    for (unsigned int i=0; i<numberOfShownBoids; i++) {
        for (unsigned int j=0; j<numberOfShownBoids; j++) {
            if (i!=j) boids[i]->handleBoid(boids[j]);
        }
    }
    for (unsigned int i=0; i<numberOfShownBoids; i++) {
        boids[i]->update(timeStep);
    }
}

void BoidsSystem::OnRenderEnter(float timeStep) {
    // Draw boids
    for (unsigned int i=0; i<numberOfShownBoids; i++) {
        boids[i]->draw();
    }
}

void BoidsSystem::HandleFire(Vec3 position, float strength) {
    for (int i=0; i<numberOfBoids; i++) {
        // Flee from fire
        boids[i]->HandleFire(position,strength);
    }
}

void BoidsSystem::HandleFireball(Vec3 position, float strength) {
    for (int i=0; i<numberOfBoids; i++) {
        // Blow away from fireball
        Vec3 dir = boids[i]->getPosition() - (position+Vec3(0,-5,0));
        boids[i]->addExternalImpulse((dir.normalize())*strength*(1/max(3.0f,dir.getLength())));
    }
}

void BoidsSystem::IncNumberOfShownBoids() {
    if( numberOfShownBoids < numberOfBoids )
        numberOfShownBoids += 1;
}

void BoidsSystem::DecNumberOfShownBoids() {
    if( numberOfShownBoids > 0 )
        numberOfShownBoids -= 1;
}

void BoidsSystem::IncAlignment() {
    if( alignment < 1 ) alignment += 0.05;
    logger.info << "Boids alignment: " << alignment << logger.end;
}

void BoidsSystem::DecAlignment() {
    if( alignment > 0 ) alignment -= 0.05;
    logger.info << "Boids alignment: " << alignment << logger.end;
}

float BoidsSystem::getAlignment() { return alignment; }

void BoidsSystem::toggleRenderState() {
    logger.info << "BoidsSystem status: ";
    enabled = true;
    renderState++;
    renderState %= numberOfRenderStates;
    switch ( renderState ){
    case 0: //without logic
        disableLogic = true;
        logger.info << "without logic";
        break;
    case 1: //disabled
        enabled = false;
        logger.info << "disabled";
        break;
    default: //render all, reset variables
        enabled = true;
        disableLogic = false;
        logger.info << "fully enabled";
        break;
    }
    logger.info << logger.end;
}
