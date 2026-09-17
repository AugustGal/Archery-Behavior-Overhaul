#include "BowAttackSpeed.h"
#include "Settings.h"
#include "Utility.h"

namespace Tweaks
{
    void BowAttackSpeed::Install()
    {
        uintptr_t address = REL::ID(26435).address();
        SKSE::GetTrampoline().write_branch<5>(address, GetArrowPower);

        REL::Relocation<uintptr_t> vtblhkbClipGenerator{ RE::VTABLE_hkbClipGenerator[0] };
        _ClipGeneratorUpdate = vtblhkbClipGenerator.write_vfunc(0x05, ClipGeneratorUpdate);
    }

    float BowAttackSpeed::GetArrowPower(float a_drawTime, float a_bowSpeed)
    {
        if (a_bowSpeed <= 0.0F)
        {
            a_bowSpeed = 1.0F;
        }

        static RE::Setting* const arrowBowMinTime = RE::GameSettingCollection::GetSingleton()->GetSetting("fArrowBowMinTime");
        static RE::Setting* const bowDrawTime = RE::GameSettingCollection::GetSingleton()->GetSetting("fBowDrawTime");
        static RE::Setting* const arrowMinPower = RE::GameSettingCollection::GetSingleton()->GetSetting("fArrowMinPower");

        const float baseNockTime = arrowBowMinTime->GetFloat();
        const float baseDrawTime = bowDrawTime->GetFloat();
        const float minArrowPower = arrowMinPower->GetFloat();

        auto* const player = RE::PlayerCharacter::GetSingleton();

        bool isQuickShot = false;
        player->GetGraphVariableBool("bPerkQuickDraw"sv, isQuickShot);

        float nockTime = baseNockTime / GetPlaybackSpeed(player, isQuickShot);

        const float pullTime = a_drawTime - nockTime;

        if (pullTime <= 0.0F) 
        {
            return minArrowPower;
        }

        const float maxPullTime = (baseDrawTime - baseNockTime) / a_bowSpeed;

        if (pullTime >= maxPullTime) 
        {
            return 1.0F;
        }

        return minArrowPower + ((pullTime / maxPullTime) * (1.0F - minArrowPower));
    }

    void BowAttackSpeed::ClipGeneratorUpdate(RE::hkbClipGenerator* a_clipGenerator, const RE::hkbContext& a_context, float a_timesteps)
    {
        if (a_context.character && a_clipGenerator) 
        {
            if (auto* graph = GetGraphFromCharacter(a_context.character)) 
            {
                if (auto* actor = graph->holder) 
                {
                    int rightHandType = 0;
                    graph->GetGraphVariableInt("iRightHandType"sv, rightHandType);

                    if (rightHandType == 7) 
                    {
                        switch (hash(graph->projectName.data(), graph->projectName.length()))
                        {
                        case "DefaultMale"_h:
                        case "DefaultFemale"_h:
                        case "FirstPerson"_h:
                        {
                            bool isQuickShot = false;

                            switch (hash(a_clipGenerator->name.data(), a_clipGenerator->name.length()))
                            {
                            case "Bow_DrawNock"_h:
                            case "SneakBow_DrawNock"_h:
                            case "Bow_Release"_h:
                            case "SneakBow_Release"_h:
                                break;

                            case "Bow_DrawNock_QuickShot"_h:
                            case "SneakBow_DrawNock_QuickShot"_h:
                                isQuickShot = true;
                                break;

                            default:
                                return _ClipGeneratorUpdate(a_clipGenerator, a_context, a_timesteps);
                            }

                            a_clipGenerator->playbackSpeed = GetPlaybackSpeed(actor, isQuickShot);
                        }
                        break;

                        }
                    }
                }
            }
        }

        _ClipGeneratorUpdate(a_clipGenerator, a_context, a_timesteps);
    }

    float BowAttackSpeed::GetPlaybackSpeed(RE::Actor* a_actor, bool a_isQuickShot)
    {
        static const float quickShotBonusSpeed = Settings::GetSingleton()->bowQuickShotBonusSpeed;
        float weaponSpeedMult = a_actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kWeaponSpeedMult);

        return a_isQuickShot ? weaponSpeedMult + quickShotBonusSpeed : weaponSpeedMult;
    }
}