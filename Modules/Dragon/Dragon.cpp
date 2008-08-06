#include "Dragon.h"

#include "../../Common/FaceListUtil.h"
#include "../Island/Island.h"
#include "../Target/Target.h"
#include "../Particle/ParticleSystem.h"

#include <Resources/IModelResource.h>
#include <Resources/ResourceManager.h>
#include <Renderers/OpenGL/TextureLoader.h>
#include <Math/Math.h>
#include <Geometry/FaceSet.h>
#include <Geometry/Face.h>
#include <Math/Quaternion.h>

using namespace OpenEngine::Resources;
using namespace OpenEngine::Geometry;
using namespace OpenEngine::Renderers::OpenGL;
using OpenEngine::Math::PI;
using OpenEngine::Math::Quaternion;

Dragon* Dragon::instance = NULL;

Dragon* Dragon::getInstance(){
    if( instance == NULL )
        instance = new Dragon();
    return instance;
}

Dragon::Dragon() {
    jawPos = 0.0f;
    enableTexture = enabled = true;
    numberOfRenderStates = 3;
    renderState = numberOfRenderStates-1;
    usingBreathWeapon = chargingFireball = false;

    /*
    glRotatef(-5,1,0,0);

    glScalef(scale,scale,scale);
    glTranslatef(0.0,0.0,0.8);
    */

    headNode = new TransformationNode();    
    this->AddNode(headNode);

    TransformationNode* craniumNode = new TransformationNode();
    float scale = 0.2; //cpvc 5.0f;
    craniumNode->Scale(scale,scale,scale);
    craniumNode->SetPosition(Vector<3,float>(0.0,0.0,0.8));
    Quaternion<float> q( PI*(5/360),
			 Vector<3,float>(1.0,0.0,0.0) );
    craniumNode->SetRotation(q);
    headNode->AddNode(craniumNode);

    // offset the jaws angle node
    jawAngleNode = new TransformationNode();
    Vector<3,float> offset(0.0,-0.15,-15.7); //cpvc 0.0, -0.15, -0.7
    jawAngleNode->SetPosition(offset);
    craniumNode->AddNode(jawAngleNode);

    TransformationNode* jawNode = new TransformationNode();
    jawNode->SetPosition(-offset);
    jawAngleNode->AddNode(jawNode);

    IModelResourcePtr craniumRes = ResourceManager<IModelResource>::
      Create("Dragon/dragon-head.obj");
    craniumRes->Load();
    //FaceListUtil::Unitize(gNode->GetFaceSet());
    //FaceListUtil::OverrideSoftNormsWithHardNorm(gNode->GetFaceSet());
    craniumNode->AddNode(craniumRes->GetSceneNode());
	
    IModelResourcePtr jawRes = ResourceManager<IModelResource>::
      Create("Dragon/dragon-jaw.obj");
    jawRes->Load();
    //FaceListUtil::Unitize(gNode->GetFaceSet());
    //FaceListUtil::OverrideSoftNormsWithHardNorm(gNode->GetFaceSet());
    jawNode->AddNode(jawRes->GetSceneNode());

}

Dragon::~Dragon(){
}

void Dragon::Initialize() {
    usingBreathWeapon = false;
    chargingFireball = false;
    fireballCharge = 0.0;
    prevTime = 0.0;
    neckLength = 45-5;
    neck = new Tube( 2.5, 1.2, 1.0, 0.5, neckLength );
    headFocus = new Follower(Target::getInstance()->getTarget());
    headPos = new Follower(Target::getInstance()->getTarget());

   ITextureResourcePtr neck = ResourceManager<ITextureResource>
     ::Create("Dragon/neck.tga");
   neck->Load();
   TextureLoader::LoadTextureResource(neck);
   neckTextureID =  neck->GetID();
}

void Dragon::Deinitialize(){
    delete headFocus;
    delete headPos;
}

void Dragon::Process(const float deltaTime, const float percent) {
    OnLogicEnter(deltaTime/1000.0);
}

bool Dragon::IsTypeOf(const std::type_info& inf) {
    return (typeid(Dragon) == inf);
}

void Dragon::Apply(IRenderingView* rv) {
  if( enableTexture ) {
    glEnable( GL_TEXTURE_2D );
    glBindTexture(GL_TEXTURE_2D, neckTextureID);
  }
  neck->draw();
  if( enableTexture )
    glDisable( GL_TEXTURE_2D );

  if( enableTexture )
    glColor3f(1.0,1.0,1.0);
  else
    glColor3f( 0.2, 0.8, 0.2 );

  glPushMatrix();
  //get necks position
  float* headTransform = neck->getLinkMatrix(Tube::links-1);
  glMultMatrixf(headTransform);
  /* @todo
    float* headTransform = neck->getLinkMatrix(Tube::links-1);
    Matrix<4,4,float> m = Matrix<4,4,float>(headTransform);
    headNode->SetMatrix(m);
  */
  
  VisitSubNodes(*rv);
  glPopMatrix();
}

