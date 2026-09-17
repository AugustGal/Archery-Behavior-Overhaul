#pragma once

namespace Tweaks
{
    class BowAttackSpeed
    {
    public:

        static void Install();

    private:

        static float GetArrowPower(float da_rawTime, float a_bowSpeed);

        static void ClipGeneratorUpdate(RE::hkbClipGenerator* a_clipGenerator, const RE::hkbContext& a_context, float a_timesteps);

        static float GetPlaybackSpeed(RE::Actor* a_actor, bool a_isQuickShot);

        static inline REL::Relocation<decltype(ClipGeneratorUpdate)> _ClipGeneratorUpdate;
    };
}