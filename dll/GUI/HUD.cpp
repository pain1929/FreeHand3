#include "HUD.hpp"
#include "util/ColorH.hpp"
#include "imgui_helper.hpp"
#include "ModuleManager.hpp"
#include "util/TimeH.hpp"
#include "SetManager.hpp"
#include "config.h"
#include <Windows.h>
#include "HotKeys.h"
#include "../GCheat.h"
HUD::HUD() : Module(obf("HUD"), obf("Renders Overlay like ModuleList")) {
	colML = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(255, 100, 100, 255), obf("colML"), getName())).getVec4();
	colML_Bg = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(0, 0, 0, 100), obf("colML_Bg"), getName())).getVec4();

	speedML = &SetManager::i().add(new Set(0.4f, 0.1f, 1.f, obf("speedML"), getName())).getVec3f();
	offsetML = &SetManager::i().add(new Set(0.06f, 0.f, 0.4f, obf("offsetML"), getName())).getVec3f();
	rangeML = &SetManager::i().add(new Set(0.02f, 0.f, 0.1f, obf("rangeML"), getName())).getVec3f();

	isML = &SetManager::i().add(new Set(false, obf("isML"), getName())).getBVal();
	isMLRainbow = &SetManager::i().add(new Set(true, obf("isMLRainbow"), getName())).getBVal();
	isTime = &SetManager::i().add(new Set(false, obf("isTime"), getName())).getBVal();

	alignML = &SetManager::i().add(new Set(0, obf("alignML"), getName())).getIVal();
	sortML = &SetManager::i().add(new Set(1, obf("sortML"), getName())).getIVal();
}

void HUD::renderImGui() {

}

void HUD::renderML() {
    ImVec2 winPadding = ImGui::GetStyle().WindowPadding;

    std::vector<const std::string*> modules;
    float calcHeight = winPadding.y * 2, longestStr = 0;

    ImVec2 vec2 = ImGui::CalcTextSize(g_cheat->title.c_str());
    if (vec2.x > longestStr) longestStr = vec2.x;
    calcHeight += vec2.y;

    modules.push_back(&g_cheat->title);

    for (const auto& func : ModuleManager::i().cheatFunctions) {
        if (!func->isEnabled()) continue;

        ImVec2 vec2 = ImGui::CalcTextSize(func->getName().c_str());
        if (vec2.x > longestStr) longestStr = vec2.x;
        calcHeight += vec2.y;

        modules.push_back(&func->getName());
    }

    if (modules.empty()) return;

    calcHeight += ImGui::GetStyle().ItemSpacing.y * (modules.size() - 1);

    float windowWidth = longestStr + winPadding.x * 2;
    constexpr float marginX = 5.0f;
    ImGui::SetNextWindowSize(ImVec2(windowWidth, calcHeight));
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - windowWidth - marginX, 0));

    // 设置更多样式为透明
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));        // 边框透明
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));       // 框架背景透明
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0, 0, 0, 0));       // 弹出背景透明

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoBackground;  // 新增：无背景

    ImGui::Begin("##MLDisplay", nullptr, flags);

    // 降序
    std::sort(modules.begin() + 1, modules.end(), [](const std::string* a, const std::string* b) {
        return a->length() > b->length();
    });

    for (int i = 0; i < modules.size(); i++) {
        const std::string* str = modules[i];

        float width = ImGui::CalcTextSize(str->c_str()).x;
        ImGui::Dummy(ImVec2(longestStr - width - ImGui::GetStyle().ItemSpacing.x, 0));
        ImGui::SameLine();

        if (*isMLRainbow) {
            float h, s, v;
            ColorH::RGBtoHSV(colML->x, colML->y, colML->z, h, s, v);
            ImGuiHelper::chromaText(*str, s, v, colML->w, i * (offsetML->x + 1), speedML->x, rangeML->x);
        } else {
            ImGui::PushStyleColor(ImGuiCol_Text, *colML);
            ImGui::Text(str->c_str());
            ImGui::PopStyleColor();
        }
    }

    ImGui::End();
    ImGui::PopStyleColor(4); // 弹出所有样式颜色
}


void HUD::render() {
	renderML();
}