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

    ClothingPreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClothingPreview"));
    ClothingPreviewMesh->SetupAttachment(HangerMesh);
    ClothingPreviewMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -45.0f));
    ClothingPreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (CubeMesh.Succeeded())
    {
        ClothingPreviewMesh->SetStaticMesh(CubeMesh.Object);
    }

    ClothingMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClothingMesh"));
    ClothingMesh->SetupAttachment(HangerMesh);
    ClothingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMariaHangerActor::ApplyClothingItem()
{
    ClothingMesh->SetSkeletalMesh(ClothingItem.SkeletalMesh);
    ClothingPreviewMesh->SetRelativeScale3D(ClothingItem.PreviewScale);
    SetOccupied(bOccupied);
}

void AMariaHangerActor::SetOccupied(bool bNewOccupied)
{
    bOccupied = bNewOccupied;
    ClothingMesh->SetVisibility(bOccupied && ClothingItem.SkeletalMesh != nullptr, true);
    ClothingPreviewMesh->SetVisibility(bOccupied && ClothingItem.SkeletalMesh == nullptr, true);
}

void AMariaHangerActor::SetHighlighted(bool bHighlighted)
{
    HangerMesh->SetRenderCustomDepth(bHighlighted);
    ClothingPreviewMesh->SetRenderCustomDepth(bHighlighted);
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

    if (Maria->WearItem(ClothingItem))
    {
        SetOccupied(false);
    }
}

void AMariaHangerActor::SetFocused_Implementation(bool bFocused)
{
    SetHighlighted(bFocused);
}
