#include "MariaWardrobeActor.h"
#include "MariaHangerActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

AMariaWardrobeActor::AMariaWardrobeActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));

    CabinetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CabinetBack"));
    CabinetMesh->SetupAttachment(Root);
    CabinetMesh->SetRelativeLocation(FVector(20.0f, 0.0f, 110.0f));
    CabinetMesh->SetRelativeScale3D(FVector(0.08f, 1.3f, 2.2f));
    if (CubeMesh.Succeeded())
    {
        CabinetMesh->SetStaticMesh(CubeMesh.Object);
    }

    LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
    LeftDoor->SetupAttachment(Root);
    LeftDoor->SetRelativeLocation(FVector(-35.0f, -65.0f, 110.0f));
    LeftDoor->SetRelativeScale3D(FVector(0.05f, 0.65f, 2.2f));
    if (CubeMesh.Succeeded())
    {
        LeftDoor->SetStaticMesh(CubeMesh.Object);
    }

    RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
    RightDoor->SetupAttachment(Root);
    RightDoor->SetRelativeLocation(FVector(-35.0f, 65.0f, 110.0f));
    RightDoor->SetRelativeScale3D(FVector(0.05f, 0.65f, 2.2f));
    if (CubeMesh.Succeeded())
    {
        RightDoor->SetStaticMesh(CubeMesh.Object);
    }

    HangerRail = CreateDefaultSubobject<USceneComponent>(TEXT("HangerRail"));
    HangerRail->SetupAttachment(Root);
    HangerRail->SetRelativeLocation(FVector(-5.0f, 0.0f, 175.0f));

    HangerClass = AMariaHangerActor::StaticClass();
}

void AMariaWardrobeActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    SetDoorsOpen(bDoorsOpen);
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
        const FVector LocalOffset(-10.0f, StartY + Index * HangerSpacing, 0.0f);
        const FVector WorldLocation = HangerRail->GetComponentTransform().TransformPosition(LocalOffset);
        const FTransform SpawnTransform(HangerRail->GetComponentRotation(), WorldLocation);

        AMariaHangerActor* Hanger = GetWorld()->SpawnActorDeferred<AMariaHangerActor>(
            HangerClass,
            SpawnTransform,
            this,
            nullptr,
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        if (Hanger)
        {
            FMariaClothingItem DemoItem;

            switch (Index % 5)
            {
                case 0:
                    DemoItem.ItemId = TEXT("Prototype_TShirt");
                    DemoItem.DisplayName = FText::FromString(TEXT("T-Shirt"));
                    DemoItem.Slot = EMariaClothingSlot::UpperBody;
                    DemoItem.PreviewScale = FVector(0.30f, 0.26f, 0.42f);
                    break;

                case 1:
                    DemoItem.ItemId = TEXT("Prototype_Sweater");
                    DemoItem.DisplayName = FText::FromString(TEXT("Sweater"));
                    DemoItem.Slot = EMariaClothingSlot::UpperBody;
                    DemoItem.PreviewScale = FVector(0.36f, 0.29f, 0.47f);
                    break;

                case 2:
                    DemoItem.ItemId = TEXT("Prototype_Dress");
                    DemoItem.DisplayName = FText::FromString(TEXT("Dress"));
                    DemoItem.Slot = EMariaClothingSlot::Dress;
                    DemoItem.PreviewScale = FVector(0.35f, 0.30f, 0.72f);
                    break;

                case 3:
                    DemoItem.ItemId = TEXT("Prototype_Jacket");
                    DemoItem.DisplayName = FText::FromString(TEXT("Jacket"));
                    DemoItem.Slot = EMariaClothingSlot::Jacket;
                    DemoItem.PreviewScale = FVector(0.40f, 0.31f, 0.50f);
                    break;

                default:
                    DemoItem.ItemId = TEXT("Prototype_Blouse");
                    DemoItem.DisplayName = FText::FromString(TEXT("Blouse"));
                    DemoItem.Slot = EMariaClothingSlot::UpperBody;
                    DemoItem.PreviewScale = FVector(0.32f, 0.27f, 0.44f);
                    break;
            }

            Hanger->ClothingItem = DemoItem;
            Hanger->bOccupied = true;
            Hanger->FinishSpawning(SpawnTransform);
            Hanger->AttachToComponent(HangerRail, FAttachmentTransformRules::KeepWorldTransform);
            Hanger->ApplyClothingItem();
            SpawnedHangers.Add(Hanger);
        }
    }
}

void AMariaWardrobeActor::SetDoorsOpen(bool bOpen)
{
    bDoorsOpen = bOpen;

    LeftDoor->SetRelativeRotation(FRotator(0.0f, bOpen ? -95.0f : 0.0f, 0.0f));
    RightDoor->SetRelativeRotation(FRotator(0.0f, bOpen ? 95.0f : 0.0f, 0.0f));
}

void AMariaWardrobeActor::Interact_Implementation(AActor* Interactor)
{
    SetDoorsOpen(!bDoorsOpen);
}

void AMariaWardrobeActor::SetFocused_Implementation(bool bFocused)
{
    CabinetMesh->SetRenderCustomDepth(bFocused);
    LeftDoor->SetRenderCustomDepth(bFocused);
    RightDoor->SetRenderCustomDepth(bFocused);
}
