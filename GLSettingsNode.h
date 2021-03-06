#ifndef _GL_SETTING_NODE_
#define _GL_SETTING_NODE_

#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Scene/RenderNode.h>

using OpenEngine::Core::IListener;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Scene::RenderNode;

class GLSettingsNode : public IListener<ProcessEventArg>, public RenderNode {
private:
  float timeSpend,time;
  bool done;
  Timer timer;
public:
  GLSettingsNode(float time) : time(time) {
      timeSpend = 0.0;
      done = false;
      timer.Start();
  }
  
  void Handle(ProcessEventArg arg) {
      unsigned int dt = timer.GetElapsedTimeAndReset().AsInt();
      float deltaTime = ((float)dt)/1000.0;
      if (done) return;
      
      if (timeSpend >= time) {
          timeSpend = time;
          done = true;
      }
      else
          timeSpend += deltaTime;
  }
  
  void Apply(IRenderingView* rv) {
      if (!done) {
          double pctDone = timeSpend/time;
          float pFade = 1.4 * pctDone;
          Vector<4,float> color( 0.39*pFade, 0.45*pFade, 1.0*pFade, 1.0 );
          rv->GetRenderer()->SetBackgroundColor(color);
      }
      glEnable(GL_COLOR_MATERIAL);
      VisitSubNodes(*rv);
  }
};

#endif
