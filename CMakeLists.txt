cmake_minimum_required(VERSION 3.5)
project(FreeHand3 VERSION 1.0.2)

set(CMAKE_CXX_STANDARD  23)
if (MSVC)
    #add_compile_options(/wd4369 /wd4309 /wd4828)
    #add_compile_options(/bigobj)
    add_link_options("$<$<NOT:$<CONFIG:Debug>>:/DEBUG>")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()




set(CMAKE_PREFIX_PATH
        D:/v143/cryptopp/${CMAKE_BUILD_TYPE}
        D:/v143/polyHook2/${CMAKE_BUILD_TYPE}/lib/PolyHook_2
        D:/v143/polyHook2/${CMAKE_BUILD_TYPE}/lib/cmake
        D:/v143/minhook/lib/minhook
)

find_package(PolyHook_2 REQUIRED)
find_package(minhook REQUIRED)

set(ZLIB_ROOT D:/v143/zlib/x64/${CMAKE_BUILD_TYPE})
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(ZLIB_LIB zlibstaticd.lib)
else ()
    set(ZLIB_LIB zlibstatic.lib)
endif ()

set(OPEN_SSL_ROOT D:/openssl3-win-build/build-VS2022/x64/Release)

include_directories(${CMAKE_CURRENT_LIST_DIR}/hook_lib
        D:/v143/freetype/include/freetype2
        ${ZLIB_ROOT}/include
        json/include
        ${OPEN_SSL_ROOT}/include lib)


link_directories(vmp ${ZLIB_ROOT}/lib
        ${OPEN_SSL_ROOT}
        ${CMAKE_CURRENT_LIST_DIR}/hook_lib
        D:/v143/freetype/lib
)
find_package(cryptopp REQUIRED)

add_subdirectory(dll)