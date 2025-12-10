#include "ModuleManager.hpp"
#include "HUD.hpp"
#include "util/StringH.hpp"
#include "HotKeys.h"
#include "RemoveCall.h"


void KeyBind::process() const{

	if (key == ImGuiKey_None)
		return;

	if (ImGui::IsKeyReleased((ImGuiKey)key)) {
		if (func)
			*func = !*func;
	}
}

CheatFunction::CheatFunction(bool* enable , std::string name) :enable(enable) , name(name) {
	keybind = new KeyBind(ImGuiKey_None , enable);
}



ModuleManager::ModuleManager() {
	modules.push_back(&HUD::i());
	

	cheatFunctions = {

	};


	for (const auto & func : cheatFunctions) {
		mapping[func->getName()] = func;
	}


}

Module* ModuleManager::getModuleByName(std::string name) {
	for (Module* mod : modules) {
		if (StringH::equalsIgnoreCase(mod->getName(), name))return mod;
	}
	return NULL;
}