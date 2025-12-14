#pragma once
#include "EspDraw.hpp"
#include "IHook.h"
#include "NamePainter.h"
#include "HotKeys.h"
#include "utils/utils.h"
#include <unordered_map>
static void * Origin_Render;

struct WeaponMdf {
    float speedX{1.f};
    float damageX{1.f};
};


inline bool IsHit(const SDK::FVector &pos, SDK::UWorld *world, const SDK::FVector &camLoc) {
    SDK::FHitResult hitResult{};
    // hit == 0 表示射线击中
    auto hit = SDK::UKismetSystemLibrary::LineTraceSingle(world,
                                                          camLoc,
                                                          pos,
                                                          SDK::ETraceTypeQuery::TraceTypeQuery1,
                                                          true, {},
                                                          SDK::EDrawDebugTrace::ForDuration,
                                                          &hitResult, true,
                                                          {1.f, 1.f, 1.f, 1.f},
                                                          {1.f, 0.f, 0.f, 1.f},
                                                          300.f);
    return !hit;
}


inline void Modifys() {
    static std::unordered_map<SDK::AWeapon *  , WeaponMdf> mdfs;
    if (!g_cheat->character) {
        return;
    }
    auto weapon = g_cheat->character->GetActiveWeapon(0);

    if (weapon && weapon->CurrentFireComponent) {
        auto &mif = mdfs[weapon];
        weapon->CurrentFireComponent->FireRate.Value =
            weapon->CurrentFireComponent->FireRate.Value / mif.speedX * HotKeys::weapon.fireSpeedMu;

        weapon->CurrentFireComponent->Damage.Value =  weapon->CurrentFireComponent->Damage.Value / mif.damageX * HotKeys::weapon.damageMu;

        mif.damageX = HotKeys::weapon.damageMu;
        mif.speedX = HotKeys::weapon.fireSpeedMu;
    }


    if (g_cheat->character->OakCharacterMovement) {
        static void * lastMove = nullptr;
        static float w{};
        static float f{};
        if (lastMove && lastMove != g_cheat->character->OakCharacterMovement) {
            lastMove = nullptr;
        }

        if (!lastMove) {
            w = g_cheat->character->OakCharacterMovement->MaxSprintSpeed.Value;
            f = g_cheat->character->OakCharacterMovement->MaxFlySpeed.Value;
            lastMove = g_cheat->character->OakCharacterMovement;
        }

        g_cheat->character->OakCharacterMovement->MaxSprintSpeed.Value = w * HotKeys::player.walkSpeedMu;
        g_cheat->character->OakCharacterMovement->MaxFlySpeed.Value = f * HotKeys::player.flySpeedMu;
    }

}

