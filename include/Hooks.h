#pragma once

#include "Hooks.h"
#include "BowAttackSpeed.h"

    namespace ABO
    {
        struct GetArrowPowerHook
        {
            static void Install()
            {
                auto address = REL::ID(26435).address();
                SKSE::GetTrampoline().write_branch<5>(address, GetArrowPower);
            }

            static float GetArrowPower(float drawTime, float bowSpeed)
            {
                return Fixes::BowAttackSpeed::GetArrowPower(drawTime, bowSpeed);
            }
        };

        struct ClipGeneratorUpdateHook
        {
            static void Install()
            {
                REL::Relocation<uintptr_t> vtblhkbClipGenerator{ RE::VTABLE_hkbClipGenerator[0] };
                _ClipGeneratorUpdate = vtblhkbClipGenerator.write_vfunc(0x05, ClipGeneratorUpdate);
            }

            static void ClipGeneratorUpdate(RE::hkbClipGenerator* a_clipGenerator, const RE::hkbContext& a_context, float a_timesteps)
            {
                if (a_context.character)
                {
                    if (a_clipGenerator)
                    {
                        Fixes::BowAttackSpeed::ClipGeneratorUpdate(a_clipGenerator, a_context, a_timesteps);
                    }
                }

                return _ClipGeneratorUpdate(a_clipGenerator, a_context, a_timesteps);
            }

            static inline REL::Relocation<decltype(ClipGeneratorUpdate)> _ClipGeneratorUpdate;
        };

        struct Hooks
        {
            static void Install()
            {
                GetArrowPowerHook::Install();
                ClipGeneratorUpdateHook::Install();
            }

        };
}