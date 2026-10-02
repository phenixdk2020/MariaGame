#include "MariaWardrobeActor.h"
#include "MariaHangerActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

AMariaWardrobeActor::AMariaWardrobeActor()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));

    auto SetupBox = [&](UStaticMeshComponent* Component, const FVector& Location, const FVector& Scale)
    {
        Component->SetupAttachment(Root);
        Component->SetRelativeLocation(Location);
        Component->SetRelativeScale3D(Scale);
        if (CubeMesh.Succeeded())
        {
            Component->SetStaticMesh(CubeMesh.Object);
        }
    };

    CabinetBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CabinetBack"));
    LeftSide = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftSide"));
    RightSide = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSide"));
    TopPanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopPanel"));
    BottomPanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomPanel"));
    UpperShelf = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpperShelf"));
    ShoeShelf = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShoeShelf"));

    SetupBox(CabinetBack, FVector(18.0f, 0.0f, 105.0f), FVector(0.06f, 1.45f, 2.10f));
    SetupBox(LeftSide, FVector(-5.0f, -145.0f, 105.0f), FVector(0.46f, 0.06f, 2.10f));
    SetupBox(RightSide, FVector(-5.0f, 145.0f, 105.0f), FVector(0.46f, 0.06f, 2.10f));
    SetupBox(TopPanel, FVector(-5.0f, 0.0f, 310.0f), FVector(0.46f, 1.45f, 0.06f));
    SetupBox(BottomPanel, FVector(-5.0f, 0.0f, -100.0f), FVector(0.46f, 1.45f, 0.06f));
    SetupBox(UpperShelf, FVector(-5.0f, 0.0f, 240.0f), FVector(0.44f, 1.38f, 0.04f));
    SetupBox(ShoeShelf, FVector(-5.0f, 0.0f, -35.0f), FVector(0.44f, 1.38f, 0.04f));

    LeftDoorHinge = CreateDefaultSubobject<USceneComponent>(TEXT("LeftDoorHinge"));
    LeftDoorHinge->SetupAttachment(Root);
    LeftDoorHinge->SetRelativeLocation(FVector(-50.0f, -145.0f, 105.0f));

    RightDoorHinge = CreateDefaultSubobject<USceneComponent>(TEXT("RightDoorHinge"));
    RightDoorHinge->SetupAttachment(Root);
    RightDoorHinge->SetRelativeLocation(FVector(-50.0f, 145.0f, 105.0f));

    LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
    LeftDoor->SetupAttachment(LeftDoorHinge);
    LeftDoor->SetRelativeLocation(FVector(0.0f, 72.5f, 0.0f));
    LeftDoor->SetRelativeScale3D(FVector(0.05f, 0.725f, 2.10f));
    if (CubeMesh.Succeeded()) LeftDoor->SetStaticMesh(CubeMesh.Object);

    RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
    RightDoor->SetupAttachment(RightDoorHinge);
    RightDoor->SetRelativeLocation(FVector(0.0f, -72.5f, 0.0f));
    RightDoor->SetRelativeScale3D(FVector(0.05f, 0.725f, 2.10f));
    if (CubeMesh.Succeeded()) RightDoor->SetStaticMesh(CubeMesh.Object);

    HangerRail = CreateDefaultSubobject<USceneComponent>(TEXT("HangerRail"));
    HangerRail->SetupAttachment(Root);
    HangerRail->SetRelativeLocation(FVector(-10.0f, 0.0f, 170.0f));

    HangerRailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HangerRailMesh"));
    HangerRailMesh->SetupAttachment(Root);
    HangerRailMesh->SetRelativeLocation(FVector(-10.0f, 0.0f, 184.0f));
    HangerRailMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    HangerRailMesh->SetRelativeScale3D(FVector(0.035f, 0.035f, 2.55f));
    HangerRailMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (CylinderMesh.Succeeded())
    {
        HangerRailMesh->SetStaticMesh(CylinderMesh.Object);
    }

    HangerClass = AMariaHangerActor::StaticClass();
}

void AMariaWardrobeActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    DoorAlpha = bDoorsOpen ? 1.0f : 0.0f;
    TargetDoorAlpha = DoorAlpha;
    ApplyDoorPose();
    BuildHangers();
}

void AMariaWardrobeActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    DoorAlpha = FMath::FInterpTo(DoorAlpha, TargetDoorAlpha, DeltaSeconds, DoorAnimationSpeed);
    ApplyDoorPose();
}

void AMariaWardrobeActor::ApplyDoorPose()
{
    const float LeftYaw = FMath::Lerp(0.0f, -105.0f, DoorAlpha);
    const float RightYaw = FMath::Lerp(0.0f, 105.0f, DoorAlpha);
    LeftDoorHinge->SetRelativeRotation(FRotator(0.0f, LeftYaw, 0.0f));
    RightDoorHinge->SetRelativeRotation(FRotator(0.0f, RightYaw, 0.0f));
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

        if (!Hanger)
        {
            continue;
        }

        FMariaClothingItem DemoItem;

        switch (Index % 7)
        {
            case 0:
                DemoItem.ItemId = TEXT("Prototype_TShirt");
                DemoItem.DisplayName = FText::FromString(TEXT("T-Shirt"));
                DemoItem.Slot = EMariaClothingSlot::UpperBody;
                DemoItem.PreviewScale = FVector(0.28f, 0.24f, 0.34f);
                break;
            case 1:
                DemoItem.ItemId = TEXT("Prototype_Blouse");
                DemoItem.DisplayName = FText::FromString(TEXT("Bluse"));
                DemoItem.Slot = EMariaClothingSlot::UpperBody;
                DemoItem.PreviewScale = FVector(0.31f, 0.25f, 0.38f);
                break;
            case 2:
                DemoItem.ItemId = TEXT("Prototype_Sweater");
                DemoItem.DisplayName = FText::FromString(TEXT("Sweater"));
                DemoItem.Slot = EMariaClothingSlot::UpperBody;
                DemoItem.PreviewScale = FVector(0.35f, 0.28f, 0.42f);
                break;
            case 3:
                DemoItem.ItemId = TEXT("Prototype_Trousers");
                DemoItem.DisplayName = FText::FromString(TEXT("Bukser"));
                DemoItem.Slot = EMariaClothingSlot::LowerBody;
                DemoItem.PreviewScale = FVector(0.24f, 0.20f, 0.55f);
                break;
            case 4:
                DemoItem.ItemId = TEXT("Prototype_Dress");
                DemoItem.DisplayName = FText::FromString(TEXT("Kjole"));
                DemoItem.Slot = EMariaClothingSlot::Dress;
                DemoItem.PreviewScale = FVector(0.34f, 0.27f, 0.72f);
                break;
            case 5:
                DemoItem.ItemId = TEXT("Prototype_Jacket");
                DemoItem.DisplayName = FText::FromString(TEXT("Jakke"));
                DemoItem.Slot = EMariaClothingSlot::Jacket;
                DemoItem.PreviewScale = FVector(0.39f, 0.30f, 0.48f);
                break;
            default:
                DemoItem.ItemId = TEXT("Prototype_Shoes");
                DemoItem.DisplayName = FText::FromString(TEXT("Sko"));
                DemoItem.Slot = EMariaClothingSlot::Shoes;
                DemoItem.PreviewScale = FVector(0.26f, 0.24f, 0.12f);
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

void AMariaWardrobeActor::SetDoorsOpen(bool bOpen)
{
    bDoorsOpen = bOpen;
    TargetDoorAlpha = bOpen ? 1.0f : 0.0f;
}

void AMariaWardrobeActor::Interact_Implementation(AActor* Interactor)
{
    SetDoorsOpen(!bDoorsOpen);
}

void AMariaWardrobeActor::SetFocused_Implementation(bool bFocused)
{
    CabinetBack->SetRenderCustomDepth(bFocused);
    LeftDoor->SetRenderCustomDepth(bFocused);
    RightDoor->SetRenderCustomDepth(bFocused);
}
