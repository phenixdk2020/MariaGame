#include "MariaHangerActor.h"
#include "Character/MariaCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AMariaHangerActor::AMariaHangerActor()
{
    PrimaryActorTick.bCanEverTick = false;

    HangerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HangerMesh"));
    SetRootComponent(HangerMesh);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        HangerMesh->SetStaticMesh(CubeMesh.Object);
        HangerMesh->SetRelativeScale3D(FVector(0.04f, 0.45f, 0.04f));
    }

    ClothingMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClothingMesh"));
    ClothingMesh->SetupAttachment(HangerMesh);
    ClothingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMariaHangerActor::ApplyClothingItem()
{
    ClothingMesh->SetSkeletalMesh(ClothingItem.SkeletalMesh);
    ClothingMesh->SetVisibility(bOccupied, true);
}

void AMariaHangerActor::SetHighlighted(bool bHighlighted)
{
    HangerMesh->SetRenderCustomDepth(bHighlighted);
    ClothingMesh->SetRenderCustomDepth(bHighlighted);
}

void AMariaHangerActor::Interact_Implementation(AActor* Interactor)
{
    if (!bOccupied)
    {
        return;
    }

    AMariaCharacter* Maria = Cast<AMariaCharacter>(Interactor);
    if (!Maria)
    {
        return;
    }

    if (Maria->Wardrobe)
    {
        Maria->Wardrobe->AddItem(ClothingItem);
    }

    if (ClothingItem.SkeletalMesh && Maria->WearItem(ClothingItem))
    {
        bOccupied = false;
        ClothingMesh->SetVisibility(false, true);
    }
}

void AMariaHangerActor::SetFocused_Implementation(bool bFocused)
{
    SetHighlighted(bFocused);
}
