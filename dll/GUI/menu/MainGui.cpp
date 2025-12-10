#include "MainGui.hpp"
#include "nav_elements.h"
#include "etc_elements.h"
#include "byte_array.h"
#include "GCheat.h"
#include "RemoveCall.h"
#include "util/Keys.hpp"
#include "HUD.hpp"
#include "HotKeys.h"
#include "ModuleManager.hpp"
//namespace fonts
ImFont* medium;
ImFont* bold;
ImFont* tab_icons;
ImFont* logo;
ImFont* tab_title;
ImFont* tab_title_icon;
ImFont* subtab_title;
ImFont* combo_arrow;
ImFont* chinese_font;


struct FontGuard {
    ImFont* font;
    FontGuard(ImFont* f) : font(f) {
        PushFont(f);
    }
    FontGuard() = delete;
    ~FontGuard() {
        PopFont();
    }
};

//控制开关
bool MainMenu::showMenu = false;

void MainMenu::procKeys() {
    for (const auto & key : ModuleManager::i().cheatFunctions) {
        key->getKeyBind()->process();
    }
}

void MainMenu::renderTitle() {
    FontGuard f(chinese_font);
    HUD::i().render();
}




void MainMenu::loadTheme() {
    //加载字体
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Load Fonts
    ImFontConfig font_config;
    font_config.PixelSnapH = false;
    font_config.OversampleH = 5;
    font_config.OversampleV = 5;
    font_config.RasterizerMultiply = 1.2f;



    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0xE000, 0xE226, // icons
        0,
    };

    font_config.GlyphRanges = ranges;

    medium = io.Fonts->AddFontFromMemoryTTF(PTRootUIMedium, sizeof(PTRootUIMedium), 15.0f, &font_config, ranges);
    bold = io.Fonts->AddFontFromMemoryTTF(PTRootUIBold, sizeof(PTRootUIBold), 15.0f, &font_config, ranges);

    tab_icons = io.Fonts->AddFontFromMemoryTTF(clarityfont, sizeof(clarityfont), 15.0f, &font_config, ranges);
    logo = io.Fonts->AddFontFromMemoryTTF(clarityfont, sizeof(clarityfont), 21.0f, &font_config, ranges);

    tab_title = io.Fonts->AddFontFromMemoryTTF(PTRootUIBold, sizeof(PTRootUIBold), 19.0f, &font_config, ranges);
    tab_title_icon = io.Fonts->AddFontFromMemoryTTF(clarityfont, sizeof(clarityfont), 18.0f, &font_config, ranges);

    subtab_title = io.Fonts->AddFontFromMemoryTTF(PTRootUIBold, sizeof(PTRootUIBold), 15.0f, &font_config, ranges);

    combo_arrow = io.Fonts->AddFontFromMemoryTTF(combo, sizeof(combo), 9.0f, &font_config, ranges);

    chinese_font = io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

}

int get_random_int(int min, int max) {
    static bool call_once = false;
    if (!call_once) {
        srand(time(NULL));
        call_once = true;
    }

    if (min > max) {
        // 如果参数顺序反了，自动交换
        int temp = min;
        min = max;
        max = temp;
    }
    return rand() % (max - min + 1) + min;
}


namespace CButton {
    // 基础半透明按钮样式
inline void StyleTransparentButton()
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 0.9f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(16.0f, 8.0f));
}

// 彩色半透明按钮样式
inline void StyleColoredTransparentButton(const ImVec4& base_color = ImVec4(0.2f, 0.6f, 1.0f, 0.6f))
{
    ImVec4 hovered_color = base_color;
    hovered_color.w = 0.8f;

    ImVec4 active_color = base_color;
    active_color.w = 0.9f;

    ImGui::PushStyleColor(ImGuiCol_Button, base_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hovered_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, active_color);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(16.0f, 8.0f));
}

// 渐变半透明按钮样式
inline void StyleGradientTransparentButton(const ImVec4& start_color = ImVec4(0.3f, 0.8f, 1.0f, 0.6f),
                                   const ImVec4& end_color = ImVec4(0.6f, 0.3f, 1.0f, 0.6f))
{
    // 注意：ImGui本身不支持渐变，这里使用中间色作为近似效果
    ImVec4 mid_color = ImVec4(
        (start_color.x + end_color.x) * 0.5f,
        (start_color.y + end_color.y) * 0.5f,
        (start_color.z + end_color.z) * 0.5f,
        0.7f
    );

    ImGui::PushStyleColor(ImGuiCol_Button, start_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, mid_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, end_color);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20.0f, 10.0f));
}

