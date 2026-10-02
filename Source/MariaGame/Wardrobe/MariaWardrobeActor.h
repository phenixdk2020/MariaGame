#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/MariaInteractable.h"
#include "MariaWardrobeActor.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class AMariaHangerActor;

UCLASS()
class MARIAGAME_API AMariaWardrobeActor : public AActor, public IMariaInteractable
{
    GENERATED_BODY()

public:
    AMariaWardrobeActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<USceneComponent> Root;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> CabinetBack;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> LeftSide;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> RightSide;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> TopPanel;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> BottomPanel;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> UpperShelf;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> ShoeShelf;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<USceneComponent> LeftDoorHinge;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<USceneComponent> RightDoorHinge;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> LeftDoor;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> RightDoor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<USceneComponent> HangerRail;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wardrobe")
    TSubclassOf<AMariaHangerActor> HangerClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wardrobe", meta=(ClampMin="1"))
    int32 HangerCount = 7;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wardrobe")
    float HangerSpacing = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wardrobe")
    float DoorAnimationSpeed = 3.5f;

    UPROPERTY(BlueprintReadOnly, Category="Wardrobe")
    bool bDoorsOpen = false;

    UFUNCTION(BlueprintCallable, Category="Wardrobe")
    void BuildHangers();

    UFUNCTION(BlueprintCallable, Category="Wardrobe")
    void SetDoorsOpen(bool bOpen);

    virtual void Interact_Implementation(AActor* Interactor) override;
    virtual void SetFocused_Implementation(bool bFocused) override;

protected:
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void Tick(float DeltaSeconds) override;

private:
    UPROPERTY(Transient)
    TArray<TObjectPtr<AMariaHangerActor>> SpawnedHangers;

    float DoorAlpha = 0.0f;
    float TargetDoorAlpha = 0.0f;

    void ClearHangers();
    void ApplyDoorPose();
};
