#pragma once

#include "CoreMinimal.h"
#include "Character/MariaCharacter.h"
#include "Interaction/MariaInteractable.h"
#include "MariaPrototypeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UMaterialInstanceDynamic;

UCLASS()
class MARIAGAME_API AMariaPrototypeCharacter : public AMariaCharacter
{
    GENERATED_BODY()

public:
    AMariaPrototypeCharacter();

    virtual bool WearItem(const FMariaClothingItem& Item) override;
    virtual void RemoveItem(EMariaClothingSlot Slot) override;

    FString GetInteractionPrompt() const;
    FString GetFocusedItemText() const;
    FString GetHairPresetName() const { return HairPresetName; }
    bool IsPreviewModeActive() const { return bPreviewMode; }

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaSeconds) override;

private:
    UPROPERTY(VisibleAnywhere) TObjectPtr<USpringArmComponent> CameraBoom;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyHead;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyNeck;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyTorso;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyPelvis;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyLeftShoulder;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyRightShoulder;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyLeftArm;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyRightArm;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyLeftHand;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyRightHand;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyLeftLeg;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyRightLeg;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyLeftFoot;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyRightFoot;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyLeftEye;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyRightEye;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> DummyNose;

    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> BaseUnderwearTop;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> BaseUnderwearBottom;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> PrototypeHair;

    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> PrototypeUpperBody;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> PrototypeLowerBody;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> PrototypeDress;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> PrototypeJacket;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> PrototypeShoes;

    UPROPERTY(Transient)
    TObjectPtr<UMaterialInstanceDynamic> PrototypeHairMaterial;

    UPROPERTY(EditAnywhere, Category="Interaction")
    float InteractionDistance = 420.0f;

    UPROPERTY(Transient)
    TScriptInterface<IMariaInteractable> FocusedInteractable;

    bool bPreviewMode = false;
    FString HairPresetName = TEXT("Brun");

    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void Interact();
    void UpdateInteractionFocus();
    void TogglePreviewMode();
    void SaveOutfit();
    void LoadOutfit();

    void HairBlonde();
    void HairBrown();
    void HairBlack();
    void SetPrototypeHairColor(const FLinearColor& Color, const FString& PresetName);

    FString ClothingSlotToString(EMariaClothingSlot Slot) const;

    UStaticMeshComponent* GetPrototypeClothingComponent(EMariaClothingSlot Slot) const;
    void ConfigurePrototypeClothing(UStaticMeshComponent* Component, const FVector& Location, const FVector& Scale);
    void ConfigureBodyPart(UStaticMeshComponent* Component, UStaticMesh* StaticMesh, const FVector& Location, const FVector& Scale);
    void ApplyPrototypeColor(UStaticMeshComponent* Component, const FLinearColor& Color, float Roughness = 0.55f, float Metallic = 0.0f);
};