// 玻璃质感按钮样式
inline void StyleGlassButton()
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.1f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 0.3f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.9f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(18.0f, 9.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.3f));
}

// 恢复按钮样式
inline void RestoreButtonStyle()
{
    ImGui::PopStyleColor(7); // 最多可能有7个颜色
    ImGui::PopStyleVar(3);   // 最多可能有3个样式变量
}

// 便捷函数：带样式的按钮
inline bool TransparentButton(const char* label, const ImVec2& size = ImVec2(0, 0))
{
    StyleTransparentButton();
    bool result = ImGui::Button(label, size);
    ImGui::PopStyleColor(3); // 最多可能有7个颜色
    ImGui::PopStyleVar(2);   // 最多可能有3个样式变量
    return result;
}

inline bool ColoredTransparentButton(const char* label, const ImVec4& color = ImVec4(0.2f, 0.6f, 1.0f, 0.6f), const ImVec2& size = ImVec2(0, 0))
{
    StyleColoredTransparentButton(color);
    bool result = ImGui::Button(label, size);
    ImGui::PopStyleColor(3); // 最多可能有7个颜色
    ImGui::PopStyleVar(2);   // 最多可能有3个样式变量
    return result;
}

inline bool GlassButton(const char* label, const ImVec2& size = ImVec2(0, 0))
{
    StyleGlassButton();
    bool result = ImGui::Button(label, size);
    ImGui::PopStyleColor(5); // 最多可能有7个颜色
    ImGui::PopStyleVar(3);   // 最多可能有3个样式变量
    return result;
}
}


