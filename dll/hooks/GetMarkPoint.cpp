#include "GetMarkPoint.hpp"

#include <HotKeys.h>

#include "GCheat.h"


extern "C" void * Origin_GetMarkPoint = nullptr;

extern "C" void Hook_GetMarkPoint(SDK::FVector * const markPoint) {
     //std::cout << markPoint->X <<" " << markPoint->Y <<" " << markPoint->Z << std::endl;
    //std::cout <<"hello world \n";
    if (HotKeys::player.tpToMarkPoint) {
        g_cheat->character->K2_SetActorLocation(*markPoint , false , nullptr , false);
    }
}

extern "C" void Tmp_GetMarkPoint();


void *GetMarkPoint::getDetour() {
    return Tmp_GetMarkPoint;
}

void **GetMarkPoint::getOrigin() {
    return &Origin_GetMarkPoint;
}

