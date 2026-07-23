//
// Created by abc17 on 2026/7/21.
//

#ifndef GAMEENGINE_AUDIOPLAYER_H
#define GAMEENGINE_AUDIOPLAYER_H

#include "SDL3_mixer/SDL_mixer.h"
static MIX_Mixer *mixer;
static MIX_Track *track;
static MIX_Audio *audio;

class AudioPlayer {
private:
    AudioPlayer()=default;
public:
    static AudioPlayer& getInstance() {
        static AudioPlayer audio_player;
        return audio_player;
    }
    static void init() {

        /* this doesn't have to run very much, so give up tons of CPU time between iterations. */
        SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "5");

        if (!MIX_Init()) {
            SDL_Log("Couldn't initialize SDL_mixer: %s", SDL_GetError());
        }

        mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
        if (!mixer) {
            SDL_Log("Couldn't create mixer: %s", SDL_GetError());
        }


    }
    static void loadAndPlay(const char* file) {
        audio = MIX_LoadAudio(mixer, file, true);
        if (!audio) {
            SDL_Log("Couldn't load audio from %s: %s", file, SDL_GetError());
        }

        track = MIX_CreateTrack(mixer);
        if (!track) {
            SDL_Log("Couldn't create track: %s", SDL_GetError());
        }
        MIX_SetTrackAudio(track, audio);
        MIX_PlayTrack(track, 0);
    }
};


#endif //GAMEENGINE_AUDIOPLAYER_H
