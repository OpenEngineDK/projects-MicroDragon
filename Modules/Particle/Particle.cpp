#include "Particle.h"

#include "../Island/HeightMap.h"
#include "../../Common/OpenGLUtil.h"
#include "../../Common/VectorExt.h"

#include <Math/Math.h>
#include <Math/Matrix.h>
#include <Math/Quaternion.h>
#include <Display/IViewingVolume.h>

using OpenEngine::Math::PI;
using OpenEngine::Math::Matrix;
using OpenEngine::Math::Quaternion;

extern unsigned int textures[10];

Particle::Particle(HeightMap* heightMap, IViewingVolume* vv, Vector<3,float> position, Vector<3,float> velocity, double size, double lifeTime, double randomValue) {
    this->heightMap = heightMap;
    this->vv = vv;
    this->position = position;
    if (!forward.IsZero())
      forward.Normalize();
    this->forward = forward;
    this->velocity = velocity;
    this->size = size;
    this->randomValue = randomValue;
    lifeTimeTotal = lifeTime;
    lifeTimeLeft = lifeTime;

    up = Vector<3,float>(0,1,0);
    left = up%(forward);
    mass = 1.0;
    max_force = 10.0;
    max_speed = 5.0;
    external_force_accumulated = Vector<3,float>(0,0,0);
    external_impulse_accumulated = Vector<3,float>(0,0,0);
    dead = false;
}

Particle::~Particle(){
}

Vector<3,float> Particle::getPosition() { 
  return position; 
}

Vector<3,float> Particle::getVelocity() { 
  return velocity; 
}

bool Particle::isDead() { 
  return dead;
}

void Particle::addExternalForce( Vector<3,float> force ) {
    external_force_accumulated = external_force_accumulated + force;
}

void Particle::addExternalImpulse( Vector<3,float> impulse ) {
    external_impulse_accumulated = external_impulse_accumulated + impulse;
}

void Particle::update( double timeDelta ) {
    updatePhysics(timeDelta);
    lifeTimeLeft -= timeDelta;
    if (lifeTimeLeft<0) dead = true;
}

void Particle::updatePhysics( double timeDelta ) {
    // Add external force
    Vector<3,float> external_acceleration = 
      external_force_accumulated*(1/mass)*timeDelta;
    velocity += external_acceleration;

    external_force_accumulated = Vector<3,float>(0,0,0);

    // Add external impulse
    Vector<3,float> external_impulse = external_impulse_accumulated*(1/mass);
    external_impulse_accumulated = Vector<3,float>(0,0,0);
    velocity += external_impulse;

    // Update
    position += velocity * timeDelta;
	
    if (position[1]<heightMap->HeightAt(position)[1]-0.1) {
        velocity = Vector<3,float>(randomValue*2-1,randomValue*2-1,
				   randomValue*2-1)*3;
        position = heightMap->HeightAt(position);
    }
}

bool Particle::IsCloserToCamera(Particle* p) {
  return ( this->DistanceToCamera() < p->DistanceToCamera() );
}

float Particle::DistanceToCamera() {
  return (vv->GetPosition() - this->position).GetLength();
}

void Particle::draw() {
    if (dead) return;

    glPushMatrix();

    Quaternion<float> direction = vv->GetDirection().GetInverse();
    Matrix<4,4,float> m = direction.GetMatrix().GetExpanded();
    Matrix<4,4,float> t;
    // write the position information
    t(3,0) = position.Get(0);
    t(3,1) = position.Get(1);
    t(3,2) = position.Get(2);
    Matrix<4,4,float> matrix = m * t;

    float f[16] = {0};
    matrix.ToArray(f);
    glMultMatrixf(f);
   
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
