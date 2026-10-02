#include "MariaPrototypeCharacter.h"
#include "Wardrobe/MariaWardrobeComponent.h"
#include "Wardrobe/MariaHangerActor.h"
#include "Wardrobe/MariaWardrobeActor.h"
#include "Save/MariaOutfitSaveGame.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AMariaPrototypeCharacter::AMariaPrototypeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 430.0f;
    CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 55.0f);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> BasicMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

    DummyHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyHead"));
    DummyNeck = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyNeck"));
    DummyTorso = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyTorso"));
    DummyLeftShoulder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyLeftShoulder"));
    DummyRightShoulder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyRightShoulder"));
    DummyLeftArm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyLeftArm"));
    DummyRightArm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyRightArm"));
    DummyLeftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyLeftHand"));
    DummyRightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyRightHand"));
    DummyLeftLeg = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyLeftLeg"));
    DummyRightLeg = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyRightLeg"));
    DummyLeftFoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyLeftFoot"));
    DummyRightFoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyRightFoot"));

    ConfigureBodyPart(DummyHead, SphereMesh.Object, FVector(0.0f, 0.0f, 83.0f), FVector(0.20f, 0.18f, 0.23f));
    ConfigureBodyPart(DummyNeck, CubeMesh.Object, FVector(0.0f, 0.0f, 62.0f), FVector(0.09f, 0.09f, 0.12f));
    ConfigureBodyPart(DummyTorso, CubeMesh.Object, FVector(0.0f, 0.0f, 29.0f), FVector(0.28f, 0.18f, 0.40f));
    ConfigureBodyPart(DummyLeftShoulder, SphereMesh.Object, FVector(0.0f, -25.0f, 48.0f), FVector(0.11f));
    ConfigureBodyPart(DummyRightShoulder, SphereMesh.Object, FVector(0.0f, 25.0f, 48.0f), FVector(0.11f));
    ConfigureBodyPart(DummyLeftArm, CubeMesh.Object, FVector(0.0f, -28.0f, 17.0f), FVector(0.085f, 0.085f, 0.34f));
    ConfigureBodyPart(DummyRightArm, CubeMesh.Object, FVector(0.0f, 28.0f, 17.0f), FVector(0.085f, 0.085f, 0.34f));
    ConfigureBodyPart(DummyLeftHand, SphereMesh.Object, FVector(0.0f, -28.0f, -18.0f), FVector(0.08f, 0.065f, 0.11f));
    ConfigureBodyPart(DummyRightHand, SphereMesh.Object, FVector(0.0f, 28.0f, -18.0f), FVector(0.08f, 0.065f, 0.11f));
    ConfigureBodyPart(DummyLeftLeg, CubeMesh.Object, FVector(0.0f, -11.0f, -42.0f), FVector(0.105f, 0.11f, 0.48f));
    ConfigureBodyPart(DummyRightLeg, CubeMesh.Object, FVector(0.0f, 11.0f, -42.0f), FVector(0.105f, 0.11f, 0.48f));
    ConfigureBodyPart(DummyLeftFoot, CubeMesh.Object, FVector(9.0f, -11.0f, -91.0f), FVector(0.19f, 0.11f, 0.07f));
    ConfigureBodyPart(DummyRightFoot, CubeMesh.Object, FVector(9.0f, 11.0f, -91.0f), FVector(0.19f, 0.11f, 0.07f));

    BaseUnderwearTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseUnderwearTop"));
    BaseUnderwearBottom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseUnderwearBottom"));
    PrototypeHair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypeHair"));

    ConfigurePrototypeClothing(BaseUnderwearTop, FVector(-1.0f, 0.0f, 28.0f), FVector(0.295f, 0.195f, 0.18f));
    ConfigurePrototypeClothing(BaseUnderwearBottom, FVector(-1.0f, 0.0f, -5.0f), FVector(0.24f, 0.18f, 0.12f));
    ConfigurePrototypeClothing(PrototypeHair, FVector(-3.0f, 0.0f, 87.0f), FVector(0.22f, 0.20f, 0.20f));

    if (CubeMesh.Succeeded())
    {
        BaseUnderwearTop->SetStaticMesh(CubeMesh.Object);
        BaseUnderwearBottom->SetStaticMesh(CubeMesh.Object);
    }

    if (SphereMesh.Succeeded())
    {
        PrototypeHair->SetStaticMesh(SphereMesh.Object);
    }

    if (BasicMaterial.Succeeded())
    {
        PrototypeHair->SetMaterial(0, BasicMaterial.Object);
    }

    BaseUnderwearTop->SetVisibility(true, true);
    BaseUnderwearBottom->SetVisibility(true, true);
    PrototypeHair->SetVisibility(true, true);

    PrototypeUpperBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypeUpperBody"));
    PrototypeLowerBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypeLowerBody"));
    PrototypeDress = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypeDress"));
    PrototypeJacket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypeJacket"));
    PrototypeShoes = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypeShoes"));

    if (CubeMesh.Succeeded())
    {
        PrototypeUpperBody->SetStaticMesh(CubeMesh.Object);
        PrototypeLowerBody->SetStaticMesh(CubeMesh.Object);
        PrototypeDress->SetStaticMesh(CubeMesh.Object);
        PrototypeJacket->SetStaticMesh(CubeMesh.Object);
        PrototypeShoes->SetStaticMesh(CubeMesh.Object);
    }

    ConfigurePrototypeClothing(PrototypeUpperBody, FVector(-1.0f, 0.0f, 30.0f), FVector(0.31f, 0.205f, 0.31f));
    ConfigurePrototypeClothing(PrototypeLowerBody, FVector(-1.0f, 0.0f, -38.0f), FVector(0.24f, 0.19f, 0.42f));
    ConfigurePrototypeClothing(PrototypeDress, FVector(-2.0f, 0.0f, 2.0f), FVector(0.34f, 0.22f, 0.70f));
    ConfigurePrototypeClothing(PrototypeJacket, FVector(-3.0f, 0.0f, 31.0f), FVector(0.35f, 0.24f, 0.38f));
    ConfigurePrototypeClothing(PrototypeShoes, FVector(8.0f, 0.0f, -91.0f), FVector(0.23f, 0.24f, 0.09f));

    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMariaPrototypeCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (PrototypeHair && PrototypeHair->GetNumMaterials() > 0)
    {
        PrototypeHairMaterial = PrototypeHair->CreateDynamicMaterialInstance(0);
    }

    HairBrown();
}

