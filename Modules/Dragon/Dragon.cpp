// header file
#include "Dragon.h"

// from module
#include "Tube.h"

// from project
#include "../../Common/Follower.h"

#include <Meta/OpenGL.h>

#include "../../Common/FaceListUtil.h"
#include "../../Common/VectorExt.h"
#include "../Island/Island.h"
#include "../Target/Target.h"
#include "../Particle/ParticleSystem.h"

#include <Resources/IModelResource.h>
#include <Resources/ResourceManager.h>
#include <Renderers/OpenGL/TextureLoader.h>
#include <Renderers/IRenderingView.h>
#include <Scene/TransformationNode.h>
#include <Math/Math.h>
#include <Geometry/FaceSet.h>
#include <Geometry/Face.h>
#include <Math/Quaternion.h>

#include <Geometry/Line.h>

using namespace OpenEngine::Resources;
using namespace OpenEngine::Geometry;
using namespace OpenEngine::Renderers::OpenGL;
using OpenEngine::Math::PI;
using OpenEngine::Math::Quaternion;

Dragon::Dragon(Island* island, Target* target, ParticleSystem* particlesystem)
  : island(island), target(target), particlesystem(particlesystem) {
    jawPos = 0.0f;
    enableTexture = enabled = true;
    numberOfRenderStates = 3;
    renderState = numberOfRenderStates-1;
    usingBreathWeapon = chargingFireball = false;

    //@todo does this make a difference?
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
    headFocus = new Follower(target->getTarget());
    headPos = new Follower(target->getTarget());

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

  list<Line*>::iterator i;
  for(i=bluelines.begin(); i != bluelines.end(); ++i)
    rv->GetRenderer()->DrawLine(*(*i), Vector<3,float>(0.0,0.0,1.0) , 1.5);
  bluelines.clear();
  
  for(i=redlines.begin(); i != redlines.end(); ++i)
    rv->GetRenderer()->DrawLine(*(*i), Vector<3,float>(1.0,0.0,0.0) , 1.5);
  redlines.clear();
  
  for(i=greenlines.begin(); i != greenlines.end(); ++i)
    rv->GetRenderer()->DrawLine(*(*i), Vector<3,float>(0.0,1.0,0.0) , 1.5);
  greenlines.clear();

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

    float unitsFromTarget = 12;
    float moveNeckBack = 5.0;
    float dragonLength = neckLength+moveNeckBack;

    Vector<3,float> startP = Vector<3,float>(30,-5,0);
    startP[1] = 0.0;

    Vector<3,float> tempDir = target->getTarget()-(startP);

    Vector<3,float> attackDir = (
                      tempDir*((tempDir.GetLength()/(dragonLength)))
                      +Vector<3,float>(0,-40,0)
                      -tempDir.GetNormalize()*pow(20.0/tempDir.GetLength(),2)
                      ).GetNormalize();

    Vector<3,float> headTarget = target->
      getTarget()-attackDir*(unitsFromTarget+sin(time*PI*0.5));

    headFocus->update(target->getTarget(),12,12,timeStep);
    headPos->update(headTarget,5,5,timeStep);

    Vector<3,float> fireSourcePrev = fireSource;
    fireSource = headPos->getPos();

    if ((fireSource-startP).GetLength()>dragonLength*0.8) {
        fireSource = startP+
	  ((fireSource-startP).GetNormalize()*dragonLength*0.8);
    }

    Vector<3,float> fireSourceHeight    = island->heightAt(fireSource);

    Vector<3,float> fireSourceMidHeight = island->
      heightAt(fireSource*0.7+startP*0.3);

    if (fireSource[1]-fireSourceHeight[1]<5) {
        fireSource = fireSourceHeight+Vector<3,float>(0,5,0);
    }

    if (fireSource[1]-fireSourceMidHeight[1]<5) {
        fireSource = Vector<3,float>(fireSource[0],
				     fireSourceMidHeight[1]+5,
				     fireSource[2]);
    }

    Vector<3,float> tmpDir(headFocus->getPos()-fireSource);
    if(!tmpDir.IsZero())
      fireDir = tmpDir.GetNormalize();

    fireSource = fireSource-fireDir*moveNeckBack;
    if (timeStep > 0.000001)
      fireSourceVel = (fireSource-fireSourcePrev)/timeStep;

    neck->update(startP,Vector<3,float>(0.1,1,0),Vector<3,float>(1,0,0),
		 fireSource,fireDir,fireSource-startP,true);

    if (chargingFireball) jawPos = min(jawPos+timeStep*21,21.0f);
    else if (usingBreathWeapon) jawPos = min(jawPos+timeStep*120,21.0f);
    else jawPos -= timeStep*60;
    jawPos = max(0.0f,jawPos);

    Vector<3,float> jawRotateAxis = neck->getLinkX(Tube::links-1);

    Vector<3,float> newHeadDir = VectorExt::
      RotateAround(fireDir,jawRotateAxis,-jawPos/2/180*PI);

    neck->update(startP,
		 Vector<3,float>(0.1,1,0),Vector<3,float>(1,0,0),
		 fireSource,newHeadDir,fireSource-startP,false);
    
	bluelines.push_back( new Line( fireSource,( fireSource + newHeadDir )*2.0 ) );
	bluelines.push_back( new Line( fireSource,( fireSource + (fireSource-startP) )*2.0 ) );
	
    for(int i=0; i< Tube::links; i++) {
      redlines.push_back( new Line( neck->getLinkPosition(i),( neck->getLinkPosition(i)+neck->getLinkX(i)*5 ) ) );
      greenlines.push_back( new Line( neck->getLinkPosition(i),( neck->getLinkPosition(i)+neck->getLinkY(i)*5 ) ) );
      bluelines.push_back( new Line( neck->getLinkPosition(i), (neck->getLinkPosition(i)+neck->getLinkZ(i)*5 ) ) );
    }
    /*
    bluelines.push_back( new Line(startP,startP+Vector<3,float>(1,0,0)) );
    redlines.push_back( new Line(fireSource,fireSource+fireSource-startP)); //up vector for head
    */

    // Create smoke from mouth while charging
    if (chargingFireball) {
        fireballCharge = min(fireballCharge+timeStep,1.0f);
        particlesystem->CreateParticles(
            time, prevTime, 25*fireballCharge,
            fireSource+fireDir*4.0+jawRotateAxis*0.75, 
	    jawRotateAxis*+5+fireDir*1+fireSourceVel/2, 3.0,
            1.0+1.0*fireballCharge, 1.0
            );
        particlesystem->CreateParticles(
            time, prevTime, 25*fireballCharge,
            fireSource+fireDir*4.0-jawRotateAxis*0.75, 
	    jawRotateAxis*-5+fireDir*1+fireSourceVel/2, 3.0,
            1.0+1.0*fireballCharge, 1.0
            );
    }
    // Create fireball
    else if (fireballCharge>0.1) {
        particlesystem->
	  CreateFireball(fireSource+fireDir*5,
			 (fireDir*75.0)+(fireSourceVel*0.5),
			 2.0+fireballCharge*5.0);
        fireballCharge = 0.0;
    }
    
    // Create breath weapon particles
    if (isUsingBreathWeapon()) {
      Vector<3,float> sourceVec = fireSource+fireDir*3;
      Vector<3,float> destinationVec = (fireDir*60.0)+(fireSourceVel*0.5);
      particlesystem->
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
    target->SetActive(input);
    usingBreathWeapon = input;
}

void Dragon::chargeFireball( bool input ) {
    chargingFireball = input;
}
