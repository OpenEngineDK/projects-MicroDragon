#include "Target.h"

#include "../../Common/OpenGLUtil.h"
#include "../Island/Island.h"
#include "../Dragon/Dragon.h"

#include <Meta/OpenGL.h>
#include <Logging/Logger.h>

using namespace std;

Target* Target::instance = NULL;

Target* Target::getInstance()
{
    if( instance == NULL )
        instance = new Target();
    return instance;
}

Target::Target() {
    target = Vec3(0,0,0);
}

Target::~Target(){
}

void Target::Initialize(){
}

void Target::Deinitialize() {
}

void Target::Process(const float deltaTime, const float percent) {
    target = Island::getInstance()->heightAt(target);
    target.y = max(target.y+1.0f,1.0f);
}

bool Target::IsTypeOf(const std::type_info& inf) {
    return (typeid(Target) == inf);
}

void Target::Apply(IRenderingView* rv) {
    // Draw target
    glPushMatrix();
    glTranslatef( target.x, target.y, target.z);
    if (Dragon::getInstance()->isUsingBreathWeapon()) {
        glColor3f( 0.8, 0.0, 0.0 );
        OpenGLUtil::GLSolidCube( 0.5 );
    }
    else {
        glColor3f( 0.0, 0.8, 0.0 );
        OpenGLUtil::GLSolidCube( 0.4 );
    }
    glPopMatrix();
}

Vec3 Target::getTarget(){
    return target;
}

void Target::setTarget(float x, float y, float z){
    target = Vec3(x,y,z);
}

void Target::setTarget( Vec3 v ){
    target = v;
}

void Target::printTarget() {
    logger.info << "Target x:" << target.x << " y:" << target.y << " z:" << target.z << logger.end;
}
