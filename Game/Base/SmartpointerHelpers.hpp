#pragma once

#include <mutex>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

namespace Game
{
    struct SDL_AudioStreamDeletor
    {
        void operator() (SDL_AudioStream* stream) const;
    };

    struct MIX_MixerDeletor
    {
        void operator() (MIX_Mixer* mixer) const;
    };

    struct MIX_AudioDeletor
    {
        void operator() (MIX_Audio* audio) const;
    };
    struct MIX_TrackDeletor
    {
        void operator() (MIX_Track* track) const;
    };


}