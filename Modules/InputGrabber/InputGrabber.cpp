#include "InputGrabber.h"

#include "../Island/Island.h"
#include "../../Common/utilities.h"
#include "../../Common/OpenGLUtil.h"

#include <Math/Math.h>
#include <Logging/Logger.h>

using OpenEngine::Math::PI;

Vec3 cameraDir;
Vec3 cameraPos;

InputGrabber* InputGrabber::instance = NULL;

InputGrabber* InputGrabber::getInstance(){
    if( instance == NULL )
        instance = new InputGrabber();
    return instance;
}

InputGrabber::InputGrabber() {
    pauseTime = false;
    target = NULL;
    focus = NULL;
    reset();
}

InputGrabber::~InputGrabber() {
    if( focus != NULL )
        delete focus;
}

void InputGrabber::reset() {
    target = Target::getInstance();
    target->setTarget(0,0,0);
    float rX = 11.5, rY = 0, rZ = 0, d = 115;
    reset( rX, rY, rZ, d );
}

void InputGrabber::reset( float rotX, float rotY, float rotZ, float distance ) {
    this->rotX = rotX; this->rotY = rotY; this->rotZ = rotZ;
    this->distance = distance;
    globalScale = 1;
    rotXI = rotX;
    rotYI = rotY;
    distanceI = distance;
    globalScaleI = globalScale;

    if( focus != NULL )
        delete focus;
    focus = new Follower(Vec3(0,0,0));
    multiplier = 1;
    timeFactor = 1;
}

void InputGrabber::printLocation() {
    logger.info << "--------------------" << logger.end;
    logger.info << "rotX:" << customFmod(rotX,360) << " rotY:" << customFmod(rotY,360) << " rotZ:" << customFmod(rotZ,360) << logger.end;
    logger.info << "distance:" << distance << " globalScale:" << globalScale << logger.end;
    target->printTarget();
    logger.info << "--------------------" << logger.end;
}

void InputGrabber::togglePause() {
    pauseTime = !pauseTime;
}

Vec3 InputGrabber::getTarget() {
    return target->getTarget();
}

void InputGrabber::Initialize() {
    reset();
}

void InputGrabber::Deinitialize() {
}

void InputGrabber::Process(const float deltaTime, const float percent) {
  OnLogicEnter(deltaTime/1000.0);
}

bool InputGrabber::IsTypeOf(const std::type_info& inf) {
    return (typeid(InputGrabber) == inf);
}

void InputGrabber::Apply(IRenderingView* rv) {
  OnRenderEnter(0.0);
  VisitSubNodes(*rv);
  OnRenderLeave(0.0);
}

void InputGrabber::OnLogicEnter(float timeStep) {
    if (rotX>89.9) rotX = 89.9;
    if (rotX< -20) rotX =  -20;
    if (distance>500) distance = 500;
    if (distance<1.5) distance = 1.5;
    if (globalScale<1/distance) globalScale = 1/distance;

    // Initial camera setup
    focus->update(getTarget(), 30.0/sqrt(distance), 40.0/sqrt(distance), timeStep );
    Vec3 focusPos = focus->getPos();
    focusPos = Island::getInstance()->heightAt(focusPos);
    focusPos.y = max(focusPos.y+1.0f,1.0f);
    cameraDir = Vec3(0,0,1).rotateAround(Vec3(1,0,0),-(rotX)/180.0*PI).rotateAround(Vec3(0,1,0),-rotY/180.0*PI);
    cameraPos = (cameraDir*distance)+focusPos;

    // ClearSight code to move camera free of blocking landscape
    float rotAdjust = 0;
    for (int d=1; d<230; d++) {
        float sDist = (d+0.0)/200.0*distance;
        Vec3 camPos = (cameraDir*sDist)+focusPos;
        Vec3 camPosOnSurface = Island::getInstance()->heightAt(camPos)+Vec3(0,1,0)*(-sDist*0.2*0.7);
        if ( camPos.y < camPosOnSurface.y ) {
            float _rotAdjust = (camPos-focusPos).getAngle(camPosOnSurface-focusPos);
            rotAdjust = max(rotAdjust,_rotAdjust);
        }
    }
    rotXI = rotXI+(rotX+rotAdjust-rotXI)*min(1.0f,timeStep*5);
    rotYI = rotYI+(rotY          -rotYI)*min(1.0f,timeStep*5);
    distanceI = distanceI*pow(distance/distanceI,timeStep*5);
    globalScaleI = distanceI*pow(globalScale/globalScaleI,timeStep*5);

    // Final camera setup
    focusPos = focusPos + Vec3(0,1,0)*distanceI*0.2;
    cameraDir = Vec3(0,0,1).rotateAround(Vec3(1,0,0),-(rotXI)/180.0*PI).rotateAround(Vec3(0,1,0),-rotYI/180.0*PI);
    cameraPos = (cameraDir*distanceI)+focusPos;
}

void InputGrabber::OnRenderEnter(float timeStep) {
    glPushMatrix();
    glTranslatef( 0, 0,  5 );
    glScalef(globalScale,globalScale,globalScale);
    glTranslatef( 0, 0, -5 );
    glTranslatef( 0, 0, -distanceI );
    glRotatef( rotZ,  0.0, 0.0, 1.0 );
    glRotatef( rotXI, 1.0, 0.0, 0.0 );
    glRotatef( rotYI, 0.0, 1.0, 0.0 );
    Vec3 focusPos = focus->getPos()+Vec3(0,1,0)*distanceI*0.2;
    glTranslatef( -focusPos.x, -focusPos.y, -focusPos.z);
}

void InputGrabber::OnRenderLeave(float timeStep) {
    glPopMatrix();
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
    // Upward motion should make camera go upwards at once despite intelligent camera
    if (x>0 && rotX<rotXI) { 
        rotX = rotXI;
    }
    rotX += x;
    rotY += y;
}

void InputGrabber::rotateViewAbsolute(float x, float y, float distance, float globalScale) {
    rotX = customFmod(x+180,360)-180;
    rotY = customFmod(y-rotY+180,360)-180+rotY;
    this->distance = distance;
    this->globalScale = globalScale;
}

void InputGrabber::updateDistance( float distance ) {
    this->distance = distance;
}

void InputGrabber::moveTarget( float x, float z ) {
    x *= multiplier;
    z *= multiplier;
    Vec3 screenZ = Vec3(-sin(rotY*PI/180.0),0,cos(rotY*PI/180.0));
    Vec3 screenX = Vec3(0,1,0).getCross(screenZ);
    Vec3 targetP = getTarget();
    targetP = targetP + (screenX*x+screenZ*z)*sqrt(distance);
    target->setTarget(targetP);
}

bool InputGrabber::isTimePaused() {
  return pauseTime;
}
