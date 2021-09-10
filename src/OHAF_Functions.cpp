#include "OHAF_Functions.h"
#include "OHAF_DataHandler.h"

namespace MaxsuOnHitAnimFW
{
	bool ShouldProcessOnHit(RE::Actor* a_target)
	{
		if (!a_target) {
			logger::error("Target Actor Not Found!");
			return false;
		}

		auto OHAF_datahandler = DataHandler::GetSingleton();

		if (!a_target->Is3DLoaded()) {
			logger::debug("Target Actor Not 3D Loaded!");
			return false;
		}

		logger::debug(FMT_STRING("Target Actor Name is \"{}\", ID is \"{:x}\""), a_target->GetName(), a_target->GetFormID());

		if (a_target->formType != RE::FormType::ActorCharacter) {
			logger::debug("Target Actor Not Actor!");
			return false;
		}

		if (a_target->IsDead()) {
			logger::debug("Target Actor is Dead!");
			return false;
		}

		if (a_target->IsGhost() || a_target->HasEffectWithArchetype(RE::EffectArchetypes::ArchetypeID::kEtherealize)) {
			logger::debug("Target Actor Is Ghost or Etherealize!");
			return false;
		}

		if (a_target->IsInKillMove()) {
			logger::debug("Target Actor Is in Kill Move!");
			return false;
		}

		if (a_target->IsPlayerRef()) {
			if (!OHAF_datahandler->IsEnablePC()) {
				logger::debug("Disable Player Character!");
				return false;
			} else if (RE::PlayerCamera::GetSingleton()->currentState->id != RE::CameraState::kThirdPerson) {
				logger::debug("Player Character Not in Third Person!");
				return false;
			}
		} else if (!a_target->IsPlayerRef() && !OHAF_datahandler->IsEnableNPC()) {
			logger::debug("Disable NPC!");
			return false;
		}

		return true;
	}



	RE::EffectSetting* GetEffectSettingFromList(RE::BSSimpleList<RE::ActiveEffect*>* thisList, std::uint16_t thisID)
	{
		if (!thisList)
		{
			logger::error("Effect List Is Null!");
			return nullptr;
		}
		
		for (auto this_ActEffect : (*thisList))
		{
			if (this_ActEffect->usUniqueID == thisID)
			{
				return this_ActEffect->GetBaseObject();
			}
		}

		return nullptr;
	}


	bool HasEffectSettingInList(RE::BSSimpleList<RE::ActiveEffect*>* thisList, RE::EffectSetting* this_effect)
	{
		if (!thisList)
		{
			logger::error("Effect List Is Null!");
			return false;
		}

		for (auto this_ActEffect : (*thisList))
		{
			if (this_ActEffect->GetBaseObject() == this_effect)
			{
				return true;
			}
		}

		return false;
	}


}