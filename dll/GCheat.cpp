//
// Created by pain1929 on 2025/10/3.
//

#include "GCheat.h"

#include "IDraw.h"
#include "ThreadManager.hpp"
#include "hooks/IHook.h"
#include "DxHook/NamePainter.h"
#include "../lib/pattern_scan.hpp"
#include "HotKeys.h"
#include "utils/NamePool.hpp"
GCheat * g_cheat = new GCheat;

using IsInitFn = bool (*)();

static bool WaitForInitialization(IsInitFn fn, int attempts = 50, int sleepMs = 100)
{
    for (int i = 0; i < attempts; ++i)
    {
        if (fn())
            return true;
        Sleep(sleepMs);
    }
    return false;
}



// 计算 x86-64 RIP 相对寻址的目标地址
static void* GetRipRelativeAddress(void* instruction_ptr)
{
    // 指令格式: 48 8B 3D xx xx xx xx
    uint8_t* instr = (uint8_t*)instruction_ptr;

    // 取出 disp32 （偏移 3 开始，占 4 字节，小端）
    int32_t disp = *(int32_t*)(instr + 3);

    // 计算下一条指令的地址 (RIP + 7)
    uint8_t* next = instr + 7;

    // 计算目标全局变量地址
    return next + disp;
}

void GCheat::engineInit() {
    NamePool::init();
    engine = SDK::UEngine::GetEngine();
}


void GCheat::drawInit() {
    IDraw::init();
}


void GCheat::init() {


    VMProtectBeginVirtualization("_code_");
#ifndef _DEBUG
    //校验

    auto handle = GetModuleHandle(xorstr_("xa_inj.dll"));
    if (!handle) {
        return;
    }
    auto auth = GetProcAddress(handle , xorstr_("auth"));
    auto get_code = GetProcAddress(handle , xorstr_("get_code"));
    using FN_auth = void (*)(uint8_t * buf , int32_t bufLen , int32_t * outLen);
    using FN_get_code = uint8_t * (*)(int64_t);

    uint8_t get_buf[128]{}; //加密后的 UUID
    int32_t outLen;
    ((FN_auth)auth)(get_buf , 128 , &outLen);
    if (!outLen) {
        return;
    }

    //获取UUID
    auto uuid = ((FN_get_code)get_code)(0xcccc);
    if (!uuid) {
        return;
    }

    auto dsk = ((FN_get_code)get_code)(0xffff);

    uint8_t en_buffer[256]{};
    memcpy(en_buffer , dsk , 32);
    memcpy(en_buffer + 32 , &outLen , 4);
    memcpy(en_buffer + 32 + 4 , get_buf , outLen);

    //开始进行解密UUID
    //|----32DSK----|-4字节数据长度-|--------数据----------|
    auto get_uuid_back = ((FN_get_code)get_code)((int64_t)en_buffer);

    // 5 跟 12 交换位置
    auto temp = get_uuid_back[12];
    get_uuid_back[12] = get_uuid_back[5];
    get_uuid_back[5] = temp;

    for (int i = 0; i < 16 ;i ++ ) {
        if (get_uuid_back[i] != uuid[i]) {
            return;
        }
    }

#endif

    VMProtectEnd();

    HotKeys::Cfg::LoadCfg();


    this->title = xorstr_("正在初始化绘制...");
    drawInit();
    this->title = xorstr_("正在初始化UE结构...");
    engineInit();
    this->title = xorstr_("正在初始化钩子...");
    IHook::init();
    this->title = xorstr_("初始化完成等待中...");
    //hooks::Init();
    hooks_dx11::Init();

    if (WaitForInitialization(hooks_dx11::IsInitialized , 50 , 300))
    {
        globals::activeBackend = globals::Backend::DX11;
    }
    else {
        throw std::runtime_error(xorstr_("gui inited failed"));
    }

}

void GCheat::remove() {
    inputhook::Remove(globals::mainWindow);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}


