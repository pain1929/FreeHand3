#pragma once
#include "IDraw.h"
#include "pch.h"
#include "GCheat.h"
#include "HotKeys.h"

class RangeDraw : public IDraw {
public:
    void draw() override {

        static auto white = IM_COL32(255, 255, 255, 255);
        static auto red = IM_COL32(255, 0, 0, 255);

        auto size = ImGui::GetIO().DisplaySize;
        g_cheat->screenCenter = {size.x / 2 , size.y / 2};
        auto list = ImGui::GetBackgroundDrawList();

        auto &bestScreen = g_cheat->bestTargetScreen;
        auto hasTarget = bestScreen.X != 0 && bestScreen.Y != 0;
        if (HotKeys::visual.showRange  &&  (HotKeys::weapon.aimbot || HotKeys::weapon.lockaim ) ) {
            list->AddCircle(size / 2 , HotKeys::weapon.aimRange,hasTarget ? red : white);
        }

        if (HotKeys::visual.targetPoint &&  hasTarget) {
            list->AddLine({bestScreen.X , bestScreen.Y} , size / 2 , red);
        }
    }
};