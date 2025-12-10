#pragma once
#include "imgui.h"

#include <vector>
#include <string>

namespace ImGuiHelper {
    float getWidth();
    float getHeight();
    ImVec4 rgbaToVec4(float r, float g, float b, float a);
    ImVec4 rgbaToVec4(ImColor& col);
    ImVec2 getTextLength(std::string text);
    void renderCombo(std::string title, std::vector<std::string>items, int& index, int comboWidth);

    void chromaText(std::string text, float sat, float value, float alpha, float offset, float speed, float range);
}