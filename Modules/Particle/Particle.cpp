#include "Particle.h"
#include "../Island/Island.h"
#include "../../Common/OpenGLUtil.h"

#include <Math/Math.h>

using OpenEngine::Math::PI;

extern Vec3 cameraDir;
extern unsigned int textures[10];

Particle::Particle(Vec3 position, Vec3 velocity, double size, double lifeTime, double randomValue) {
    this->position = position;
    this->forward = forward.normalize();
    this->velocity = velocity;
    this->size = size;
    this->randomValue = randomValue;
    lifeTimeTotal = lifeTime;
    lifeTimeLeft = lifeTime;

    up = Vec3(0,1,0);
    left = up.getCross(forward);
    mass = 1.0;
    max_force = 10.0;
    max_speed = 5.0;
    external_force_accumulated = Vec3(0,0,0);
    external_impulse_accumulated = Vec3(0,0,0);
    dead = false;
}

Particle::~Particle(){
}

Vec3 Particle::getPosition() { return position; }
Vec3 Particle::getVelocity() { return velocity; }

bool Particle::isDead() { return dead; }

void Particle::addExternalForce( Vec3 force ) {
    external_force_accumulated = external_force_accumulated + force;
}

void Particle::addExternalImpulse( Vec3 impulse ) {
    external_impulse_accumulated = external_impulse_accumulated + impulse;
}

void Particle::update( double timeDelta ) {
    updatePhysics(timeDelta);
    lifeTimeLeft -= timeDelta;
    if (lifeTimeLeft<0) dead = true;
}

void Particle::updatePhysics( double timeDelta ) {
    // Add external force
    Vec3 external_acceleration = external_force_accumulated*(1/mass)*timeDelta;
    external_force_accumulated = Vec3(0,0,0);
    velocity = (velocity+external_acceleration);

    // Add external impulse
    Vec3 external_impulse = external_impulse_accumulated*(1/mass);
    external_impulse_accumulated = Vec3(0,0,0);
    velocity = (velocity+external_impulse);

    // Update
    position = position+(velocity*timeDelta);
	
    if (position.y<Island::getInstance()->heightAt(position).y-0.1) {
        velocity = Vec3(randomValue*2-1,randomValue*2-1,randomValue*2-1)*3;
        position = Island::getInstance()->heightAt(position);
    }
}

void Particle::draw( ) {
    if (dead) return;

    forward = cameraDir;
    left = (Vec3(0,1,0).getCross(forward)).normalize().rotateAround(forward,2*PI*randomValue*3);
    up = (forward.getCross(left)).normalize();

    glPushMatrix(); // Head
    float matrix[] = {
        left.x    ,left.y    ,left.z    ,0,
        up.x      ,up.y      ,up.z      ,0,
        forward.x ,forward.y ,forward.z ,0,
        position.x,position.y,position.z,1};
    glMultMatrixf(matrix);
  
    double life = lifeTimeLeft/lifeTimeTotal;
    double life2 = pow(life,2.0);
  
    double scale = (0.4+0.6*(1-life))*size;
    glScalef( scale, scale, scale );
  
    glColor4f( 1.0*life2*2, 0.3*life2*2, 0.0*life2*2, life2 );
    glBindTexture(GL_TEXTURE_2D, textures[int(randomValue*3)] );

    glBegin( GL_QUADS );

    glTexCoord2f(1.0f, 1.0f);
    glNormal3f(  1,  1,  1 );
	glVertex3f(  1,  1,  0 );

    glTexCoord2f(1.0f, 0.0f);
    glNormal3f(  1, -1,  1 );
    glVertex3f(  1, -1,  0 );

    glTexCoord2f(0.0f, 0.0f);
    glNormal3f( -1, -1,  1 );
    glVertex3f( -1, -1,  0 );

    glTexCoord2f(0.0f, 1.0f);
    glNormal3f( -1,  1,  1 );
    glVertex3f( -1,  1,  0 );

    glEnd();
    glPopMatrix();
}
