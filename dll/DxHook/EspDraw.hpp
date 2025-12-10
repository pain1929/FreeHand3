#pragma once
#include "IDraw.h"
#include "pch.h"
#include <queue>
#include "HotKeys.h"
#include "GCheat.h"


class EspDraw : public IDraw {

    std::mutex lock;
public:

    struct Obj {
       bool vis{false};
       ImVec2 headPos;
    };
    std::queue<Obj> objs;


    void draw() override {

        if (!HotKeys::visual.targetPointVis) {
            return;
        }
        static auto white = IM_COL32(255, 255, 255, 255);
        static auto red = IM_COL32(255, 0, 0, 255);
        auto api = ImGui::GetBackgroundDrawList();
        auto size = ImGui::GetIO().DisplaySize;
        std::lock_guard<std::mutex> lg(lock);
        while (!objs.empty()) {
            auto obj = objs.front();
            objs.pop();
            auto headPos = obj.headPos;
            api->AddLine(headPos , {size.x / 2, 0} , obj.vis ? red : white , 1.f);
        }
    }

    void push(const std::list<Obj> & objects) {
        std::lock_guard<std::mutex> lg(lock);
        for (const auto & obj : objects) {
            objs.push(obj);
        }
    }
};

extern std::shared_ptr<EspDraw> GEspDraw;