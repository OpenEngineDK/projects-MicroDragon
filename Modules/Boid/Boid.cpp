#include "Boid.h"

#include "BoidsSystem.h"
#include "../Island/Island.h"
#include "../OscSurface/OscSurface.h"
//#include "../Particle/ParticleSystem.h"
#include "../../Common/OpenGLUtil.h"
#include "../../Common/utilities.h"

#include <Math/Math.h>

using OpenEngine::Math::PI;

Boid::Boid( Vec3 position, Vec3 forward, Vec3 velocity, Vec3 color ) {
    this->position = position;
    this->forward = forward.normalize();
    this->velocity = velocity;
    this->color = color;
    up = Vec3(0,1,0);
    left = up.getCross(forward);
    mass = 1.0;
    max_force = 10.0;
    max_speed = 5.0;
    steering_accumulated = Vec3(0,0,0);
    external_force_accumulated = Vec3(0,0,0);
    external_impulse_accumulated = Vec3(0,0,0);
    walkCycle = 0.0;
    airborn = false;
    prevTime = 0.0;

    burned = 0.0;
    hot = 0.0;
    burning = false;
    drowning = false;
    life = 1.0;
    dead = false;
}

Boid::~Boid(){
}

Vec3 Boid::getPosition() { return position; }
Vec3 Boid::getVelocity() { return velocity; }

void Boid::addSteering( Vec3 steering, double weight ) {
    Vec3 vect = steering*weight;
    steering_accumulated = steering_accumulated + vect;
}

void Boid::addExternalForce( Vec3 force ) {
    external_force_accumulated = external_force_accumulated + force;
}

void Boid::addExternalImpulse( Vec3 impulse ) {
    external_impulse_accumulated = external_impulse_accumulated + impulse;
}

void Boid::addDesiredVelocity( Vec3 steering, double weight ) {
    Vec3 vect = ((steering.normalize()*max_speed) - velocity )*weight;
    steering_accumulated = steering_accumulated + vect;
}

void Boid::gotoTarget( Vec3 target, double radius, double weight, bool squared ) {
    Vec3 targetDir = target-position;
    double targetDist = targetDir.getLength();
    if (targetDist>0 && targetDist<radius) {
        double gotoStrength = (1/pow(targetDist,(squared?2:1)));
        addSteering(targetDir*gotoStrength,weight);
    }
}

void Boid::handleBoid(Boid* boid) {
    Vec3 otherDir = boid->getPosition()-position;
    double otherDist = otherDir.getLength();
  
    // Separation
    gotoTarget(boid->getPosition(),8.0,-8.0,true);

    // Cohesion
    //gotoTarget(boid->getPosition(),8.0,1.5,false);
    gotoTarget(boid->getPosition(),6.5+5.0*BoidsSystem::getInstance()->getAlignment(),1.5,false);
  
    // Look mostly at alignment of other boids in front
    double visualMultiplier = (forward.getDot(otherDir.normalize())+1)*0.5;

    // Alignment
    if (otherDist>0 && otherDist<8.0 && boid->getVelocity().getLength()>0) {
        double alignmentStrength = (1/otherDist)*BoidsSystem::getInstance()->getAlignment();
        addDesiredVelocity(boid->getVelocity(),alignmentStrength*visualMultiplier);
    }
}

void Boid::update( double timeDelta ) {
    updateSteering(timeDelta);
    updatePhysics(timeDelta);
    updateLocomotion(timeDelta);
    prevTime = prevTime+timeDelta;
}

void Boid::updateSteering( double timeDelta ) {
    steering_force = steering_accumulated.truncate(max_force);
    steering_accumulated = Vec3(0,0,0);
}

