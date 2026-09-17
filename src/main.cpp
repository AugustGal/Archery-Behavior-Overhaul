#include "BowAttackSpeed.h"
#include "Settings.h"

void InitListener(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type)
	{
	case SKSE::MessagingInterface::kDataLoaded:

        if (Settings::GetSingleton()->bowAttackSpeed)
        {
            Tweaks::BowAttackSpeed::Install();

            logger::info("Bow Attack Speed tweak loaded");
        }
        else
        {
            logger::info("Bow Attack Speed tweak not loaded");
        }

		break;
	}
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    REL::Module::reset();

    Init(skse);

    const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto name{ plugin->GetName() };
    const auto version{ plugin->GetVersion() };

    logger::init();

    logger::info("{} {} is loading...", name, version);

    SKSE::AllocTrampoline(14);

    if (const auto messaging{ SKSE::GetMessagingInterface() }; !messaging->RegisterListener(InitListener)) 
    {
        return false;
    }

    Settings::GetSingleton()->LoadSettings();

    logger::info("{} has finished loading.", name);
    logger::info("");

    return true;
}