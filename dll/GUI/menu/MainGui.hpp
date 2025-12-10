#pragma once

enum heads {
    rage, antiaim, visuals, settings, skins, configs, scripts
};

enum sub_heads {
    general, accuracy, exploits, _general, advanced
};

class MainMenu {
    static void renderTitle();
    static void procKeys();
public:
    static bool showMenu;
    static void loadTheme();
    static void render();
};