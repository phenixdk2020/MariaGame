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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> CabinetMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> LeftDoor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> RightDoor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USceneComponent> HangerRail;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wardrobe")
    TSubclassOf<AMariaHangerActor> HangerClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wardrobe", meta=(ClampMin="1"))
    int32 HangerCount = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wardrobe")
    float HangerSpacing = 18.0f;

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

private:
    UPROPERTY(Transient)
    TArray<TObjectPtr<AMariaHangerActor>> SpawnedHangers;

    void ClearHangers();
};
