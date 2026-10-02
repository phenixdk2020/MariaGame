#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MariaHairStyleDataAsset.generated.h"

class USkeletalMesh;
class UStaticMesh;

UCLASS(BlueprintType)
class MARIAGAME_API UMariaHairStyleDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hair")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hair")
    TObjectPtr<USkeletalMesh> SkeletalMesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hair")
    TObjectPtr<UStaticMesh> StaticPreviewMesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hair")
    FLinearColor DefaultColor = FLinearColor(0.16f, 0.055f, 0.02f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hair")
    bool bSupportsPhysics = false;
};
