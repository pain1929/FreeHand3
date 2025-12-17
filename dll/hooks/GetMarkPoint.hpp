#pragma once


#include "pch.h"

#include "IHook.h"
#include "xorstr.hpp"
#include "pattern_scan.hpp"


class GetMarkPoint : public IHook {
public:
    void *getDetour() override;
    void **getOrigin() override;
    void *getTarget() override {
        return Scanner::PatternScan(xorstr_("Borderlands3.exe") ,
            xorstr_("? ? ? ? 02 00 00 0f 28 c1 ? 0f 11 4d f0 ? c6 c1 55 ? c6 c9 aa ? 0f 11 4d f8 ? 0f 11 45 f4 ? 16 ? ? ? ? ec 7c 5e 05 ? ? ? ? 5e 05 ? 0f 11 45 f0"));
    }
};