void AMariaPrototypeCharacter::ConfigureBodyPart(UStaticMeshComponent* Component, UStaticMesh* StaticMesh, const FVector& Location, const FVector& Scale)
{
    Component->SetupAttachment(RootComponent);
    Component->SetStaticMesh(StaticMesh);
    Component->SetRelativeLocation(Location);
    Component->SetRelativeScale3D(Scale);
    Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMariaPrototypeCharacter::ConfigurePrototypeClothing(UStaticMeshComponent* Component, const FVector& Location, const FVector& Scale)
{
    Component->SetupAttachment(RootComponent);
    Component->SetRelativeLocation(Location);
    Component->SetRelativeScale3D(Scale);
    Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Component->SetVisibility(false, true);
}

UStaticMeshComponent* AMariaPrototypeCharacter::GetPrototypeClothingComponent(EMariaClothingSlot Slot) const
{
    switch (Slot)
    {
        case EMariaClothingSlot::Hair: return PrototypeHair;
        case EMariaClothingSlot::UpperBody: return PrototypeUpperBody;
        case EMariaClothingSlot::LowerBody: return PrototypeLowerBody;
        case EMariaClothingSlot::Dress: return PrototypeDress;
        case EMariaClothingSlot::Jacket: return PrototypeJacket;
        case EMariaClothingSlot::Shoes: return PrototypeShoes;
        default: return nullptr;
    }
}

bool AMariaPrototypeCharacter::WearItem(const FMariaClothingItem& Item)
{
    const bool bBaseResult = Super::WearItem(Item);
    if (!bBaseResult)
    {
        return false;
    }

    if (Item.Slot == EMariaClothingSlot::Dress)
    {
        PrototypeUpperBody->SetVisibility(false, true);
        PrototypeLowerBody->SetVisibility(false, true);
    }
    else if (Item.Slot == EMariaClothingSlot::UpperBody || Item.Slot == EMariaClothingSlot::LowerBody)
    {
        PrototypeDress->SetVisibility(false, true);
    }

    if (UStaticMeshComponent* Component = GetPrototypeClothingComponent(Item.Slot))
    {
        Component->SetRelativeScale3D(Item.PreviewScale);
        Component->SetVisibility(true, true);
    }

    return true;
}

void AMariaPrototypeCharacter::RemoveItem(EMariaClothingSlot Slot)
{
    Super::RemoveItem(Slot);

    if (UStaticMeshComponent* Component = GetPrototypeClothingComponent(Slot))
    {
        Component->SetVisibility(false, true);
    }

    if (Slot == EMariaClothingSlot::Hair)
    {
        PrototypeHair->SetVisibility(true, true);
    }

    if (Slot == EMariaClothingSlot::Dress)
    {
        if (Wardrobe && Wardrobe->EquippedItems.Contains(EMariaClothingSlot::UpperBody))
        {
            PrototypeUpperBody->SetVisibility(true, true);
        }

        if (Wardrobe && Wardrobe->EquippedItems.Contains(EMariaClothingSlot::LowerBody))
        {
            PrototypeLowerBody->SetVisibility(true, true);
        }
    }
}

void AMariaPrototypeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMariaPrototypeCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMariaPrototypeCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMariaPrototypeCharacter::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMariaPrototypeCharacter::LookUp);

    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AMariaPrototypeCharacter::Interact);
    PlayerInputComponent->BindAction(TEXT("Preview"), IE_Pressed, this, &AMariaPrototypeCharacter::TogglePreviewMode);
    PlayerInputComponent->BindAction(TEXT("SaveOutfit"), IE_Pressed, this, &AMariaPrototypeCharacter::SaveOutfit);
    PlayerInputComponent->BindAction(TEXT("LoadOutfit"), IE_Pressed, this, &AMariaPrototypeCharacter::LoadOutfit);
    PlayerInputComponent->BindAction(TEXT("HairBlonde"), IE_Pressed, this, &AMariaPrototypeCharacter::HairBlonde);
    PlayerInputComponent->BindAction(TEXT("HairBrown"), IE_Pressed, this, &AMariaPrototypeCharacter::HairBrown);
    PlayerInputComponent->BindAction(TEXT("HairBlack"), IE_Pressed, this, &AMariaPrototypeCharacter::HairBlack);
}

void AMariaPrototypeCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bPreviewMode)
    {
        UpdateInteractionFocus();
    }
}

void AMariaPrototypeCharacter::MoveForward(float Value)
{
    if (bPreviewMode || !Controller || FMath::IsNearlyZero(Value))
    {
        return;
    }

    const FRotator Rotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(Rotation).GetUnitAxis(EAxis::X), Value);
}

void AMariaPrototypeCharacter::MoveRight(float Value)
{
    if (bPreviewMode || !Controller || FMath::IsNearlyZero(Value))
    {
        return;
    }

    const FRotator Rotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y), Value);
}

void AMariaPrototypeCharacter::Turn(float Value)
{
    if (bPreviewMode)
    {
        AddActorLocalRotation(FRotator(0.0f, Value * 2.0f, 0.0f));
        return;
    }

    AddControllerYawInput(Value);
}

void AMariaPrototypeCharacter::LookUp(float Value)
{
    if (!bPreviewMode)
    {
        AddControllerPitchInput(Value);
    }
}

void AMariaPrototypeCharacter::Interact()
{
    if (!bPreviewMode && FocusedInteractable.GetObject())
    {
        IMariaInteractable::Execute_Interact(FocusedInteractable.GetObject(), this);
    }
}

void AMariaPrototypeCharacter::TogglePreviewMode()
{
    bPreviewMode = !bPreviewMode;
    CameraBoom->TargetArmLength = bPreviewMode ? 520.0f : 430.0f;

    if (bPreviewMode && FocusedInteractable.GetObject())
    {
        IMariaInteractable::Execute_SetFocused(FocusedInteractable.GetObject(), false);
        FocusedInteractable.SetObject(nullptr);
        FocusedInteractable.SetInterface(nullptr);
    }
}

