#pragma once

#include "../FMOD/api/core/inc/fmod.hpp"
#include "common.h"
#include "../Common/Window.h"
#include "../Common/Camera.h"
#include "../CSC8503/GameWorld.h"
#include "../Common/Vector3.h"

#include <Vector>

class AudioManager {
    public:
        void InitSystem();
        void SetVolume();
        void AudioUpdate(NCL::CSC8503::GameWorld* world, float dt);
        void CacheRelease();

        void PlayJumpSound(FMOD_VECTOR _pos, FMOD_VECTOR _vel);
        void PlayHurtSound(FMOD_VECTOR _pos, FMOD_VECTOR _vel);
        void PlayPickupSound(FMOD_VECTOR _pos, FMOD_VECTOR _vel);
        void PlaySplashSound(FMOD_VECTOR _pos, FMOD_VECTOR _vel);
        void PlayFaintSound(FMOD_VECTOR _pos, FMOD_VECTOR _vel);

        void SetJump() { Jump = true; }

    private:
        const float     DISTANCEFACTOR = 1.0f;
        FMOD::System   *system;
        FMOD::Sound    *FaintSound, *FootStepSound, *HurtSound, *JumpSound, *PickUpSound, *SplashSound, *WaveSound;
        FMOD::Channel  *channel = 0;
        FMOD_RESULT     result;
        void           *extradriverdata = 0;
        FMOD_VECTOR     listenerpos  = { 0.0f, 0.0f, -1.0f * DISTANCEFACTOR };
        FMOD_VECTOR forward        = { 0.0f, 0.0f, 1.0f };
        float volume = 5.0f;

        bool Jump = false;
};