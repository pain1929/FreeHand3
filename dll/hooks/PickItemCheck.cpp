#include "PickItemCheck.hpp"

extern "C" void * Origin_PickItemCheck = nullptr;

void ** PickItemCheck::getOrigin() {
    return &Origin_PickItemCheck;
}
