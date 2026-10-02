#pragma once

#include "CoreMinimal.h"
#include "Wardrobe/MariaClothingTypes.h"
#include "MariaClothingImportTypes.generated.h"

UENUM(BlueprintType)
enum class EMariaClothingImageView : uint8
{
    Front,
    Back,
    LeftSide,
    RightSide,
    Detail
};

UENUM(BlueprintType)
enum class EMariaClothingImportStatus : uint8
{
    WaitingForImages,
    Ready,
    Validating,
    Analyzing,
    TemplateMatch,
    MaterialBuild,
    BodyFit,
    Complete,
    Failed,
    Cancelled
};

USTRUCT(BlueprintType)
struct FMariaClothingSourceImage
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FilePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMariaClothingImageView View = EMariaClothingImageView::Front;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bValid = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString ValidationMessage;
};

USTRUCT(BlueprintType)
struct FMariaClothingImportJob
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FGuid JobId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMariaClothingSourceImage> SourceImages;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EMariaClothingImportStatus Status = EMariaClothingImportStatus::WaitingForImages;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EMariaClothingSlot SuggestedSlot = EMariaClothingSlot::UpperBody;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString StatusMessage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Progress = 0.0f;
};
