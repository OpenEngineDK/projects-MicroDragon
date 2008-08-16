// TimerLayer
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _TIMER_LAYER_H_
#define _TIMER_LAYER_H_

#include "DragonText.h"
#include "../GameState.h"

#include <Display/Layer.h>
#include <Display/TextSurface.h>
#include <Scene/LayerNode.h>
#include <Core/IListener.h>
#include <Renderers/IRenderer.h>
#include <Display/IFrame.h>

using OpenEngine::Core::IListener;
using OpenEngine::Renderers::RenderingEventArg;
using OpenEngine::Display::TextSurface;
using OpenEngine::Display::Layer;
using OpenEngine::Scene::LayerNode;
using OpenEngine::Display::IFrame;

class DragonHUD : public IListener<RenderingEventArg> {
private:
    DragonText* time;
    DragonText* score;
    LayerNode* layerNode;
    int frameWidth, frameHeight;
    int testval;
    GameState& gamestate;
public:
    DragonHUD(IFrame& frame, GameState& gamestate);
    virtual ~DragonHUD();

    void Handle(RenderingEventArg arg);
    LayerNode* GetLayerNode();
};


#endif // _TIMER_LAYER_H_
