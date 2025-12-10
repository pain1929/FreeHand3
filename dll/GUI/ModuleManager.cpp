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
		new CheatFunction(&HotKeys::player.flyMode , "飞行模式"),
		new CheatFunction(&HotKeys::weapon.aimbot , "静默自瞄"),
		new CheatFunction(&HotKeys::weapon.infAmmo , "无限子弹"),
		new CheatFunction(&HotKeys::weapon.noSpread , "无扩散"),
		new CheatFunction(&HotKeys::visual.showRange , "显示瞄准范围"),
		new CheatFunction(&HotKeys::visual.targetPoint , "显示自瞄目标"),
		new CheatFunction(&HotKeys::visual.targetPointVis , "显示目标指向")
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