#include "AudioManager.h"
#include <iostream>

void AudioManager::InitSystem() {
    //Common_Init(&extradriverdata);

    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
    ERRCHECK(result);

    result = system->set3DSettings(1.0, DISTANCEFACTOR, 0.1f);
    ERRCHECK(result);

    result = system->createSound(Common_MediaPath("faint.mp3"), FMOD_3D, 0, &FaintSound);
    ERRCHECK(result);

    result = system->createSound(Common_MediaPath("footsteps.wav"), FMOD_3D, 0, &FootStepSound);
    ERRCHECK(result);

    result = system->createSound(Common_MediaPath("hurt.wav"), FMOD_3D, 0, &HurtSound);
    ERRCHECK(result);

    result = system->createSound(Common_MediaPath("jump.wav"), FMOD_3D, 0, &JumpSound);
    ERRCHECK(result);

    result = system->createSound(Common_MediaPath("Pickup.wav"), FMOD_3D, 0, &PickUpSound);
    ERRCHECK(result);

    result = system->createSound(Common_MediaPath("splash.wav"), FMOD_3D, 0, &SplashSound);
    ERRCHECK(result);

    result = system->createSound(Common_MediaPath("wave.mp3"), FMOD_3D, 0, &WaveSound);
    ERRCHECK(result);

    result = FaintSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
    ERRCHECK(result);

    result = FootStepSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
    ERRCHECK(result);

    result = HurtSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
    ERRCHECK(result);

    result = JumpSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
    ERRCHECK(result);

    result = PickUpSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
    ERRCHECK(result);

    result = SplashSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
    ERRCHECK(result);

    result = WaveSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
    ERRCHECK(result);
}

void AudioManager::SetVolume() {
    std::cout << "Please Enter Preffered Output Volume (Current Value: " << volume << " ): ";
    float _volume;
    std::cin >> _volume;
    volume = _volume;   
    result = channel->setVolume(volume);
    ERRCHECK(result);
}

void AudioManager::AudioUpdate(NCL::CSC8503::GameWorld* world, float dt) {
    //Common_Update();

    NCL::Vector3 cameraPos = world->GetMainCamera(0)->GetPosition();

    FMOD_VECTOR pos = { 0.0f, 25.0f, -200.0f };
    FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
    
    if (NCL::Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::P)) {
        result = system->playSound(WaveSound, 0, false, &channel);
        ERRCHECK(result);
        result = channel->setVolume(volume);
        ERRCHECK(result);
        result = channel->set3DAttributes(&pos, &vel);
        ERRCHECK(result);
    }

    if (Jump == true) {
        pos = {cameraPos.x, cameraPos.y, cameraPos.z};
        PlayJumpSound(pos, vel);
        Jump = false;
    }

    if (NCL::Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::O)) {
        SetVolume();
    }

    {       
        static FMOD_VECTOR lastpos = { 0.0f, 0.0f, 0.0f };
        FMOD_VECTOR up             = { 0.0f, 1.0f, 0.0f };
        FMOD_VECTOR vel;
        
        float yaw = world->GetMainCamera(0)->GetYaw();

        float radians = yaw * ( 3.14159265 / 180 );

        forward.x = sin(radians);
        forward.y = 0.0f;
        forward.z = cos(radians);

        listenerpos = {cameraPos.x, cameraPos.y, cameraPos.z};

        vel.x = (listenerpos.x - lastpos.x) * dt;
        vel.y = (listenerpos.y - lastpos.y) * dt;
        vel.z = (listenerpos.z - lastpos.z) * dt;

        lastpos = listenerpos;

        result = system->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);
        ERRCHECK(result);
    }

    result = system->update();
    ERRCHECK(result);
}

void AudioManager::CacheRelease() {
    result = FaintSound->release();
    ERRCHECK(result);

    result = FootStepSound->release();
    ERRCHECK(result);

    result = HurtSound->release();
    ERRCHECK(result);

    result = JumpSound->release();
    ERRCHECK(result);

    result = PickUpSound->release();
    ERRCHECK(result);

    result = SplashSound->release();
    ERRCHECK(result);

    result = WaveSound->release();
    ERRCHECK(result);

    //Common_Close();
}

void AudioManager::PlayJumpSound(FMOD_VECTOR _pos, FMOD_VECTOR _vel) {
    result = system->playSound(JumpSound, 0, false, &channel);
    ERRCHECK(result);
    result = channel->setVolume(volume);
    ERRCHECK(result);
    result = channel->set3DAttributes(&_pos, &_vel);
    ERRCHECK(result);
}

void AudioManager::PlayHurtSound(FMOD_VECTOR _pos, FMOD_VECTOR _vel) {
    result = system->playSound(HurtSound, 0, false, &channel);
    ERRCHECK(result);
    result = channel->setVolume(volume);
    ERRCHECK(result);
    result = channel->set3DAttributes(&_pos, &_vel);
    ERRCHECK(result);
}

void AudioManager::PlayPickupSound(FMOD_VECTOR _pos, FMOD_VECTOR _vel) {
    result = system->playSound(PickUpSound, 0, false, &channel);
    ERRCHECK(result);
    result = channel->setVolume(volume);
    ERRCHECK(result);
    result = channel->set3DAttributes(&_pos, &_vel);
    ERRCHECK(result);
}

void AudioManager::PlaySplashSound(FMOD_VECTOR _pos, FMOD_VECTOR _vel) {
    result = system->playSound(SplashSound, 0, false, &channel);
    ERRCHECK(result);
    result = channel->setVolume(volume);
    ERRCHECK(result);
    result = channel->set3DAttributes(&_pos, &_vel);
    ERRCHECK(result);
}

void AudioManager::PlayFaintSound(FMOD_VECTOR _pos, FMOD_VECTOR _vel) {
    result = system->playSound(FaintSound, 0, false, &channel);
    ERRCHECK(result);
    result = channel->setVolume(volume);
    ERRCHECK(result);
    result = channel->set3DAttributes(&_pos, &_vel);
    ERRCHECK(result);
}