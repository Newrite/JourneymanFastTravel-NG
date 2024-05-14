#pragma once

#include "FastTravelManager.h"

namespace Hooks
{
	inline static REL::Relocation<std::uintptr_t> Can_Fast_Travel_Map_Hook{ REL::RelocationID(52208, 53095), REL::Relocate(0x2C5, 0x328) };
	inline static REL::Relocation<std::uintptr_t> Papyrus_Fast_Travel_Hook{ REL::RelocationID(54824, 55457), REL::Relocate(0xA1, 0xA1) };
	
	inline bool Install() {
		if (!FastTravelManager::InstallCanFastTravelCheck()) {
			return false;
		}
		if (!FastTravelManager::InstallPapyrusFastTravelCheck()) {
			return false;
		}
		return true;
	}
}