void Dragon::OnLogicEnter(float timeStep){
    double time = prevTime + timeStep;

    float unitsFromTarget = 12; // 12
    float moveNeckBack = 5.0;
    float dragonLength = neckLength+moveNeckBack;

    Vec3 startP = Vec3(30,-5,0);
    Vec3 tempDir = Target::getInstance()->getTarget()-(startP*Vec3(1,0,1));
    Vec3 attackDir = (
                      tempDir*((tempDir.getLength()/(dragonLength)))
                      +Vec3(0,-40,0)
                      -tempDir.normalize()*pow(20.0/tempDir.getLength(),2)
                      ).normalize();
    Vec3 headTarget = Target::getInstance()->getTarget()-attackDir*(unitsFromTarget+sin(time*PI*0.5));

    headFocus->update(Target::getInstance()->getTarget(),12,12,timeStep);
    headPos->update(headTarget,5,5,timeStep);

    Vec3 fireSourcePrev = fireSource;
    fireSource = headPos->getPos();
    if ((fireSource-startP).getLength()>dragonLength*0.8) {
        fireSource = startP+((fireSource-startP).normalize()*dragonLength*0.8);
    }
    Vec3 fireSourceHeight    = Island::getInstance()->heightAt(fireSource);
    Vec3 fireSourceMidHeight = Island::getInstance()->heightAt(fireSource*0.7+startP*0.3);
    if (fireSource.y-fireSourceHeight.y<5) {
        fireSource = fireSourceHeight+Vec3(0,5,0);
    }
    if (fireSource.y-fireSourceMidHeight.y<5) {
        fireSource = Vec3(fireSource.x,fireSourceMidHeight.y+5,fireSource.z);
    }

    fireDir = (headFocus->getPos()-fireSource).normalize();
    fireSource = fireSource-fireDir*moveNeckBack;
    fireSourceVel = (fireSource-fireSourcePrev)/timeStep;

    neck->update(startP,Vec3(0.1,1,0),Vec3(1,0,0),fireSource,fireDir,fireSource-startP,true);

    if (chargingFireball) jawPos = min(jawPos+timeStep*21,21.0f);
    else if (usingBreathWeapon) jawPos = min(jawPos+timeStep*120,21.0f);
    else jawPos -= timeStep*60;
    jawPos = max(0.0f,jawPos);

    Vec3 jawRotateAxis = neck->getLinkX(Tube::links-1);
    Vec3 newHeadDir = fireDir.rotateAround(jawRotateAxis,-jawPos/2/180*PI);
    neck->update(startP,Vec3(0.1,1,0),Vec3(1,0,0),fireSource,newHeadDir,fireSource-startP,false);
  
    // Create smoke from mouth while charging
    if (chargingFireball) {
        fireballCharge = min(fireballCharge+timeStep,1.0f);
        ParticleSystem::getInstance()->CreateParticles(
            time, prevTime, 25*fireballCharge,
            fireSource+fireDir*4.0+jawRotateAxis*0.75, jawRotateAxis*+5+fireDir*1+fireSourceVel/2, 3.0,
            1.0+1.0*fireballCharge, 1.0
            );
        ParticleSystem::getInstance()->CreateParticles(
            time, prevTime, 25*fireballCharge,
            fireSource+fireDir*4.0-jawRotateAxis*0.75, jawRotateAxis*-5+fireDir*1+fireSourceVel/2, 3.0,
            1.0+1.0*fireballCharge, 1.0
            );
    }
    // Create fireball
    else if (fireballCharge>0.1) {
        ParticleSystem::getInstance()->
	  CreateFireball(fireSource+fireDir*5,
			 (fireDir*75.0)+(fireSourceVel*0.5),
			 2.0+fireballCharge*5.0);
        fireballCharge = 0.0;
    }
    
    // Create breath weapon particles
    if (isUsingBreathWeapon()) {
      Vec3 sourceVec = fireSource+fireDir*3;
      Vec3 destinationVec = (fireDir*60.0)+(fireSourceVel*0.5);
      ParticleSystem::getInstance()->
	CreateParticles(time, prevTime, 100,
			sourceVec , destinationVec,
			7.0, 2.0, 2.0);
    }

    prevTime = time;

    // update jaw angle
    Quaternion<float> q( PI*(jawPos/180.0),
			 Vector<3,float>(1.0, 0.0, 0.0));
    jawAngleNode->SetRotation(q);
}

void Dragon::toggleRenderState(){
    enabled = true;
    renderState++;
    renderState %= numberOfRenderStates;

    switch ( renderState ){
    case 0: //disable texture
        enableTexture = false;
        break;
    case 1: //disable dragon
        enabled = false;
        break;
    default: //render all, reset variables
        enabled = true;
        enableTexture = true;
        break;
    }
}

bool Dragon::isUsingBreathWeapon(){
    return usingBreathWeapon;
}

void Dragon::useBreathWeapon( bool input ) {
    usingBreathWeapon = input;
}

void Dragon::chargeFireball( bool input ) {
    chargingFireball = input;
}
