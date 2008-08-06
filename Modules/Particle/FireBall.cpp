#include "FireBall.h"
#include "../Island/Island.h"
#include "../../Common/OpenGLUtil.h"
#include "../Boid/BoidsSystem.h"
#include "ParticleSystem.h"

#include <Math/Math.h>

using OpenEngine::Math::PI;

extern Vec3 cameraDir;
extern unsigned int textures[10];

FireBall::FireBall(Vec3 position, Vec3 velocity, double size, double lifeTime, double randomValue) 
    : Particle(position, velocity, size, lifeTime, randomValue) {

}

FireBall::~FireBall() {
}

void FireBall::updatePhysics( double timeDelta ) {
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
	
    if (position.y<Island::getInstance()->heightAt(position).y+1.0 && dead != true) {
        position.y = Island::getInstance()->heightAt(position).y+1.0;
        dead = true;
        BoidsSystem::getInstance()->HandleFireball(position,15*size);
        ParticleSystem::getInstance()->CreateParticles(
                                                       0.1, 0.0, 200*size,
                                                       position+Vec3(0,0.5*size-1,0), Vec3(0,1.0*size,0), 2.0*size,
                                                       0.5*size, 2.0);
    }
}

void FireBall::draw() {
    if (dead) return;

    forward = cameraDir;
  
    double life = lifeTimeLeft/lifeTimeTotal;
    double life2 = pow(life,2.0);

    for (int i=0; i<5; i++) {
        float layer = (i+0.5f)/5.0f;
    
        left = (Vec3(0,1,0).getCross(forward)).normalize().rotateAround(forward,2*PI*(randomValue*3+layer));
        up = (forward.getCross(left)).normalize();
        Vec3 pos = position+forward*i*0.1;
        
        double scale = (0.2+0.8*sqrt(1-life))*size*(0.2+0.8*(1-layer));
        glColor4f( (1.0+0.2*layer), (0.1+1.1*layer), (0.0+0.5*layer), life );
    
        glPushMatrix();
        float matrix[] = {
            left.x    ,left.y    ,left.z    ,0,
            up.x      ,up.y      ,up.z      ,0,
            forward.x ,forward.y ,forward.z ,0,
            pos.x     ,pos.y     ,pos.z     ,1};
        glMultMatrixf(matrix);
    
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
