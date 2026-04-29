#pragma once

#include "Commands/Command.h"
#include "Core/ServiceLocator.h"

#include "../Base/SoundEvents.h"


namespace Game::Sounds
{
    enum class TestSoundEvents
    {
        Stop,
        Resume,
        Pause,
        Mute,
        UnMute,
        SetVolume,
        TogglePause,
        ToggleMute,
        StopAll,
        ResumeAll,
        PauseAll,
        MuteAll,
        UnMuteAll,
        SetVolumeAll,
        TogglePauseAll,
        ToggleMuteAll,
    };


    class TestSoundCommand : public bae::Command
    {
    public:
        explicit TestSoundCommand(const TestSoundEvents soundEvent,
                                  const bae::ActiveSoundID activeSoundId = bae::ActiveSoundID(-1),
                                  const float volume                     = 1.f) :
            Command(),
            m_TestEvent{ soundEvent },
            m_ActiveSoundId{ activeSoundId },
            m_Volume{ volume }
        {
        };

        ~TestSoundCommand() override = default;

        TestSoundCommand(const TestSoundCommand& other)            = delete;
        TestSoundCommand(TestSoundCommand&& other)                 = delete;
        TestSoundCommand& operator=(const TestSoundCommand& other) = delete;
        TestSoundCommand& operator=(TestSoundCommand&& other)      = delete;


        void Execute() override
        {
            auto& soundSystem = bae::ServiceLocator::GetSoundSystem();

            switch(m_TestEvent)
            {
                case TestSoundEvents::Stop:
                    soundSystem.Stop(m_ActiveSoundId);
                    break;
                case TestSoundEvents::Resume:
                    soundSystem.Resume(m_ActiveSoundId);
                    break;
                case TestSoundEvents::Pause:
                    soundSystem.Pause(m_ActiveSoundId);
                    break;
                case TestSoundEvents::Mute:
                    soundSystem.Mute(m_ActiveSoundId);
                    break;
                case TestSoundEvents::UnMute:
                    soundSystem.UnMute(m_ActiveSoundId);
                    break;
                case TestSoundEvents::SetVolume:
                    soundSystem.SetVolume(m_ActiveSoundId, m_Volume);
                    break;
                case TestSoundEvents::TogglePause:
                    if(soundSystem.IsPaused(m_ActiveSoundId))
                        soundSystem.Resume(m_ActiveSoundId);
                    else
                        soundSystem.Pause(m_ActiveSoundId);
                    break;
                case TestSoundEvents::ToggleMute:
                    if(soundSystem.IsMuted(m_ActiveSoundId))
                        soundSystem.UnMute(m_ActiveSoundId);
                    else
                        soundSystem.Mute(m_ActiveSoundId);
                    break;

                case TestSoundEvents::StopAll:
                    soundSystem.StopAllSounds();
                    break;
                case TestSoundEvents::ResumeAll:
                    soundSystem.ResumeAllSounds();
                    break;
                case TestSoundEvents::PauseAll:
                    soundSystem.PauseAllSounds();
                    break;
                case TestSoundEvents::MuteAll:
                    soundSystem.MuteAllSounds();
                    break;
                case TestSoundEvents::UnMuteAll:
                    soundSystem.UnMuteAllSounds();
                    break;
                case TestSoundEvents::SetVolumeAll:
                    soundSystem.SetVolumeAllSounds(m_Volume);
                    break;
                case TestSoundEvents::TogglePauseAll:
                    if(m_bAreAllSoundsPaused)
                    {
                        soundSystem.ResumeAllSounds();
                    }
                    else
                    {
                        soundSystem.PauseAllSounds();
                    }

                    m_bAreAllSoundsPaused = !m_bAreAllSoundsPaused;
                    break;
                case TestSoundEvents::ToggleMuteAll:
                    if(m_bAreAllSoundsMuted)
                    {
                        soundSystem.UnMuteAllSounds();
                    }
                    else
                    {
                        soundSystem.MuteAllSounds();
                    }

                    m_bAreAllSoundsMuted = !m_bAreAllSoundsMuted;
                    break;
            }
        };

    protected:
        bae::ActiveSoundID m_ActiveSoundId;
        TestSoundEvents m_TestEvent;

        float m_Volume{ 1.f };

        bool m_bAreAllSoundsPaused{ false };
        bool m_bAreAllSoundsMuted{ false };
    };

    class TestPlaySoundCommand : public bae::Command
    {
    public:
        explicit TestPlaySoundCommand(const bae::SoundID soundId, const float volume = 1.f) :
            Command(),
            m_SoundId{ soundId },
            m_Volume{ volume }
        {
        };

        ~TestPlaySoundCommand() override = default;

        TestPlaySoundCommand(const TestPlaySoundCommand& other)            = delete;
        TestPlaySoundCommand(TestPlaySoundCommand&& other)                 = delete;
        TestPlaySoundCommand& operator=(const TestPlaySoundCommand& other) = delete;
        TestPlaySoundCommand& operator=(TestPlaySoundCommand&& other)      = delete;


        void Execute() override
        {
            auto& soundSystem = bae::ServiceLocator::GetSoundSystem();
            soundSystem.Play(m_SoundId, m_Volume);
        };

    protected:
        bae::SoundID m_SoundId;
        float m_Volume;
    };
}


