#include "MariaHangerActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

AMariaHangerActor::AMariaHangerActor()
{
    PrimaryActorTick.bCanEverTick = false;

    HangerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HangerMesh"));
    SetRootComponent(HangerMesh);

    ClothingMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClothingMesh"));
    ClothingMesh->SetupAttachment(HangerMesh);
    ClothingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMariaHangerActor::ApplyClothingItem()
{
    ClothingMesh->SetSkeletalMesh(ClothingItem.SkeletalMesh);
}

void AMariaHangerActor::SetHighlighted(bool bHighlighted)
{
    HangerMesh->SetRenderCustomDepth(bHighlighted);
    ClothingMesh->SetRenderCustomDepth(bHighlighted);
}