void MainMenu::render() {


    procKeys();
    renderTitle();

    if (!showMenu) {
        return;
    }


    static heads tab{ rage };
    static sub_heads subtab{ general };

    const char* tab_name = tab == rage ? "FreeHand4" : tab == antiaim ? "Anti-aim" : tab == visuals ? "Visuals" : tab == settings ? "Settings" : tab == skins ? "Skins" : tab == configs ? "Configs" : tab == scripts ? "Scripts" : 0;
    const char* tab_icon = tab == rage ? "B" : tab == antiaim ? "C" : tab == visuals ? "D" : tab == settings ? "E" : tab == skins ? "F" : tab == configs ? "G" : tab == scripts ? "H" : 0;

    static bool boolean, boolean_1 = false;
    static int sliderscalar, combo = 0;

    const char* combo_items[3] = { "Value", "Value 1", "Value 2" };

    ImGui::SetNextWindowSize({ 800, 650 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

    ImGui::Begin("hi world", nullptr, ImGuiWindowFlags_NoDecoration); {
        auto draw = ImGui::GetWindowDrawList();

        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();

        ImGuiStyle style = ImGui::GetStyle();

        draw->AddRectFilled(pos, ImVec2(pos.x + 210, pos.y + size.y), ImColor(24, 24, 26), style.WindowRounding, ImDrawFlags_RoundCornersLeft);
        draw->AddLine(ImVec2(pos.x + 210, pos.y + 2), ImVec2(pos.x + 210, pos.y + size.y - 2), ImColor(1.0f, 1.0f, 1.0f, 0.03f));
        draw->AddLine(ImVec2(pos.x + 47, pos.y + 2), ImVec2(pos.x + 47, pos.y + size.y - 2), ImColor(1.0f, 1.0f, 1.0f, 0.03f));
        draw->AddLine(ImVec2(pos.x + 2, pos.y + 47), ImVec2(pos.x + 47, pos.y + 47), ImColor(1.0f, 1.0f, 1.0f, 0.03f));
        draw->AddLine(ImVec2(pos.x + 63, pos.y + 47), ImVec2(pos.x + 195, pos.y + 47), ImColor(1.0f, 1.0f, 1.0f, 0.03f));

        draw->AddText(logo, 21.0f, ImVec2(pos.x + 14, pos.y + 12), ImColor(147, 190, 66), "C");


        draw->AddText(tab_title_icon, 18.0f, ImVec2(pos.x + 65, pos.y + 14), ImColor(147, 190, 66), tab_icon);
        draw->AddText(tab_title, 19.0f, ImVec2(pos.x + 93, pos.y + 15), ImColor(1.0f, 1.0f, 1.0f), tab_name);

        draw->AddRect(pos + ImVec2(1, 1), pos + size - ImVec2(1, 1), ImColor(1.0f, 1.0f, 1.0f, 0.03f), style.WindowRounding);

        ImGui::SetCursorPos({ 8, 56 });
        ImGui::BeginGroup(); {
            if (elements::tab("B", tab == rage)) { tab = rage; }
            if (elements::tab("H" , tab == scripts)) {tab = scripts;}
        } ImGui::EndGroup();

        switch (tab) {
            case rage:
                draw->AddText(subtab_title, 15.0f, ImVec2(pos.x + 72, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.4f), "MAIN");

            ImGui::SetCursorPos({ 57, 86 });
            ImGui::BeginGroup(); {
                FontGuard font_guard(chinese_font);
                if (elements::subtab("主要", subtab == general)) { subtab = general; }
                if (elements::subtab("其他", subtab == accuracy)) { subtab = accuracy; }
                if (elements::subtab("热键绑定" , subtab == exploits)) { subtab = exploits;}
            } ImGui::EndGroup();

            switch (subtab) {
                case general: {
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child("武器修改", ImVec2(240, 280)); {
                        PushFont(chinese_font);
                        ImGui::Checkbox("无限子弹" , &HotKeys::weapon.infAmmo);
                        ImGui::SliderInt("伤害倍率" , &HotKeys::weapon.damageMu , 1 , 10);
                        ImGui::SliderInt("射速倍率" , &HotKeys::weapon.fireSpeedMu , 1 , 10);
                        PopFont();
                    }
                    e_elements::end_child();

                    ImGui::SetCursorPos({ 226, 310 });
                    e_elements::begin_child("视觉", ImVec2(240, 300)); {
                        PushFont(chinese_font);
                        ImGui::Checkbox("高亮" , &HotKeys::visual.highLight);
                        PopFont();
                    }
                    e_elements::end_child();



                    ImGui::SetCursorPos({ 476, 16 });
                    e_elements::begin_child("自瞄", ImVec2(240, 280)); {
                        PushFont(chinese_font);
                        ImGui::Checkbox("自瞄" , &HotKeys::weapon.aimbot);
                        if (HotKeys::weapon.aimbot) {
                            ImGui::SliderFloat("瞄准范围" , &HotKeys::weapon.aimRange , 10 , 300);
                        }
                        ImGui::Checkbox("无扩散" , &HotKeys::weapon.noSpread);
                        PopFont();
                    }
                    e_elements::end_child();

                    ImGui::SetCursorPos({ 476, 310 });
                    e_elements::begin_child("角色修改", ImVec2(240, 300)); {
                        PushFont(chinese_font);
                        ImGui::Checkbox("飞行模式" , &HotKeys::player.flyMode);
                        ImGui::SliderInt("移动速度倍率" , &HotKeys::player.walkSpeedMu , 1, 10);
                        ImGui::SliderInt("飞行速度倍率" , &HotKeys::player.flySpeedMu , 1, 10);
                        PopFont();
                    }
                    e_elements::end_child();

                    break;
                }
                case accuracy : {
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child("暂无内容", ImVec2(240, 550)); {
                        FontGuard f(chinese_font);
                    }
                    e_elements::end_child();

                    break;
                }
                case exploits : {
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child("热键绑定", ImVec2(550, 550)); {
                        PushFont(chinese_font);
                        int indexComb = 0;
                        for (const auto & f : ModuleManager::i().cheatFunctions) {
                            ImGui::Text(f->getName().c_str());
                            ImGuiKeys keys;
                            ImGuiKey key;
                            std::string lab = "##" + std::to_string(indexComb++);
                            keys.process(lab.c_str() ,(ImGuiKey)f->getKeyBind()->key , key);
                            f->setKeyBind(key);
                        }

                        PopFont();
                    }
                    e_elements::end_child();
                    break;
                }
                break;
            }
        }
        ImGui::End();

        ImGui::PopStyleVar();
    }
}

