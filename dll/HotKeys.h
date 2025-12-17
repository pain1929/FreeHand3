#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "GUI/ModuleManager.hpp"
using Json = nlohmann::json;


struct User {
    std::string username;
    std::string password;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(User , username , password)
};

struct Misc {
    std::string serverIp;
    bool showHUD = true;
    float HUDY = 0;
    bool blockFire {false};
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Misc , HUDY , blockFire)
};

struct Weapon {
    bool aimbot {false}; //!<瞄准辅助
    bool lockaim{false}; //!<锁定瞄准
    float aimRange{100.f}; //!<瞄准范
    bool infAmmo {false}; //!<无限子弹
    bool noSpread {false}; //!<无扩散
    float damageMu{1};   //!<伤害倍率
    float fireSpeedMu{1}; //!<射速倍率
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Weapon , aimbot , lockaim , aimRange ,  infAmmo , noSpread,damageMu,fireSpeedMu)
};

struct Visual {
    bool highLight{false};//!<高亮
    bool showRange{false};
    bool targetPoint{false};
    bool targetPointVis{false};
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Visual , highLight,showRange , targetPoint , targetPointVis)
};


struct Player {
    bool flyMode{false};
    int walkSpeedMu{1}; //!<移动速度倍率
    int flySpeedMu{1}; //!<飞行速度倍率
    bool anyWhereEcho{false};
    bool tpToMarkPoint{false};
    bool pickUpItemInf{false};
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Player , flyMode  , walkSpeedMu , flySpeedMu , anyWhereEcho , tpToMarkPoint , pickUpItemInf)
};


struct GameCfg {
    User user;
    Misc misc;
    Weapon weapon;
    Player player;
    Visual visual;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(GameCfg , user , misc , weapon , player , visual)
};


namespace HotKeys {
    namespace Menu {
        inline bool showMenu = false;
    }

    inline User user;
    inline Misc misc;
    inline Weapon weapon;
    inline Player player;
    inline Visual visual;

    namespace Cfg {
        inline std::wstring G_CfgPath = L"./freeHand.cfg";

        inline std::string ReadFileStr(std::wstring path) {
            std::ifstream in(path);
            if (!in.is_open()) {
                return {};
            }
            std::ostringstream oss;
            oss << in.rdbuf();
            in.close();
            return oss.str();
        }

        inline void WriteFileStr(std::wstring path, std::string str) {
            std::ofstream out(path, std::ios::binary);
            if (out.is_open()) {
                out.write(str.c_str(), str.length());
            }
        }

        inline void LoadCfg() {
            try {
                std::ifstream f(G_CfgPath);
                if (f.is_open()) {
                    Json data = Json::parse(f);
                    GameCfg config = data.get<GameCfg>();

                    visual = config.visual;
                    weapon = config.weapon;
                    player = config.player;
                    user = config.user;
                    misc = config.misc;
                }

                std::ifstream keys("keys");
                if (keys.is_open()) {
                    Json data = Json::parse(keys);
                    auto mapping = data.get<std::map<std::string , int>>();
                    for (const auto & pair : mapping) {
                        auto cheatFunc = ModuleManager::i().mapping[pair.first];
                        if (cheatFunc) {
                            cheatFunc->setKeyBind(pair.second);
                        }
                    }
                }

            }
            catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }

        inline void SaveCfg() {
            try {
                GameCfg cfg;
                cfg.visual = visual;
                cfg.weapon = weapon;
                cfg.player = player;
                cfg.user = user;
                cfg.misc = misc;

                Json json = cfg;
                WriteFileStr(G_CfgPath, json.dump());

                std::map<std::string , int> map;
                for (const auto & cheatFunc :  ModuleManager::i().cheatFunctions) {
                    map[cheatFunc->getName()] = cheatFunc->getKeyBind()->key;
                }
                Json j = map;
                WriteFileStr(L"keys" , j.dump());

            }
            catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }




}