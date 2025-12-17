#pragma once

#include "IHook.h"
#include "pattern_scan.hpp"
#include "xorstr.hpp"
inline void *Origin_EchoAnyWhere;

inline bool __fastcall Hook_EchoAnyWhere(int64_t a1 , int64_t a2) {
    using FN = bool (__fastcall *) (int64_t a1 , int64_t a2);
    ((FN)Origin_EchoAnyWhere)(a1 ,a2);
    return true;
}


class EchoAnyWhere : public IHook {
    //? 89 5c 24 08 ? 89 6c 24 10 ? 89 74 24 18 ? 89 7c 24 20 41 56 ? ? ? ? 00 00 00 ? ? ? ? 0a 00 00 48 8b ea 48 8b f1

public:
    void *getDetour() override {
        return Hook_EchoAnyWhere;
    }
    void **getOrigin() override {
        return &Origin_EchoAnyWhere;
    }
    void *getTarget() override {
        return Scanner::PatternScan(xorstr_("Borderlands3.exe") ,
                    xorstr_("? 89 5c 24 08 ? 89 6c 24 10 ? 89 74 24 18 ? 89 7c 24 20 41 56 ? ? ? ? 00 00 00 ? ? ? ? 0a 00 00 48 8b ea 48 8b f1"));
    }
};