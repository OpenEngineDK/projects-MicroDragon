#ifndef _TRANSPARENTCY_NODE_
#define _TRANSPARENTCY_NODE_

#include <Renderers/IRenderingView.h>
#include <Meta/OpenGL.h>

using OpenEngine::Renderers::IRenderingView;

class TransparencyNode : public RenderNode {
public:
  void Apply(IRenderingView* rv) {
      //glEnable(GL_TEXTURE_2D);
      glEnable(GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      //glDepthMask(GL_FALSE);
      //glDisable(GL_LIGHTING);

      VisitSubNodes(*rv);

      //glEnable(GL_LIGHTING);
      glDisable(GL_BLEND);
      //glDepthMask(GL_TRUE);
  }
};

#endif
