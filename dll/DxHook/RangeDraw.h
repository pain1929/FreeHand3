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
        auto list = ImGui::GetBackgroundDrawList();

        if (HotKeys::weapon.aimbot || HotKeys::weapon.lockaim) {

            auto &bestScreen = g_cheat->bestTargetScreen;
            auto hasTarget = bestScreen.X != 0 && bestScreen.Y != 0;
            list->AddCircle(size / 2 , HotKeys::weapon.aimRange,hasTarget ? red : white);
            if (hasTarget) {
                list->AddLine({bestScreen.X , bestScreen.Y} , size / 2 , red);
            }

        }
    }
};