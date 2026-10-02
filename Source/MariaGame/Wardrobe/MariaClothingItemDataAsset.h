#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Wardrobe/MariaClothingTypes.h"
#include "MariaClothingItemDataAsset.generated.h"

UCLASS(BlueprintType)
class MARIAGAME_API UMariaClothingItemDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Clothing")
    FMariaClothingItem Item;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Metadata")
    FString Brand;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Metadata")
    TArray<FName> Tags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Metadata")
    FString SourceImportId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fit")
    float FitScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fit")
    bool bSupportsClothSimulation = false;
};
