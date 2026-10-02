#include "MariaPrototypeCharacter.h"
#include "Interaction/MariaInteractable.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

AMariaPrototypeCharacter::AMariaPrototypeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 320.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

    DummyHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyHead"));
    DummyHead->SetupAttachment(RootComponent);
    DummyHead->SetRelativeLocation(FVector(0.0f, 0.0f, 72.0f));
    DummyHead->SetRelativeScale3D(FVector(0.22f));
    if (SphereMesh.Succeeded()) DummyHead->SetStaticMesh(SphereMesh.Object);

    DummyTorso = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyTorso"));
    DummyTorso->SetupAttachment(RootComponent);
    DummyTorso->SetRelativeLocation(FVector(0.0f, 0.0f, 25.0f));
    DummyTorso->SetRelativeScale3D(FVector(0.34f, 0.20f, 0.52f));
    if (CubeMesh.Succeeded()) DummyTorso->SetStaticMesh(CubeMesh.Object);

    DummyLeftArm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyLeftArm"));
    DummyLeftArm->SetupAttachment(RootComponent);
    DummyLeftArm->SetRelativeLocation(FVector(0.0f, -30.0f, 28.0f));
    DummyLeftArm->SetRelativeScale3D(FVector(0.10f, 0.10f, 0.50f));
    if (CubeMesh.Succeeded()) DummyLeftArm->SetStaticMesh(CubeMesh.Object);

    DummyRightArm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyRightArm"));
    DummyRightArm->SetupAttachment(RootComponent);
    DummyRightArm->SetRelativeLocation(FVector(0.0f, 30.0f, 28.0f));
    DummyRightArm->SetRelativeScale3D(FVector(0.10f, 0.10f, 0.50f));
    if (CubeMesh.Succeeded()) DummyRightArm->SetStaticMesh(CubeMesh.Object);

    DummyLeftLeg = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyLeftLeg"));
    DummyLeftLeg->SetupAttachment(RootComponent);
    DummyLeftLeg->SetRelativeLocation(FVector(0.0f, -13.0f, -38.0f));
    DummyLeftLeg->SetRelativeScale3D(FVector(0.14f, 0.14f, 0.58f));
    if (CubeMesh.Succeeded()) DummyLeftLeg->SetStaticMesh(CubeMesh.Object);

    DummyRightLeg = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyRightLeg"));
    DummyRightLeg->SetupAttachment(RootComponent);
    DummyRightLeg->SetRelativeLocation(FVector(0.0f, 13.0f, -38.0f));
    DummyRightLeg->SetRelativeScale3D(FVector(0.14f, 0.14f, 0.58f));
    if (CubeMesh.Succeeded()) DummyRightLeg->SetStaticMesh(CubeMesh.Object);

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

    ConfigurePrototypeClothing(PrototypeUpperBody, FVector(-1.0f, 0.0f, 28.0f), FVector(0.37f, 0.23f, 0.32f));
    ConfigurePrototypeClothing(PrototypeLowerBody, FVector(-1.0f, 0.0f, -24.0f), FVector(0.31f, 0.22f, 0.40f));
    ConfigurePrototypeClothing(PrototypeDress, FVector(-2.0f, 0.0f, 5.0f), FVector(0.39f, 0.25f, 0.65f));
    ConfigurePrototypeClothing(PrototypeJacket, FVector(-3.0f, 0.0f, 30.0f), FVector(0.41f, 0.27f, 0.36f));
    ConfigurePrototypeClothing(PrototypeShoes, FVector(0.0f, 0.0f, -82.0f), FVector(0.28f, 0.30f, 0.10f));

    for (UStaticMeshComponent* Part : {DummyHead.Get(), DummyTorso.Get(), DummyLeftArm.Get(), DummyRightArm.Get(), DummyLeftLeg.Get(), DummyRightLeg.Get()})
    {
        Part->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
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
}

void AMariaPrototypeCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateInteractionFocus();
}

void AMariaPrototypeCharacter::MoveForward(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value)) return;
    const FRotator Rotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(Rotation).GetUnitAxis(EAxis::X), Value);
}

void AMariaPrototypeCharacter::MoveRight(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value)) return;
    const FRotator Rotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y), Value);
}

void AMariaPrototypeCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void AMariaPrototypeCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void AMariaPrototypeCharacter::Interact()
{
    if (FocusedInteractable.GetObject())
    {
        IMariaInteractable::Execute_Interact(FocusedInteractable.GetObject(), this);
    }
}

void AMariaPrototypeCharacter::UpdateInteractionFocus()
{
    if (!FollowCamera || !GetWorld()) return;

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