void AMariaPrototypeCharacter::SaveOutfit()
{
    if (!Wardrobe)
    {
        return;
    }

    UMariaOutfitSaveGame* SaveGame = Cast<UMariaOutfitSaveGame>(
        UGameplayStatics::CreateSaveGameObject(UMariaOutfitSaveGame::StaticClass()));

    if (!SaveGame)
    {
        return;
    }

    SaveGame->EquippedItems = Wardrobe->EquippedItems;
    UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("MariaOutfit"), 0);
}

void AMariaPrototypeCharacter::LoadOutfit()
{
    if (!Wardrobe)
    {
        return;
    }

    UMariaOutfitSaveGame* SaveGame = Cast<UMariaOutfitSaveGame>(
        UGameplayStatics::LoadGameFromSlot(TEXT("MariaOutfit"), 0));

    if (!SaveGame)
    {
        return;
    }

    for (TActorIterator<AMariaHangerActor> It(GetWorld()); It; ++It)
    {
        It->SetOccupied(true);
    }

    RemoveItem(EMariaClothingSlot::UpperBody);
    RemoveItem(EMariaClothingSlot::LowerBody);
    RemoveItem(EMariaClothingSlot::Dress);
    RemoveItem(EMariaClothingSlot::Jacket);
    RemoveItem(EMariaClothingSlot::Shoes);

    for (const TPair<EMariaClothingSlot, FName>& Pair : SaveGame->EquippedItems)
    {
        for (TActorIterator<AMariaHangerActor> It(GetWorld()); It; ++It)
        {
            AMariaHangerActor* Hanger = *It;

            if (Hanger && Hanger->ClothingItem.ItemId == Pair.Value)
            {
                if (WearItem(Hanger->ClothingItem))
                {
                    Hanger->SetOccupied(false);
                }

                break;
            }
        }
    }
}

void AMariaPrototypeCharacter::HairBlonde()
{
    SetPrototypeHairColor(FLinearColor(0.72f, 0.52f, 0.24f, 1.0f), TEXT("Blond"));
}

void AMariaPrototypeCharacter::HairBrown()
{
    SetPrototypeHairColor(FLinearColor(0.16f, 0.055f, 0.02f, 1.0f), TEXT("Brun"));
}

void AMariaPrototypeCharacter::HairBlack()
{
    SetPrototypeHairColor(FLinearColor(0.012f, 0.008f, 0.006f, 1.0f), TEXT("Sort"));
}

void AMariaPrototypeCharacter::SetPrototypeHairColor(const FLinearColor& Color, const FString& PresetName)
{
    HairPresetName = PresetName;

    if (!PrototypeHairMaterial && PrototypeHair && PrototypeHair->GetNumMaterials() > 0)
    {
        PrototypeHairMaterial = PrototypeHair->CreateDynamicMaterialInstance(0);
    }

    if (PrototypeHairMaterial)
    {
        PrototypeHairMaterial->SetVectorParameterValue(TEXT("Color"), Color);
        PrototypeHairMaterial->SetVectorParameterValue(TEXT("BaseColor"), Color);
    }
}

