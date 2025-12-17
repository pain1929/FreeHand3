#pragma once


#include <pattern_scan.hpp>
#include "xorstr.hpp"
#include "IHook.h"

extern "C" void PickItemCheckHook();
class PickItemCheck : public IHook {
public:
    PickItemCheck();
    void *getDetour() override {
        return PickItemCheckHook;
    }
    void **getOrigin() override;

    void *getTarget() override {
        //0x3003453  0x3003593
        auto func =  (char * )Scanner::PatternScan(xorstr_("Borderlands3.exe") ,
            xorstr_("? 89 58 08 ? 89 68 10 ? 89 70 18 ? 89 78 20 41 56 ? 83 ec 30 49 8b e9 45 8b f0"));
        return func + 0x3003593 - 0x3003453;
    }
};