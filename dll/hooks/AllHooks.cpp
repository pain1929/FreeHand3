#include "GCheat.h"
#include "IHook.h"

std::set<std::shared_ptr<IHook>> IHook::hooks;

void IHook::init() {

    for (const auto & h : hooks) {
       h->hook();
    }
}

void IHook::registerHook(const std::shared_ptr<IHook> &hook) {
    hooks.insert(hook);
}