void Boid::updatePhysics( double timeDelta ) {
    // Gravity
    addExternalForce(Vec3(0,-20,0));
    if ((position+up).y<0.0) {
        addExternalForce(Vec3(
                              -velocity.x,
                              -velocity.y + 22,
                              -velocity.z
                              ));
    }

    // Add friction when not standing up on the ground (and alive)
    if (!airborn && (dead || up.getDot(getUprightVector())<0.8)) {
        addExternalForce(velocity*-5);
    }
  
    // Add steepness force and navigation
    if (!airborn) {
        Vec3 steepnessForce = Island::getInstance()->normalAt(position)*Vec3(1,0,1);
        if (steepnessForce.getLength()>0.2) {
            steepnessForce = steepnessForce.normalize()*pow(steepnessForce.getLength(),3);
            addExternalForce(steepnessForce*4);
        }
    }
    
    // Look ahead at landscape in front
    float aheadAndRight = Island::getInstance()->heightAt(position+forward*7-left*7).y;
    float aheadAndLeft  = Island::getInstance()->heightAt(position+forward*7+left*7).y;
    float aheadStraight = Island::getInstance()->heightAt(position+forward*7       ).y;
    // Add lava repulsion (force away from areas lower than zero)
    if (aheadAndRight<0) addSteering(left*-1*-5,1.0);
    if (aheadAndLeft <0) addSteering(left   *-5,1.0);
    if (aheadStraight<0) addSteering(forward*-5,2.0);
    // Add steepness avoision (force away from areas with different height than current)
    if (Abs(aheadAndRight-position.y)>5) addSteering(left*-1*-0.005,pow(Abs(position.y-aheadAndRight),3));
    if (Abs(aheadAndLeft -position.y)>5) addSteering(left   *-0.005,pow(Abs(position.y-aheadAndLeft ),3));
    if (Abs(aheadStraight-position.y)>5) addSteering(forward*-0.005,pow(Abs(position.y-aheadStraight),3));
  
    if (burning) addSteering(velocity*3,life);

    // Add steering force
    if (!airborn && !dead && up.getDot(getUprightVector())>0.8) {
        Vec3 steering_acceleration = steering_force*(1/mass)*timeDelta;
        velocity = (velocity+steering_acceleration).truncate(max_speed);
    }

    // Add external force
    Vec3 external_acceleration = external_force_accumulated*(1/mass)*timeDelta;
    external_force_accumulated = Vec3(0,0,0);
    velocity = (velocity+external_acceleration);
    
    // Update
    Vec3 positionPrev = position;
    position = position+(velocity*timeDelta);
    
    if (position.y<Island::getInstance()->heightAt(position).y+10.0*timeDelta) airborn = false; else airborn = true;
    
    Vec3 onGround = Island::getInstance()->heightAt(position);
    if (position.y<onGround.y) {
        position = onGround;
    }
    if (timeDelta>0 && airborn==false) velocity = (position-positionPrev)/timeDelta;

    // Add external impulse
    Vec3 external_impulse = external_impulse_accumulated*(1/mass);
    if (external_impulse.getLength()>0) {
        external_impulse_accumulated = Vec3(0,0,0);
        velocity = (velocity+external_impulse);
        position = position+(external_impulse*timeDelta);
        airborn = true;
    }
}

