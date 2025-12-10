//
// Created by pain1929 on 2025/10/5.
//

#ifndef IDRAW_H
#define IDRAW_H
#include <list>
#include "util/MH.hpp"

class IDraw {
public:
    static std::list<std::shared_ptr<IDraw>> draws;

    static float angleToScreenDistance(float angleDistance, float screenWidth, float playerFov) {
        // 原理：角度距离 / 玩家FOV = 屏幕位置 / 屏幕范围
        return (angleDistance / playerFov) * (screenWidth / 2.0f);
    }

    virtual ~IDraw() = default;
    virtual void draw() = 0;
};



#endif //IDRAW_H
