#include "MariaHangerActor.h"
#include "Character/MariaCharacter.h"
#include "Wardrobe/MariaWardrobeComponent.h"
#include "Wardrobe/MariaWardrobeActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

AMariaHangerActor::AMariaHangerActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

    HangerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HangerCenter"));
    HangerMesh->SetupAttachment(Root);
    HangerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -7.0f));
    HangerMesh->SetRelativeScale3D(FVector(0.035f, 0.12f, 0.035f));

    LeftShoulder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftShoulder"));
    LeftShoulder->SetupAttachment(Root);
    LeftShoulder->SetRelativeLocation(FVector(0.0f, -21.0f, -16.0f));
    LeftShoulder->SetRelativeRotation(FRotator(24.0f, 0.0f, 0.0f));
    LeftShoulder->SetRelativeScale3D(FVector(0.035f, 0.24f, 0.035f));

    RightShoulder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightShoulder"));
    RightShoulder->SetupAttachment(Root);
    RightShoulder->SetRelativeLocation(FVector(0.0f, 21.0f, -16.0f));
    RightShoulder->SetRelativeRotation(FRotator(-24.0f, 0.0f, 0.0f));
    RightShoulder->SetRelativeScale3D(FVector(0.035f, 0.24f, 0.035f));

    BottomBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomBar"));
    BottomBar->SetupAttachment(Root);
    BottomBar->SetRelativeLocation(FVector(0.0f, 0.0f, -31.0f));
    BottomBar->SetRelativeScale3D(FVector(0.025f, 0.43f, 0.025f));

    HookStem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HookStem"));
    HookStem->SetupAttachment(Root);
    HookStem->SetRelativeLocation(FVector(0.0f, 0.0f, 8.0f));
    HookStem->SetRelativeScale3D(FVector(0.025f, 0.025f, 0.15f));

    HookTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HookTop"));
    HookTop->SetupAttachment(Root);
    HookTop->SetRelativeLocation(FVector(0.0f, 0.0f, 23.0f));
    HookTop->SetRelativeScale3D(FVector(0.055f));

    if (CubeMesh.Succeeded())
    {
        HangerMesh->SetStaticMesh(CubeMesh.Object);
        LeftShoulder->SetStaticMesh(CubeMesh.Object);
        RightShoulder->SetStaticMesh(CubeMesh.Object);
        BottomBar->SetStaticMesh(CubeMesh.Object);
        HookStem->SetStaticMesh(CubeMesh.Object);
    }

    if (SphereMesh.Succeeded())
    {
        HookTop->SetStaticMesh(SphereMesh.Object);
    }

    ClothingPreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClothingPreview"));
    ClothingPreviewMesh->SetupAttachment(Root);
    ClothingPreviewMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -58.0f));
    ClothingPreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (CubeMesh.Succeeded())
    {
        ClothingPreviewMesh->SetStaticMesh(CubeMesh.Object);
    }

    ClothingMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClothingMesh"));
    ClothingMesh->SetupAttachment(Root);
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
    LeftShoulder->SetRenderCustomDepth(bHighlighted);
    RightShoulder->SetRenderCustomDepth(bHighlighted);
    BottomBar->SetRenderCustomDepth(bHighlighted);
    HookStem->SetRenderCustomDepth(bHighlighted);
    HookTop->SetRenderCustomDepth(bHighlighted);
    ClothingPreviewMesh->SetRenderCustomDepth(bHighlighted);
    ClothingMesh->SetRenderCustomDepth(bHighlighted);
}

void AMariaHangerActor::Interact_Implementation(AActor* Interactor)
{
    if (!bOccupied)
    {
        return;
    }

    if (const AMariaWardrobeActor* WardrobeActor = Cast<AMariaWardrobeActor>(GetOwner()))
    {
        if (!WardrobeActor->bDoorsOpen)
        {
            return;
        }
    }

    AMariaCharacter* Maria = Cast<AMariaCharacter>(Interactor);

    if (!Maria || !Maria->Wardrobe)
    {
        return;
    }

    auto RestoreItemToHanger = [this](FName ItemId)
    {
        if (ItemId.IsNone() || !GetWorld())
        {
            return;
        }

        for (TActorIterator<AMariaHangerActor> It(GetWorld()); It; ++It)
        {
            AMariaHangerActor* Hanger = *It;

            if (Hanger && Hanger->ClothingItem.ItemId == ItemId)
            {
                Hanger->SetOccupied(true);
                break;
            }
        }
    };

    auto RestoreEquippedSlot = [&](EMariaClothingSlot Slot)
    {
        if (const FName* ExistingItemId = Maria->Wardrobe->EquippedItems.Find(Slot))
        {
            RestoreItemToHanger(*ExistingItemId);
        }
    };

    if (ClothingItem.Slot == EMariaClothingSlot::Dress)
    {
        RestoreEquippedSlot(EMariaClothingSlot::UpperBody);
        RestoreEquippedSlot(EMariaClothingSlot::LowerBody);
        RestoreEquippedSlot(EMariaClothingSlot::Dress);
    }
    else if (ClothingItem.Slot == EMariaClothingSlot::UpperBody ||
             ClothingItem.Slot == EMariaClothingSlot::LowerBody)
    {
        RestoreEquippedSlot(ClothingItem.Slot);
        RestoreEquippedSlot(EMariaClothingSlot::Dress);
    }
    else
    {
        RestoreEquippedSlot(ClothingItem.Slot);
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