inline void __fastcall Hook_Render(SDK::UGameViewportClient * viewport , SDK::UCanvas * canvas) {
    auto world = viewport->World;
    auto game = world->OwningGameInstance;
    auto state = world->GameState;

    do {
        if (!world ||
            !game ||
            !state) {
            break;
        }

        auto level = world->PersistentLevel;

        if (!level)
            break;

        auto local = game->LocalPlayers[0];
        if (!local) {
            break;
        }

        auto controller = local->PlayerController;
        if (!controller) {
            break;
        }

        SDK::AOakPlayerController *oakController = nullptr;
        if (controller->IsA(SDK::AOakPlayerController::StaticClass())) {
            oakController = (SDK::AOakPlayerController *) controller;
        }

        if (!oakController) {
            break;
        }
        g_cheat->controller = oakController;


        auto character = controller->Character;
        if (!character) {
            break;
        }

        SDK::AOakCharacter *oakCharacter = nullptr;
        if (character->IsA(SDK::AOakCharacter::StaticClass())) {
            oakCharacter = (SDK::AOakCharacter *) character;
        }

        if (!oakCharacter)
            break;
        g_cheat->character = oakCharacter;


        Modifys();

        if (HotKeys::player.flyMode) {
            oakCharacter->OakCharacterMovement->MovementMode = SDK::EMovementMode::MOVE_Flying;
        } else if (oakCharacter->OakCharacterMovement->MovementMode == SDK::EMovementMode::MOVE_Flying){
            oakCharacter->OakCharacterMovement->MovementMode = SDK::EMovementMode::MOVE_Walking;
        }




        SDK::FVector2D bestTarget{};
        SDK::FVector bestTargetPos{};
        float distanceToCenter{10000.f};
        bool findOut{false};

        std::list<EspDraw::Obj> espObjs;
        for (const auto &actor: level->Actors) {
            if (!actor) continue;
            if (actor == oakCharacter) continue;


            if (actor->IsA(SDK::ABPChar_Enemy_C::StaticClass())) {
                auto cha = (SDK::ABPChar_Enemy_C *) actor;

                //已经死亡
                if (cha->HealthState == SDK::EHealthState::Dead) {
                    continue;
                }



                auto mesh = (SDK::USkeletalMeshComponent *) actor->GetComponentByClass(
                    SDK::USkeletalMeshComponent::StaticClass());


                SDK::FVector pos{};
                if (mesh) {
                    for (int i = 0; i < mesh->GetNumBones();i++) {
                        auto boneFName = mesh->GetBoneName(i);
                        if (boneFName.GetRawString() == "Head") {
                            pos = mesh->GetSocketLocation(boneFName);
                            break;
                        }
                    }
                } else {

                }


                // Stencil Glow
                if (HotKeys::visual.highLight) {
                    cha->GbxMesh->SetRenderCustomDepth(true);
                    cha->GbxMesh->SetCustomDepthStencilValue(255);
                    cha->GbxMesh->SetCustomDepthStencilWriteMask(SDK::ERendererStencilMask::ERSM_Default);
                    cha->GbxMesh->CustomColorValueView0 = { 1.f, 0.f, 0.f, 1.f };
                    cha->GbxMesh->CustomColorValueView1 = { 1.f, 0.f, 0.f, 1.f };
                    cha->GbxMesh->CustomColorValueView2 = { 1.f, 0.f, 0.f, 1.f };
                    cha->GbxMesh->CustomColorValueView3 = { 1.f, 0.f, 0.f, 1.f };
                }


                //检查命中
                auto isHit = IsHit(pos, world, g_cheat->controller->PlayerCameraManager->GetCameraLocation());


                if (HotKeys::visual.targetPointVis &&  pos.X !=0 && pos.Y !=0) {
                    SDK::FVector2D screen{};
                    if (controller->ProjectWorldLocationToScreen(pos, &screen, false, false)) {
                        espObjs.push_back(EspDraw::Obj{isHit , {screen.X , screen.Y}});
                    }

                }



                if (isHit ) {
                    SDK::FVector2D screen{};
                    if (controller->ProjectWorldLocationToScreen(pos, &screen, false, false)) {
                        //检查范围
                        if (!IN_RANGE(screen.X, 0, g_cheat->screenCenter.X * 2) ||
                            !IN_RANGE(screen.Y, 0, g_cheat->screenCenter.Y * 2)) {
                            continue;
                            }

                        //范围校验
                        auto dis = screen.GetDistanceTo(g_cheat->screenCenter);
                        if (dis < distanceToCenter && dis < HotKeys::weapon.aimRange) {
                            distanceToCenter = dis;
                            bestTarget = screen;
                            bestTargetPos = pos;
                            findOut = true;
                        }
                    }
                }



            }
        }

        GEspDraw->push(espObjs);


        if (findOut) {
            g_cheat->bestTargetPos = bestTargetPos;
            g_cheat->bestTargetScreen = bestTarget;
        } else {
            g_cheat->bestTargetPos = {};
            g_cheat->bestTargetScreen = {};
        }


    } while (false);


    using FN = void (*)(SDK::UGameViewportClient *, SDK::UCanvas *);
    ((FN)Origin_Render)(viewport, canvas);
}


class PostRender : public IHook {
public:
    void *getDetour() override {
        return Hook_Render;
    }
    void **getOrigin() override {
        return &Origin_Render;
    }
    void *getTarget() override {
        auto vp = g_cheat->engine->GameViewport;
        auto vtable = (void **) vp->VTable;
        return vtable[0x63];
    }
};