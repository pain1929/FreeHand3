#pragma once
#include <pattern_scan.hpp>
#include "xorstr.hpp"
#include "IHook.h"
#include "pch.h"
#include "HotKeys.h"
#include "GCheat.h"
#include "utils/utils.h"

inline void * Origin_Shoot;


inline char __fastcall Hook_Shoot(SDK::UObject *  a1) {
    using FN = char (__fastcall * ) (SDK::UObject * );
    if (HotKeys::misc.blockFire) {
        return 0;
    }
    ValueReturn<int32_t> ShotAmmoCostValue;
    ValueReturn<SDK::FRotator> TargetRotation;
    ValueReturn<SDK::FGbxAttributeFloat> Spread;
    ValueReturn<SDK::FGbxAttributeFloat> AccuracyImpulse;
    ValueReturn<SDK::FGbxAttributeFloat> BurstAccuracyImpulseScale;

    if (HotKeys::weapon.infAmmo) {
        auto weapon =g_cheat->character->GetActiveWeapon(0);
        if (weapon && weapon->CurrentFireComponent) {
            ShotAmmoCostValue = ValueReturn<int32_t>(&weapon->CurrentFireComponent->ShotAmmoCost.Value , 0);

        }
    }

    if (HotKeys::weapon.noSpread) {
        auto com =g_cheat->controller->RecoilControlComponent;
        if (com) {
            TargetRotation =   ValueReturn<SDK::FRotator>(&com->TargetRotation , {0 , 0 , 0});
        }

        auto weapon =g_cheat->character->GetActiveWeapon(0);
        if (weapon && weapon->CurrentFireComponent) {
            auto fireCom = weapon->CurrentFireComponent;
            auto newSpread = fireCom->Spread;
            newSpread.Value = 0;
            newSpread.BaseValue = 0;
            Spread = ValueReturn<SDK::FGbxAttributeFloat>(&fireCom->Spread , newSpread);



            auto newAccuracyImpulse = fireCom->AccuracyImpulse;
            newAccuracyImpulse.Value = 0;
            newAccuracyImpulse.BaseValue = 0;
            AccuracyImpulse = ValueReturn<SDK::FGbxAttributeFloat>(&fireCom->AccuracyImpulse , newAccuracyImpulse);


            auto newBurstAccuracyImpulseScale = fireCom->BurstAccuracyImpulseScale;
            newBurstAccuracyImpulseScale.Value = 0;
            newBurstAccuracyImpulseScale.BaseValue = 0;
            BurstAccuracyImpulseScale = ValueReturn<SDK::FGbxAttributeFloat>(&fireCom->BurstAccuracyImpulseScale , newBurstAccuracyImpulseScale);

        }
    }





    return ((FN)Origin_Shoot)(a1);
}


class Shoot : public IHook {
public:
    void *getTarget() override {
        return Scanner::PatternScan(xorstr_("Borderlands3.exe") ,
            xorstr_("40 55 53 56 41 55 48 8D AC 24 A8 FB FF FF"));
    }

    void *getDetour() override {
        return Hook_Shoot;
    }

    void **getOrigin() override {
        return &Origin_Shoot;
    }
};