void Boid::updateLocomotion( double timeDelta ) {
    Vec3 normal = Island::getInstance()->normalAt(position);
    if (airborn && drowning) normal = Vec3(0,1,0);

    // Orientation
    if (airborn && !drowning) { // Falling in the air mode
        // Primary rule: local up axis approaches velocity vector:
        up = (up+(velocity*timeDelta*1.0).truncate(max_speed));
        up = Vec3(up.x,max(0.0f,up.y),up.z).normalize(); // head never lower than feet
        // Secondary rule: local left axis remains like it was before
        left = (up.getCross(forward)).normalize();
        forward = (left.getCross(up)).normalize();
    }
    else if (dead) { // Being dead mode
        // Primary rule: local up axis approaches tangent to surface
        up = (up+(left.getCross(normal)*timeDelta*6.0)).normalize();
        // Secondary rule: local forward axis approaches pointing down in the ground
        forward = (forward+(normal*-1*timeDelta*6.0)).normalize();
        left = (up.getCross(forward)).normalize();
        forward = (left.getCross(up)).normalize();
    }
    else { // Walking around mode
        // Primary rule: local up axis approaches global up axis* (*see getUprightVector())
        up = (up+(getUprightVector()*timeDelta*2.0)).normalize();
        // Secondary rule: local forward axis approaches velocity vector
        forward = (forward+(velocity*timeDelta*3.0).truncate(max_speed)).normalize();
        left = (up.getCross(forward)).normalize();
        forward = (left.getCross(up)).normalize();
    }

    running = min(1.0,velocity.getLength()/max_speed);
    walkCycle += timeDelta*sqrt(running);
    if (dead) walkCycle = 0;

    if ((position.y<0) && (position-(velocity*timeDelta)).y>0) {
        OscSurface::getInstance()->createRipple(position.x,position.z,1.2,0.3*velocity.getLength());
    }
    if ((position+up).y<0) { // in lava
        drowning = true;
        burned += 100.0;
    }
    else { // out of lava
        drowning = false;
    }
    if (burned>0) {
        hot += burned*timeDelta*0.005;
        life -= burned*timeDelta*0.0003;
        hot -= timeDelta/3.0;
        burned = 0;
    }
    if (hot>1 && life>0) {
        burning = true;
    }
    if (burning) {
      /* cpvc
        ParticleSystem::getInstance()->CreateParticles(
                                                       prevTime+timeDelta, prevTime, 25,
                                                       position+up, velocity, 3.0,
                                                       1.0, 1.0
                                                       );
      */
    }
    if (life<=0) { dead = true; }
    if (life<-0.4) { burning = false; life = 0; }
}

Vec3 Boid::getUprightVector() {
    // Upright vector is global up vector
    Vec3 upright = Vec3(0,1,0);
    // If running fast downwards, influence upright vector
    // with vector perpendicular to velocity, so the boid
    // runs a bit more perpenducular to the surface
    if (velocity.y<0) {
        upright = upright+(velocity.getCross(left)*0.2);
    }
    return upright.normalize();
}

void Boid::HandleFire(Vec3 firePosition, float fireStrength) {
    // Flee from fire
    gotoTarget(firePosition,16.0,-16.0*fireStrength*0.01,true);

    // Get burned
    float fireDist = (firePosition-position).getLength();
    if (fireDist<4.0) {
        burned += (4.0/fireDist)*fireStrength;
    }
}

void Boid::draw( ) {
    draw2(false);
  
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
    glColor4f( 0.0, 0.0, 0.0, 0.3 );
    Vec3 n = Island::getInstance()->normalAt(position);
    float matrix[] = {
        1, -n.x, 0,0,
        0,    1, 0,0,
        0, -n.z, 1,0,
        position.x,Island::getInstance()->heightAt(position).y+0.1,position.z,1};
    glMultMatrixf(matrix);
    glScalef( 1.0, 0.0, 1.0 );
    draw2(true);
    glPopMatrix();
    glDisable(GL_BLEND);
}

