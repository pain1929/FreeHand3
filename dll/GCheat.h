//
// Created by pain1929 on 2025/10/3.
//

#ifndef GCHEAT_H
#define GCHEAT_H
#include "DxHook/namespaces.h"
#include "config.h"

class GCheat {
    void engineInit();
    void drawInit();
public:
    std::string title = xorstr_("cheat template");
    void init();
    void remove();
};


extern GCheat * g_cheat;



#endif //GCHEAT_H
