#pragma once

#include "CoreMinimal.h"
#include "Character/MariaCharacter.h"
#include "MariaPrototypeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class MARIAGAME_API AMariaPrototypeCharacter : public AMariaCharacter
{
    GENERATED_BODY()

public:
    AMariaPrototypeCharacter();

protected:
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaSeconds) override;

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(EditAnywhere, Category="Interaction")
    float InteractionDistance = 350.0f;

    UPROPERTY(Transient)
    TScriptInterface<class IMariaInteractable> FocusedInteractable;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void Interact();
    void UpdateInteractionFocus();
};
