#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "ModuleManager.hpp"
using Json = nlohmann::json;

struct Misc {
    std::map<std::string , int> bindKeys;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Misc  , bindKeys)
};


struct GameCfg {
    Misc  misc;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(GameCfg,misc)
};


namespace HotKeys {
    inline Misc misc;

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
                    misc = config.misc;


                    //加载快捷键
                    for (const auto & pair : misc.bindKeys) {
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
                cfg.misc = misc;
                //保存快捷键
                for (const auto & m : ModuleManager::i().mapping) {
                    cfg.misc.bindKeys[m.first] = m.second->getKeyBind()->key;
                }

                Json json = cfg;
                WriteFileStr(G_CfgPath, json.dump());
            }
            catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }




}