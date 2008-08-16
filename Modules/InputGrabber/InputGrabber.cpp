#include "InputGrabber.h"

// from project
#include "../../Common/Follower.h"
#include "../../Common/OpenGLUtil.h"
#include "../../Common/VectorExt.h"
#include "../../Common/utilities.h"
#include "../Island/HeightMap.h"
#include "../Target/Target.h"

// from OpenEngine Core
#include <Display/Camera.h>
#include <Logging/Logger.h>
#include <Math/Math.h>
#include <Math/Quaternion.h>
#include <Renderers/IRenderingView.h>

#include <math.h>

using OpenEngine::Math::PI;
using OpenEngine::Math::Quaternion;
using std::min;
using std::max;

InputGrabber::InputGrabber(Camera* camera, HeightMap* heightMap, Target* target)
  : camera(camera), heightMap(heightMap), target(target) {
    pauseTime = false;
    focus = NULL;
    reset();
}

InputGrabber::~InputGrabber() {
    if( focus != NULL )
        delete focus;
}

void InputGrabber::reset() {
    target->setTarget(0,0,0);
    float rX = 11.5, rY = 0, d = 115;
    reset( rX, rY, d );
}

void InputGrabber::reset( float rotX, float rotY, float distance ) {
    this->rotX = rotX; this->rotY = rotY;
    this->distance = distance;
    globalScale = 1;
    rotXI = rotX;
    rotYI = rotY;
    distanceI = distance;
    globalScaleI = globalScale;

    if( focus != NULL )
        delete focus;
    focus = new Follower(Vector<3,float>(0,0,0));
    multiplier = 3; //cpvc: 1
    timeFactor = 1;
}

void InputGrabber::printLocation() {
    logger.info << "--------------------" << logger.end;
    logger.info << "rotX:" << customFmod(rotX,360);
    logger.info << " rotY:" << customFmod(rotY,360) << logger.end;
    logger.info << "distance:" << distance;
    logger.info << " globalScale:" << globalScale << logger.end;
    target->printTarget();
    logger.info << "--------------------" << logger.end;
}

void InputGrabber::togglePause() {
    pauseTime = !pauseTime;
}

void InputGrabber::Handle(InitializeEventArg arg) {
    timer.Start();
    reset();
}

