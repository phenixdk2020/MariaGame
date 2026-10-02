#include "MariaWardrobeActor.h"
#include "MariaHangerActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

AMariaWardrobeActor::AMariaWardrobeActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    CabinetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CabinetMesh"));
    CabinetMesh->SetupAttachment(Root);

    HangerRail = CreateDefaultSubobject<USceneComponent>(TEXT("HangerRail"));
    HangerRail->SetupAttachment(Root);
}

void AMariaWardrobeActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    BuildHangers();
}

void AMariaWardrobeActor::ClearHangers()
{
    for (AMariaHangerActor* Hanger : SpawnedHangers)
    {
        if (IsValid(Hanger))
        {
            Hanger->Destroy();
        }
    }

    SpawnedHangers.Reset();
}

void AMariaWardrobeActor::BuildHangers()
{
    ClearHangers();

    if (!GetWorld() || !HangerClass || HangerCount <= 0)
    {
        return;
    }

    const float TotalWidth = (HangerCount - 1) * HangerSpacing;
    const float StartY = -TotalWidth * 0.5f;

    for (int32 Index = 0; Index < HangerCount; ++Index)
    {
        const FVector LocalOffset(0.0f, StartY + Index * HangerSpacing, 0.0f);
        const FTransform SpawnTransform(HangerRail->GetComponentRotation(),
                                        HangerRail->GetComponentLocation() + HangerRail->GetComponentTransform().TransformVectorNoScale(LocalOffset));

        AMariaHangerActor* Hanger = GetWorld()->SpawnActorDeferred<AMariaHangerActor>(
            HangerClass,
            SpawnTransform,
            this,
            nullptr,
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        if (Hanger)
        {
            Hanger->FinishSpawning(SpawnTransform);
            Hanger->AttachToComponent(HangerRail, FAttachmentTransformRules::KeepWorldTransform);
            SpawnedHangers.Add(Hanger);
        }
    }
}
