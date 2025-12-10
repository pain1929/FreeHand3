#pragma once
#include <pattern_scan.hpp>
#include <xorstr.hpp>
#include "IHook.h"
#include <pch.h>
#include <GCheat.h>
#include "HotKeys.h"
static void * Origin_Angle;


/**
 *
 * @param a1
 * @param viewAngle Out_ 俯仰角
 * @param a3
 * @param pos In_人物坐标
 * @return
 */
inline __int64 __fastcall Hook_Angle(SDK::AWeapon * weapon, SDK::FRotator * viewAngle , __int64 a3, SDK::FVector * pos) {
    using FN = __int64 (__fastcall *) (SDK::AWeapon * , SDK::FRotator * , __int64 , SDK::FVector *);
    auto ret  = ((FN)Origin_Angle) (weapon , viewAngle , a3 ,pos);

    //判断如果这个枪是主角的枪那么自瞄
    if (weapon->Owner == g_cheat->character) {
        //静默单机有效
        if (g_cheat->bestTargetPos.X != 0 && g_cheat->bestTargetPos.Y != 0 &&
            HotKeys::weapon.aimbot && !HotKeys::weapon.lockaim) {
            auto cam = g_cheat->controller->PlayerCameraManager;
            auto view = SDK::UKismetMathLibrary::FindLookAtRotation( cam->GetCameraLocation() , g_cheat->bestTargetPos);
            *viewAngle = view;
        }
    }

    return ret;
}

class GetViewAngle : public IHook {
public:
    void **getOrigin() override {
        return &Origin_Angle;
    }
    void *getDetour() override {
        return Hook_Angle;
    }

    void *getTarget() override {
        return Scanner::PatternScan(xorstr_("Borderlands3.exe") ,
            xorstr_("40 53 48 83 EC ? F2 0F 10 05 ? ? ? ? 48 8B DA 8B 05 ? ? ? ? 48 8B D1"));
    }
};