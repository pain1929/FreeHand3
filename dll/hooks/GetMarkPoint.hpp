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
        //0x12cdf00  0x12ce079
        auto func =  (char * )Scanner::PatternScan(xorstr_("Borderlands3.exe") ,
            xorstr_("? 89 5c 24 18 ? 89 7c 24 20 55 48 8b ec ? 83 ec 40 ? ? ? ? 01 00 00 48 8b d9"));

        return func + 0x12ce079 - 0x12cdf00;
    }
};