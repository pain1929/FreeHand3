#pragma once


#include "pch.h"

#include "IHook.h"
#include "xorstr.hpp"
#include "pattern_scan.hpp"
inline void * Origin_GetMarkPoint;

inline int64_t __fastcall Hook_GetMarkPoint(int64_t a1 ) {
    using FN = int64_t (__fastcall *) (int64_t a1 );
    auto ret = ((FN)Origin_GetMarkPoint)(a1);
    if (ret) {
        if (*(bool *)(ret + 0x598)) {
            auto obj2 = *(int64_t *)(ret + 0x168);
            if (obj2) {
                auto markPos = (SDK::FVector *)(obj2 + 0x220);
                std::cout <<markPos->X <<" " << markPos->Y <<" " << markPos->Z << std::endl;
            }
        }

    }
    return ret;
}


class GetMarkPoint : public IHook {
public:
    void *getDetour() override {
        return Hook_GetMarkPoint;
    }
    void **getOrigin() override {
        return &Origin_GetMarkPoint;
    }
    void *getTarget() override {
        return Scanner::PatternScan(xorstr_("Borderlands3.exe") ,
            xorstr_("? 8b 41 04 45 85 c0 ? 4e 8b 01 85 c0"));
    }
};