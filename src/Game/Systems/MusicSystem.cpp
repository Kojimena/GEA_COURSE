#include "MusicSystem.h"

void MusicSystem::setup() {
    InitAudioDevice();

    backgroundSound = LoadSound("../src/assets/background_music.mp3");

    SetSoundVolume(backgroundSound, 0.1f);

    PlaySound(backgroundSound);
}

void MusicSystem::update() {
    if (!IsSoundPlaying(backgroundSound)) {
        PlaySound(backgroundSound);
    }
}

MusicSystem::~MusicSystem() {
    StopSound(backgroundSound);
    UnloadSound(backgroundSound);
    CloseAudioDevice();
}