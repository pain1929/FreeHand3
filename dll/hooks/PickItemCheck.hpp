#pragma once


#include <pattern_scan.hpp>
#include "xorstr.hpp"
#include "IHook.h"

inline void * Origin_PickItemCheck;

inline bool __fastcall Hook_PickItemCheck(int64_t a1, int32_t a2 , int32_t a3 , int64_t a4) {
    using FN = bool (__fastcall * ) (int64_t a1, int32_t a2 , int32_t a3 , int64_t a4);
    ((FN)Origin_PickItemCheck)(a1 ,a2 ,a3 ,a4);
    return true;
}


class PickItemCheck : public IHook {
public:
    void *getDetour() override {
        return Hook_PickItemCheck;
    }
    void **getOrigin() override {
        return &Origin_PickItemCheck;
    }

    void *getTarget() override {
        return Scanner::PatternScan(xorstr_("Borderlands3.exe") ,
            xorstr_("? 89 58 08 ? 89 68 10 ? 89 70 18 ? 89 78 20 41 56 ? 83 ec 30 49 8b e9 45 8b f0"));
    }
};