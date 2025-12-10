//
// Created by pain1929 on 2025/10/5.
//

#include "IDraw.h"
#include "RangeDraw.h"
std::list<std::shared_ptr<IDraw>> IDraw::draws;


void IDraw::init() {
    draws.push_back(std::make_shared<RangeDraw>());
}

