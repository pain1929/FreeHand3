#pragma once
#include <map>
#include <string>
#include <set>
class IHook {
public:
    static std::set<std::shared_ptr<IHook>> hooks;
    static void registerHook(const std::shared_ptr<IHook> & hook);
    static void init();

    virtual void * getDetour() = 0;
    virtual void * getTarget() = 0;
    virtual void ** getOrigin() = 0;

    virtual void remove() {
       MH_RemoveHook(getTarget());
    }

    void hook() {
        auto target = getTarget();
        auto status = MH_CreateHook(target, getDetour() , getOrigin());
        if (status != MH_OK) {
            throw std::runtime_error(xorstr_("hook error ") + std::string(MH_StatusToString(status)));
        }
        status = MH_EnableHook(target);
        if (status != MH_OK) {
            throw std::runtime_error(xorstr_("hook error ") + std::string(MH_StatusToString(status)));
        }
    }

    virtual ~IHook() = default;
};