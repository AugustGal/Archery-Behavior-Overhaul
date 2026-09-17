#pragma once

class Settings
{
	public:

		static Settings* GetSingleton();

		void LoadSettings();

		inline static bool debug_logging{};

		bool bowAttackSpeed;
		float bowQuickShotBonusSpeed;
};



