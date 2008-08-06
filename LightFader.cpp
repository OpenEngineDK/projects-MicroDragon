#include "LightFader.h"

#include <Logging/Logger.h>

LightFader::LightFader(LightNode& from, LightNode& to, LightNode* lightNode, float time) {

  this->from = new LightNode(from);
  this->to = new LightNode(to);//todo to.Clone();
  this->lightNode = lightNode;
  this->time = time;
  done = false;
  timeSpend = 0.0;
}

LightFader::~LightFader(){
  delete from;
  delete to;
}


void LightFader::Initialize() {
}

void LightFader::Deinitialize() {
}

void LightFader::Process(const float deltaTime, const float percent) {
  if(done) return;

  if (timeSpend >= time) {
      timeSpend = time;
      done = true;
  }
  else
      timeSpend += deltaTime;

  double pctDone = timeSpend/time;

  lightNode->ambient =
    from->ambient + (to->ambient - from->ambient) * pctDone;
  lightNode->diffuse = 
    from->diffuse + (to->diffuse - from->diffuse) * pctDone;
  lightNode->specular =
    from->specular + (to->specular - from->specular) * pctDone;
}

bool LightFader::IsTypeOf(const std::type_info& inf) {
    return (typeid(LightFader) == inf);
}
