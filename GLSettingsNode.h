#ifndef _GL_SETTING_NODE_
#define _GL_SETTING_NODE_

#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Renderers/IRenderNode.h>

using OpenEngine::Core::IListener;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Renderers::IRenderNode;

class GLSettingsNode : public IListener<ProcessEventArg>, public IRenderNode {
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
      if(done) return;

      if (timeSpend >= time) {
	  timeSpend = time;
	  done = true;
      }
      else
	  timeSpend += deltaTime;
  }
  
  void Apply(IRenderingView* rv) {
    if(!done) {
      double pctDone = timeSpend/time;
      float pFade = 1.4 * pctDone;
      glClearColor( 0.39*pFade, 0.45*pFade, 1.0*pFade, 1.0 );

    }

    /*
    glClearDepth(1.0f); // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
    glEnable( GL_DEPTH_TEST );
    */

    glShadeModel(GL_SMOOTH); // Enable Smooth Shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations

    
    glEnable(GL_NORMALIZE);
    //glEnable(GL_AUTO_NORMAL);

    glEnable(GL_COLOR_MATERIAL);
    VisitSubNodes(*rv);
  }
};

#endif
