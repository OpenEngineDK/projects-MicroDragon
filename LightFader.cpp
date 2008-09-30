#include "LightFader.h"

#include <Logging/Logger.h>

LightFader::LightFader(LightNode* from, LightNode* to, LightNode* lightNode, float time) {

    this->from = from;
  this->to = to;//todo to.Clone();
  this->lightNode = lightNode;
  this->time = time;
  done = false;
  timeSpend = 0.0;

  timer.Start();
}

LightFader::~LightFader(){
  delete from;
  delete to;
}

void LightFader::Handle(ProcessEventArg arg) {
    unsigned int dt = timer.GetElapsedTimeAndReset().AsInt();
    float deltaTime = ((float)dt)/1000.0;
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
