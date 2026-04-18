#include "SmartpointerHelpers.hpp"


void Game::MIX_MixerDeletor::operator() (MIX_Mixer* mixer) const
{
    if (mixer)
    {
        MIX_DestroyMixer(mixer);
    }
}

void Game::MIX_AudioDeletor::operator()(MIX_Audio* audio) const
{
    if (audio)
    {
        MIX_DestroyAudio(audio);
    }
}

void Game::MIX_TrackDeletor::operator()(MIX_Track* track) const
{
    if (track)
    {
        MIX_DestroyTrack(track);
    }
}
