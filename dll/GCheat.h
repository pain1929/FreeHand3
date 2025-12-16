//
// Created by pain1929 on 2025/10/3.
//

#ifndef GCHEAT_H
#define GCHEAT_H
#include "DxHook/namespaces.h"
#include "config.h"
#include "DxHook/WndProcHooks.h"
class GCheat {
    void engineInit();
    void drawInit();
public:
    std::shared_ptr<WndProcHooks> wnd_proc_hooks;
    SDK::UEngine * engine;
    SDK::AOakCharacter * character{};
    SDK::AOakPlayerController * controller{};
    SDK::FVector2D screenCenter{}; //!<屏幕中心
    SDK::FVector bestTargetPos{};
    SDK::FVector2D bestTargetScreen{};

    std::string title = xorstr_("cheat template");
    void init();
    void remove();
};


extern GCheat * g_cheat;



#endif //GCHEAT_H
