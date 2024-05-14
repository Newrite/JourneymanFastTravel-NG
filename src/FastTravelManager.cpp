#include "FastTravelManager.h"
#include "Hooks.h"
#include "Settings.h"

bool FastTravelManager::InstallCanFastTravelCheck()
{
  auto& trampoline = SKSE::GetTrampoline();
  _CanFastTravelMap = trampoline.write_call<5>(Hooks::Can_Fast_Travel_Map_Hook.address(), CanFastTravelMap);
  logger::info("Installed ft hook");
  return true;
}

bool FastTravelManager::InstallPapyrusFastTravelCheck()
{
  auto& trampoline = SKSE::GetTrampoline();
  _GetWorldSpaceHook = trampoline.write_call<5>(Hooks::Papyrus_Fast_Travel_Hook.address(), GetWorldSpaceHook);
  logger::info("Installed papyrus ft hook");
  return true;
}

RE::TESWorldSpace* FastTravelManager::GetWorldSpaceHook(RE::TESObjectREFR* a1)
{
  if (auto settings = Settings::GetSingleton()) {
    settings->menuFastTravel = false;
  }
  // logger::info("GetWorldSpaceHook call");

  return _GetWorldSpaceHook(a1);
}

bool FastTravelManager::CanFastTravelMap(RE::Actor* a_actor, bool a_bool)
{
  auto inv = a_actor->GetInventory();
  auto settings = Settings::GetSingleton();
  auto player = RE::PlayerCharacter::GetSingleton();
  // logger::info("CanFastTravelMap call");

  if ((!settings->EnableOnlyOnSM || settings->Survival_ModeEnabledShared->value == 1.0f) && !player->IsGodMode()) {
    // logger::info("CanFastTravelMap call enter condition");

    if (IsOnFlyingMount(a_actor)) {
      // logger::info("CanFastTravelMap call actor fly");
      return true;
    }

    for (const auto& [item, data] : inv) {
      // logger::info("CanFastTravelMap call cycle start");
      if (settings->RequiredItems->HasForm(item->GetFormID())) {
        // logger::info("CanFastTravelMap call cycle HasForm true");

        if (_CanFastTravelMap(a_actor, a_bool)) {
          // logger::info("CanFastTravelMap call can fast travel have travel pack");
          settings->menuFastTravel = true;
          return true;
        }
        // logger::info("CanFastTravelMap cycle HasForm true but not allowed");
        return false;
      }
    }

    // logger::info("CanFastTravelMap call end of cycle");

    // If you get to here you didnt have the required item/s and you can't travel
    RE::DebugNotification(settings->RequiredItemNotFoundMessage.c_str());
    return false;
  }

  // logger::info("CanFastTravelMap call finish");
  return IsOnFlyingMount(a_actor) || _CanFastTravelMap(a_actor, a_bool);
  // return _CanFastTravelMap(a_actor, a_bool);
}
