#include "GCheat.h"
#include "IHook.h"
#include "PostRender.hpp"
#include "GetViewAngle.hpp"
#include "Shoot.hpp"
#include "PickItemCheck.hpp"
#include "EchoAnyWhere.hpp"
std::set<std::shared_ptr<IHook>> IHook::hooks;

void IHook::init() {

    registerHook(std::make_shared<PostRender>());
    registerHook(std::make_shared<GetViewAngle>());
    registerHook(std::make_shared<Shoot>());
    registerHook(std::make_shared<PickItemCheck>());
    registerHook(std::make_shared<EchoAnyWhere>());

    for (const auto & h : hooks) {
       h->hook();
    }
}

void IHook::registerHook(const std::shared_ptr<IHook> &hook) {
    hooks.insert(hook);
}





