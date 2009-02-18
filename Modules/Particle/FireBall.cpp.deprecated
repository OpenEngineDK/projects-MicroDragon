#include "FireBall.h"

#include "ParticleSystem.h"
#include "ParticleSystemEvents.h"

#include "../../Common/VectorExt.h"

#include "../Island/HeightMap.h"
#include "../Boid/BoidsSystem.h"

#include <Meta/OpenGL.h>
#include <Math/Math.h>
#include <Math/Matrix.h>
#include <Math/Quaternion.h>
#include <Display/IViewingVolume.h>

using OpenEngine::Math::PI;
using OpenEngine::Math::Matrix;
using OpenEngine::Math::Quaternion;

extern unsigned int textures[10];

FireBall::FireBall(HeightMap* heightMap, IViewingVolume* vv,
		   BoidsSystem* boidssystem, ParticleSystem* particlesystem,
		   Vector<3,float> position, Vector<3,float> velocity,
		   double size, double lifeTime, double randomValue) 
  : Particle(heightMap, vv, position, velocity, size, lifeTime, randomValue) {
    this->boidssystem = boidssystem;
    this->particlesystem = particlesystem;
}

FireBall::~FireBall() {
}

void FireBall::updatePhysics( double timeDelta ) {
    // Add external force
    Vector<3,float> external_acceleration = 
      external_force_accumulated*(1/mass)*timeDelta;

    external_force_accumulated = Vector<3,float>(0,0,0);
    velocity = (velocity+external_acceleration);

    // Add external impulse
    Vector<3,float> external_impulse = external_impulse_accumulated*(1/mass);
    external_impulse_accumulated = Vector<3,float>(0,0,0);
    velocity = (velocity+external_impulse);

    // Update
    position = position+(velocity*timeDelta);
	
    if (position[1]<heightMap->HeightAt(position)[1]+1.0 && dead != true) {
        position[1] = heightMap->HeightAt(position)[1]+1.0;
        dead = true;

	particlesystem->ParticleSystemEvent()
	  .Notify( ParticleSystemEventArg(EXPLOTION, position,15*size) );
        particlesystem->
	  CreateParticles(0.1, 0.0, 200*size,
			  position+Vector<3,float>(0,0.5*size-1,0),
			  Vector<3,float>(0,1.0*size,0), 2.0*size,
			  0.5*size, 2.0);
    }
}

void FireBall::draw() {
    if (dead) return;

    forward = vv->GetPosition(); // cpvc: inputgrabber->GetCameraDir();
  
    double life = lifeTimeLeft/lifeTimeTotal;
    //double life2 = pow(life,2.0);

    for (int i=0; i<5; i++) {
        float layer = (i+0.5f)/5.0f;
	glColor4f( (1.0+0.2*layer), (0.1+1.1*layer), (0.0+0.5*layer), life );
	
	Quaternion<float> direction = vv->GetDirection().GetInverse();
    
	direction *= Quaternion<float>(2*PI*(randomValue*3+layer),
				       Vector<3,float>(0.0,0.0,1.0));

	Matrix<4,4,float> m = direction.GetMatrix().GetExpanded();
	Matrix<4,4,float> t;
	// write the position information
	t(3,0) = position.Get(0);
	t(3,1) = position.Get(1);
	t(3,2) = position.Get(2);
	Matrix<4,4,float> matrix = m * t;
	
	float f[16] = {0};
	matrix.ToArray(f);

	glPushMatrix();	
	glMultMatrixf(f);

        double scale = (0.2+0.8*sqrt(1-life))*size*(0.2+0.8*(1-layer));
        glScalef( scale, scale, scale );
  
        glBindTexture(GL_TEXTURE_2D, textures[(int(randomValue*3)+i)%3] );
  
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
}
