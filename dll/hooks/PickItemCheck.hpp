#pragma once


#include <pattern_scan.hpp>
#include "xorstr.hpp"
#include "IHook.h"

extern "C" void PickItemCheckHook();
class PickItemCheck : public IHook {
public:
    void *getDetour() override {
        return PickItemCheckHook;
    }
    void **getOrigin() override;

    void *getTarget() override {
        return Scanner::PatternScan(xorstr_("Borderlands3.exe") ,
            xorstr_("? ? ? ? 00 00 ? 03 ? 01 c3 32 c0 c3 cc cc cc cc cc cc cc cc cc cc cc cc cc cc cc 48 85 c9 ? 15 ? ? ? ? 01 00 00"));
    }
};