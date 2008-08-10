#include "Target.h"

#include "../../Common/OpenGLUtil.h"
#include "../Island/Island.h"

#include <Meta/OpenGL.h>
#include <Logging/Logger.h>

using namespace std;

Target::Target(Island* island) : island(island) {
    target = Vector<3,float>(0,0,0);
    active = false;
}

Target::~Target(){
}

void Target::Initialize(){
}

void Target::Deinitialize() {
}

void Target::Process(const float deltaTime, const float percent) {
  target = island->heightAt(target);
    target[1] = max(target[1]+1.0f,1.0f);
}

bool Target::IsTypeOf(const std::type_info& inf) {
    return (typeid(Target) == inf);
}

void Target::Apply(IRenderingView* rv) {
    // Draw target
    glPushMatrix();
    glTranslatef( target[0], target[1], target[2]);
    if (active) {
        glColor3f( 0.8, 0.0, 0.0 );
        OpenGLUtil::GLSolidCube( 0.5 );
    }
    else {
        glColor3f( 0.0, 0.8, 0.0 );
        OpenGLUtil::GLSolidCube( 0.4 );
    }
    glPopMatrix();
}

void Target::SetActive(bool active) {
    this->active = active;
}

Vector<3,float> Target::getTarget(){
    return target;
}

void Target::setTarget(float x, float y, float z){
    setTarget(Vector<3,float>(x,y,z));
}

void Target::setTarget( Vector<3,float> v ){
    target = v;
}

void Target::printTarget() {
  logger.info << target << logger.end;
}
