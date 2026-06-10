#pragma once


namespace Game
{
    class GhostKiller
    {
    public:
        GhostKiller()          = default;
        virtual ~GhostKiller() = default;
        virtual void Kill() = 0;
    };
}
