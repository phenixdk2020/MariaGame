#include "MariaPrototypeCharacter.h"
#include "Interaction/MariaInteractable.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"

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
        NewObject = Hit.GetComponent();
    }

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
