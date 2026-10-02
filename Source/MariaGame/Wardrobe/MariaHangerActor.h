#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MariaClothingTypes.h"
#include "MariaHangerActor.generated.h"

class UStaticMeshComponent;
class USkeletalMeshComponent;

UCLASS()
class MARIAGAME_API AMariaHangerActor : public AActor
{
    GENERATED_BODY()

public:
    AMariaHangerActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> HangerMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USkeletalMeshComponent> ClothingMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wardrobe")
    FMariaClothingItem ClothingItem;

    UFUNCTION(BlueprintCallable, Category="Wardrobe")
    void ApplyClothingItem();

    UFUNCTION(BlueprintCallable, Category="Wardrobe")
    void SetHighlighted(bool bHighlighted);
};
