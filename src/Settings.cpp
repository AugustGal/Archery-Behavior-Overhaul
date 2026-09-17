#include "Settings.h"
#include <SimpleIni.h>

Settings* Settings::GetSingleton()
{
	static Settings settings;
	return &settings;
}

void Settings::LoadSettings()
{
    logger::info("Loading settings");

        CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\ArcheryBehaviorOverhaul.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");

    if (debug_logging) 
    {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }

    bowAttackSpeed = ini.GetBoolValue("Tweaks", "bowAttackSpeed", true);
    bowQuickShotBonusSpeed = std::stof(ini.GetValue("Tweaks", "bowQuickShotBonusSpeed", "1.0"));

    logger::info("Loaded settings");
}