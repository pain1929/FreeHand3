

include_directories(GUI ./ CppSDK CppSDK/SDK imgui DxHook ../lib ../json/include)

# 配置生成 config.h
configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/config.h.in   # 输入模板文件路径
        ${CMAKE_CURRENT_LIST_DIR}/config.h      # 输出生成的文件路径
        @ONLY                             # 只替换 CMake 变量
)


add_library(${PROJECT_NAME} SHARED DllMain.cpp
        pch.h
        pch.cpp
        DxHook/d3d11hook.cpp
        DxHook/d3d12hook.cpp
        DxHook/globals.cpp
        DxHook/hooks.cpp
        DxHook/inputhooks.cpp
        DxHook/namespaces.h
        imgui/imconfig.h
        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_impl_dx11.cpp
        imgui/imgui_impl_dx11.h
        imgui/imgui_impl_dx12.cpp
        imgui/imgui_impl_dx12.h
        imgui/imgui_impl_win32.cpp
        imgui/imgui_impl_win32.h
        imgui/imgui_internal.h
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        GCheat.cpp
        GCheat.h
        GUI/util/ColorH.cpp
        GUI/util/ColorH.hpp
        GUI/util/FileH.cpp
        GUI/util/FileH.hpp
        GUI/util/MH.cpp
        GUI/util/MH.hpp
        GUI/util/Obf.hpp
        GUI/util/Singleton.hpp
        GUI/util/StringH.cpp
        GUI/util/StringH.hpp
        GUI/util/TimeH.cpp
        GUI/util/TimeH.hpp
        GUI/util/Vec2H.hpp
        GUI/CheatFunc.hpp
        GUI/HUD.cpp
        GUI/HUD.hpp
        GUI/Module.hpp
        GUI/ModuleManager.cpp
        GUI/ModuleManager.hpp
        GUI/Set.cpp
        GUI/Set.hpp
        GUI/SetManager.cpp
        GUI/SetManager.hpp
        hooks/IHook.h
        hooks/AllHooks.cpp
        DxHook/IDraw.cpp
        DxHook/IDraw.h
        RemoveCall.h
        Memory/Memory.cpp
        Memory/Memory.h
        DxHook/NamePainter.cpp
        DxHook/NamePainter.h
        ../lib/ThreadManager.hpp
        GUI/menu/etc_elements.cpp
        GUI/menu/etc_elements.h
        GUI/menu/nav_elements.cpp
        GUI/menu/nav_elements.h
        GUI/menu/nav_elements.h
        GUI/menu/nav_elements.cpp
        GUI/menu/etc_elements.h
        GUI/menu/etc_elements.cpp
        GUI/imgui_helper.hpp
        GUI/imgui_helper.cpp
        GUI/menu/MainGui.hpp
        GUI/menu/MainGui.cpp
        CppSDK/SDK/Basic.cpp
        CppSDK/SDK/CoreUObject_functions.cpp
        CppSDK/SDK/OakGame_functions.cpp
        CppSDK/SDK/Engine_functions.cpp
        hooks/PostRender.hpp
        hooks/GetViewAngle.hpp
        hooks/Shoot.hpp
        DxHook/RangeDraw.h
)


target_link_libraries(${PROJECT_NAME}
        d3d11
        dxgi
        d3d12
        dxguid
        winmm.lib
        ${ZLIB_LIB}
        cryptopp::cryptopp
        PolyHook_2::PolyHook_2
        Zydis::Zydis
        asmjit::asmtk
        minhook::minhook
        freetyped.lib
)

target_precompile_headers(${PROJECT_NAME} PRIVATE "pch.h")
