//
// Created by pain1929 on 2025/10/22.
//

#ifndef NAMEPAINTER_H
#define NAMEPAINTER_H


#include "IDraw.h"
#include <mutex>
#include <queue>
#include "GCheat.h"

class NamePainter : public IDraw {
    std::mutex lock;
public:

    struct DrawObject {
        ImVec2 pos;
        std::string name;
    };
    std::queue<DrawObject> names;

    void draw() override {
        auto api = ImGui::GetBackgroundDrawList();
        std::lock_guard<std::mutex> lg(lock);
        while (!names.empty()) {
            auto obj = names.front();
            names.pop();
            // 在Actor头顶绘制名字
            api->AddText(
                nullptr,                      // 使用默认字体
                13,
                obj.pos,                      // 屏幕位置
                IM_COL32(255, 255, 255, 255), // 白色
                obj.name.c_str()             // Actor的名字
            );
        }

    }

    void push(const std::list<DrawObject> & objects) {
        std::lock_guard<std::mutex> lg(lock);
        for (const auto & obj : objects) {
            names.push(obj);
        }
    }
};


extern NamePainter * g_name_painter;



#endif //NAMEPAINTER_H
