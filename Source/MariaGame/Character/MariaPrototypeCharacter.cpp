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

    for (UStaticMeshComponent* Part : {DummyHead.Get(), DummyTorso.Get(), DummyLeftArm.Get(), DummyRightArm.Get(), DummyLeftLeg.Get(), DummyRightLeg.Get()})
    {
        Part->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
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
