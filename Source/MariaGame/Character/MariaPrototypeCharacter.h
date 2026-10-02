#pragma once

#include "CoreMinimal.h"
#include "Character/MariaCharacter.h"
#include "MariaPrototypeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;

UCLASS()
class MARIAGAME_API AMariaPrototypeCharacter : public AMariaCharacter
{
    GENERATED_BODY()

public:
    AMariaPrototypeCharacter();

    virtual bool WearItem(const FMariaClothingItem& Item) override;
    virtual void RemoveItem(EMariaClothingSlot Slot) override;

protected:
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaSeconds) override;

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> DummyHead;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> DummyTorso;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> DummyLeftArm;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> DummyRightArm;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> DummyLeftLeg;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> DummyRightLeg;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> PrototypeUpperBody;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> PrototypeLowerBody;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> PrototypeDress;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> PrototypeJacket;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> PrototypeShoes;

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

    UStaticMeshComponent* GetPrototypeClothingComponent(EMariaClothingSlot Slot) const;
    void ConfigurePrototypeClothing(UStaticMeshComponent* Component, const FVector& Location, const FVector& Scale);
};
