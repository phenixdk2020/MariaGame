#pragma once

#include "CoreMinimal.h"
#include "MariaClothingTypes.generated.h"

class USkeletalMesh;
class UTexture2D;

UENUM(BlueprintType)
enum class EMariaClothingSlot : uint8
{
    Hair,
    Headwear,
    UpperBody,
    LowerBody,
    Dress,
    Jacket,
    Shoes,
    Accessory
};

USTRUCT(BlueprintType)
struct FMariaClothingItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMariaClothingSlot Slot = EMariaClothingSlot::UpperBody;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USkeletalMesh> SkeletalMesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTexture2D> Thumbnail = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Prototype")
    FLinearColor PreviewColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Prototype")
    FVector PreviewScale = FVector(0.35f, 0.22f, 0.45f);
};
