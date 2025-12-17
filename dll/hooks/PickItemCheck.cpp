#include "PickItemCheck.hpp"

#include <HotKeys.h>

extern "C" void * Origin_PickItemCheck = nullptr;
extern "C" bool * pickupItemEnable = nullptr;


void ** PickItemCheck::getOrigin() {
    return &Origin_PickItemCheck;
}


PickItemCheck::PickItemCheck() {
    pickupItemEnable = &HotKeys::player.pickUpItemInf;
}