void InputGrabber::Handle(ProcessEventArg arg) {
    unsigned int dt = timer.GetElapsedTimeAndReset().AsInt();
    float deltaTime = ((float)dt)/1000.0;
    float timeStep = deltaTime/1000.0;

    if (rotX>89.9) rotX = 89.9;
    if (rotX< -20) rotX =  -20;
    if (distance>500) distance = 500;
    if (distance<1.5) distance = 1.5;
    if (globalScale<1/distance) globalScale = 1/distance;

    // Initial camera setup
    focus->update(target->getTarget(), 30.0/sqrt(distance),
		  40.0/sqrt(distance), timeStep );

    Vector<3,float> focusPos = focus->getPos();
    focusPos = heightMap->HeightAt(focusPos);
    focusPos[1] = max(focusPos[1]+1.0f,1.0f);

    cameraDir =  VectorExt::
      RotateAround( VectorExt::
		    RotateAround(Vector<3,float>(0,0,1), 
				 Vector<3,float>(1,0,0),
				 -(rotX)/180.0*PI),
		    Vector<3,float>(0,1,0),
		    -rotY/180.0*PI);

    cameraPos = (cameraDir*distance)+focusPos;

    // ClearSight code to move camera free of blocking landscape
    float rotAdjust = 0;
    for (int d=1; d<230; d++) {
        float sDist = (d+0.0)/200.0*distance;

        Vector<3,float> camPos = (cameraDir*sDist)+focusPos;
        Vector<3,float> camPosOnSurface = heightMap->
	  HeightAt(camPos)+Vector<3,float>(0,1,0)*(-sDist*0.2*0.7);

        if ( camPos[1] < camPosOnSurface[1] ) {
	  float _rotAdjust = VectorExt::
	    GetAngle( (camPos-focusPos),camPosOnSurface-focusPos);
	  rotAdjust = max(rotAdjust,_rotAdjust);
        }
    }
    rotXI = rotXI+(rotX+rotAdjust-rotXI)*min(1.0f,timeStep*5);
    rotYI = rotYI+(rotY          -rotYI)*min(1.0f,timeStep*5);
    distanceI = distanceI*pow(distance/distanceI,timeStep*5);
    globalScaleI = distanceI*pow(globalScale/globalScaleI,timeStep*5);

    // Final camera setup
    focusPos = focusPos + Vector<3,float>(0,1,0)*distanceI*0.2;

    cameraDir = VectorExt::
      RotateAround(VectorExt::
		   RotateAround(Vector<3,float>(0,0,1),
				Vector<3,float>(1,0,0),
				-(rotXI)/180.0*PI),
		   Vector<3,float>(0,1,0),
		   -rotYI/180.0*PI);

    cameraPos = (cameraDir*distanceI)+focusPos;


    /*
      glTranslatef( 0, 0,  5 );
      glScalef(globalScale,globalScale,globalScale);
      glTranslatef( 0, 0, -5 );
      glTranslatef( 0, 0, -distanceI );
    */


    //logger.info << "rotXI: " << rotXI << " rotYI: " << rotYI << logger.end;

    Quaternion<float> y(0.0, PI*(rotYI/180.0), 0.0);
    Quaternion<float> x(PI*((double)rotXI/(double)180.0),
			Vector<3,float>(1.0,0.0,0.0));
    Vector<3,float> cPos =
      (y*x).RotateVector( Vector<3,float>(0.0,0.0,-distance) );

    camera->SetPosition(cPos);
    camera->LookAt(target->getTarget());
}

void InputGrabber::incMultiplier() {
    if( multiplier >= maxMultiplier )
        return;
    multiplier *= damper;
    logger.info << "multiplier: " << multiplier << logger.end;
}

void InputGrabber::decMultiplier() {
    if( multiplier <= minMultiplier )
        return;
    multiplier *= 1/damper;
    logger.info << "multiplier: " << multiplier << logger.end;
}

void InputGrabber::zoom(float speed) {
    if (speed<0) distance *= (1 +(-speed * multiplier));
    else         distance /= (1 +( speed * multiplier));
}

void InputGrabber::scaleGlobal(float scale) {
    globalScale *= (1 +(scale * multiplier));
    if (globalScale>1) globalScale = 1;
    if (globalScale<0.01) globalScale = 0.01;
}

void InputGrabber::rotateViewRelative( float x, float y ) {
    x *= multiplier;
    y *= multiplier;

    // Upward motion should make camera go upwards
    // at once despite intelligent camera
    if (x>0 && rotX<rotXI) { 
        rotX = rotXI;
    }

    rotX += x;
    rotY += y;
}

void InputGrabber::rotateViewAbsolute(float x, float y, 
				      float distance, float globalScale) {
    rotX = customFmod(x+180,360)-180;
    rotY = customFmod(y-rotY+180,360)-180+rotY;
    this->distance = distance;
    this->globalScale = globalScale;
}

void InputGrabber::updateDistance( float distance ) {
    this->distance = distance;
}

void InputGrabber::moveTarget( float x, float z ) {
    Vector<3,float> zDir = camera->GetPosition();
    zDir[1] = 0.0;
    if (!zDir.IsZero())
      zDir.Normalize();
    Vector<3,float> xDir = zDir % Vector<3,float>(0.0,-1.0,0.0);
    target->setTarget( target->getTarget() + (xDir*x+zDir*z) * multiplier );
}

bool InputGrabber::isTimePaused() {
  return pauseTime;
}

Vector<3,float> InputGrabber::GetCameraDir() {
  return cameraDir;
}