void Boid::draw2( bool shadow ) {
    glPushMatrix(); // Minion
  
    glTranslatef( 0.0, (1-up.y)*0.3, 0.0 );
    if (!shadow) { glTranslatef(position.x,position.y,position.z); }
    float matrix[] = {
        left.x    ,left.y    ,left.z    ,0,
        up.x      ,up.y      ,up.z      ,0,
        forward.x ,forward.y ,forward.z ,0,
        0         ,0         ,0         ,1};
    glMultMatrixf(matrix);
  
    // Up and down motion due to running:
    double runningSqrt = sqrt(running);
    glTranslatef( 0.0, (0.05-0.05*cos(walkCycle*8*PI+PI*3/8))*runningSqrt, 0.0 );
  
    Vec3 shirtColor = Vec3( 0.30, 0.25, 0.20 )*0.80+color*0.20;
    float l = life;

    glPushMatrix(); // Leg
    glTranslatef( -0.15, 0.7, 0.0 );
    glRotatef((+10-45*sin(walkCycle*4*PI))*runningSqrt,1,0,0);
    glPushMatrix(); // Actual Leg
    if (!shadow) glColor3f( 0.4*l, 0.3*l, 0.2*l );
    glTranslatef( 0.0, -0.2, 0.0 );
    glScalef( 0.2, 0.6, 0.2 );
    OpenGLUtil::GLSolidCube( 1.0 );
    glPopMatrix();
    glPushMatrix(); // Foot
    if (!shadow) glColor3f( 0.2*l, 0.2*l, 0.2*l );  
    glTranslatef( 0.0, -0.6, 0.1 );
    glScalef( 0.2, 0.2, 0.4 );
    OpenGLUtil::GLSolidCube( 1.0 );
    glPopMatrix();
    glPopMatrix();
    glPushMatrix(); // Leg
    glTranslatef( 0.15, 0.7, 0.0 );  
    glRotatef((+10+45*sin(walkCycle*4*PI))*runningSqrt,1,0,0);
    glPushMatrix(); // Actual Leg
    if (!shadow) glColor3f( 0.4*l, 0.3*l, 0.2*l );
    glTranslatef( 0.0, -0.2, 0.0 );
    glScalef( 0.2, 0.6, 0.2 );
    OpenGLUtil::GLSolidCube( 1.0 );
    glPopMatrix();
    glPushMatrix(); // Foot
    if (!shadow) glColor3f( 0.2*l, 0.2*l, 0.2*l );
    glTranslatef( 0.0, -0.6, 0.1 );
    glScalef( 0.2, 0.2, 0.4 );
    OpenGLUtil::GLSolidCube( 1.0 );
    glPopMatrix();
    glPopMatrix();
    glPushMatrix(); // Arm
    glTranslatef( -0.3, 1.3, 0.0 );
    glRotatef(-90*running*running+25*sin(walkCycle*4*PI)*sin(running*PI),1,0,0);
    glPushMatrix(); // Actual arm
    if (!shadow) glColor3f( shirtColor.x*l, shirtColor.y*l, shirtColor.z*l );
    glTranslatef( 0.0, -0.25, 0.0 );
    glScalef( 0.2, 0.6, 0.2 );
    OpenGLUtil::GLSolidCube( 1.0 );
    glPopMatrix();
    glPushMatrix(); // Hand
    if (!shadow) glColor3f( 0.7*l, 0.6*l, 0.5*l );
    glTranslatef( 0.0, -0.55, 0.0 );
    OpenGLUtil::GLSolidSphere( 0.1, 8, 4 );
    glPopMatrix();
    glPopMatrix();
    glPushMatrix(); // Arm
    glTranslatef( 0.3, 1.3, 0.0 );
    glRotatef(-90*running*running-25*sin(walkCycle*4*PI)*sin(running*PI),1,0,0);
    glPushMatrix(); // Actual arm
    if (!shadow) glColor3f( shirtColor.x*l, shirtColor.y*l, shirtColor.z*l );
    glTranslatef( 0.0, -0.25, 0.0 );
    glScalef( 0.2, 0.6, 0.2 );
    OpenGLUtil::GLSolidCube( 1.0 );
    glPopMatrix();
    glPushMatrix(); // Hand
    if (!shadow) glColor3f( 0.7*l, 0.6*l, 0.5*l );
    glTranslatef( 0.0, -0.55, 0.0 );
    OpenGLUtil::GLSolidSphere( 0.1, 8, 4 );
    glPopMatrix();
    glPopMatrix();
    glPushMatrix(); // Body
    if (!shadow) glColor3f( shirtColor.x*l, shirtColor.y*l, shirtColor.z*l );
    glTranslatef( 0.0, 1.05, 0.0 );
    glScalef( 0.5, 0.7, 0.3 );
    OpenGLUtil::GLSolidCube( 1.0 );
    glPopMatrix();
    glPushMatrix(); // Head
    if (!shadow) glColor3f( 0.7*l, 0.6*l, 0.5*l );
    glTranslatef( 0.0, 1.6, 0.0 );
    glScalef( 0.4, 0.4, 0.4 );
    OpenGLUtil::GLSolidSphere( 0.5, 12, 6 );
    glPopMatrix();
  
    glPopMatrix();
  
}
