// Play sounds by event triggering
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "Voice.h"

#include <Math/RandomGenerator.h>
#include <Resources/ResourceManager.h>

using OpenEngine::Math::RandomGenerator;
using OpenEngine::Resources::ISoundResource;
using OpenEngine::Resources::ResourceManager;

Voice::Voice(ISoundSystem& soundsystem): soundsystem(soundsystem) {
    RandomGenerator* randGen = new RandomGenerator(); //@todo: move this
    randGen->SeedWithTime();

    string soundfile;
    switch(randGen->UniformInt(0,5)) {
    case 0: soundfile = "SoundFX/vester-aargh.ogg"; break;
    case 1: soundfile = "SoundFX/cpvc-AARGH.ogg"; break;
    case 2: soundfile = "SoundFX/ian-aargh.ogg"; break;
    case 3: soundfile = "SoundFX/jakob-aargh.ogg"; break;
    case 4: soundfile = "SoundFX/salomon-aargh.ogg"; break;
    case 5: soundfile = "SoundFX/ptx-jargh.ogg"; break;
    default:
        throw Exception("SoundFX index out of range");
    }

    ISoundResourcePtr screamres =
        ResourceManager<ISoundResource>::Create(soundfile);
    screamsound = (IMonoSound*)soundsystem.CreateSound(screamres);
    //@todo: remove the IMonoSound* cast above
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