FString AMariaPrototypeCharacter::ClothingSlotToString(EMariaClothingSlot Slot) const
{
    switch (Slot)
    {
        case EMariaClothingSlot::Hair: return TEXT("Hår");
        case EMariaClothingSlot::Headwear: return TEXT("Hovedbeklædning");
        case EMariaClothingSlot::UpperBody: return TEXT("Overdel");
        case EMariaClothingSlot::LowerBody: return TEXT("Underdel");
        case EMariaClothingSlot::Dress: return TEXT("Kjole");
        case EMariaClothingSlot::Jacket: return TEXT("Jakke");
        case EMariaClothingSlot::Shoes: return TEXT("Sko");
        case EMariaClothingSlot::Accessory: return TEXT("Tilbehør");
        default: return TEXT("Ukendt");
    }
}

FString AMariaPrototypeCharacter::GetInteractionPrompt() const
{
    UObject* Focused = FocusedInteractable.GetObject();

    if (!Focused)
    {
        return bPreviewMode ? TEXT("P - Forlad preview") : FString();
    }

    if (const AMariaWardrobeActor* WardrobeActor = Cast<AMariaWardrobeActor>(Focused))
    {
        return WardrobeActor->bDoorsOpen ? TEXT("E - Luk garderobe") : TEXT("E - Åbn garderobe");
    }

    if (const AMariaHangerActor* Hanger = Cast<AMariaHangerActor>(Focused))
    {
        if (const AMariaWardrobeActor* WardrobeActor = Cast<AMariaWardrobeActor>(Hanger->GetOwner()))
        {
            if (!WardrobeActor->bDoorsOpen)
            {
                return TEXT("Åbn garderoben først");
            }
        }

        if (!Hanger->bOccupied)
        {
            return TEXT("Tom bøjle");
        }

        return FString::Printf(TEXT("E - Tag %s på"), *Hanger->ClothingItem.DisplayName.ToString());
    }

    return TEXT("E - Interager");
}

FString AMariaPrototypeCharacter::GetFocusedItemText() const
{
    UObject* Focused = FocusedInteractable.GetObject();

    if (!Focused)
    {
        return FString();
    }

    if (Cast<AMariaWardrobeActor>(Focused))
    {
        return TEXT("Garderobe");
    }

    if (const AMariaHangerActor* Hanger = Cast<AMariaHangerActor>(Focused))
    {
        if (!Hanger->bOccupied)
        {
            return TEXT("Tom bøjle");
        }

        return FString::Printf(
            TEXT("%s  |  %s"),
            *Hanger->ClothingItem.DisplayName.ToString(),
            *ClothingSlotToString(Hanger->ClothingItem.Slot));
    }

    return Focused->GetName();
}

void AMariaPrototypeCharacter::UpdateInteractionFocus()
{
    if (!FollowCamera || !GetWorld())
    {
        return;
    }

    const FVector Start = FollowCamera->GetComponentLocation();
    const FVector End = Start + FollowCamera->GetForwardVector() * InteractionDistance;

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(MariaInteraction), false, this);
    GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    UObject* NewObject = Hit.GetActor();

    if (!NewObject || !NewObject->GetClass()->ImplementsInterface(UMariaInteractable::StaticClass()))
    {
        NewObject = nullptr;
    }

    if (FocusedInteractable.GetObject() == NewObject)
    {
        return;
    }

    if (FocusedInteractable.GetObject())
    {
        IMariaInteractable::Execute_SetFocused(FocusedInteractable.GetObject(), false);
    }

    FocusedInteractable.SetObject(NewObject);
    FocusedInteractable.SetInterface(NewObject ? Cast<IMariaInteractable>(NewObject) : nullptr);

    if (NewObject)
    {
        IMariaInteractable::Execute_SetFocused(NewObject, true);
    }
}
