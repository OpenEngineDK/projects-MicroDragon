// Play sounds by event triggering
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "Voice.h"

#include <Resources/ResourceManager.h>

using OpenEngine::Resources::ResourceManager;
using OpenEngine::Resources::ISoundResource;

Voice::Voice(ISoundSystem& soundsystem): soundsystem(soundsystem) {
    ISoundResourcePtr screamres;
    
    int i = int((rand()/(float)RAND_MAX)*5);
    string soundfile;
    switch(i) {
    case 0: soundfile = "SoundFX/vester-aargh.ogg"; break;
    case 1: soundfile = "SoundFX/cpvc-AARGH.ogg"; break;
    case 2: soundfile = "SoundFX/ian-aargh.ogg"; break;
    case 3: soundfile = "SoundFX/jakob-aargh.ogg"; break;
    case 4: soundfile = "SoundFX/salomon-aargh.ogg"; break;
    default:
        soundfile = "SoundFX/ptx-jargh.ogg"; break;
    }


    screamres = ResourceManager<ISoundResource>::Create(soundfile);
    screamsound = soundsystem.CreateMonoSound(screamres);
}

Voice::~Voice() {
    delete screamsound;
}

void Voice::SetPosition(Vector<3,float> pos) {
    screamsound->SetPosition(pos);
}

void Voice::Scream() {
    screamsound->Play();
}



