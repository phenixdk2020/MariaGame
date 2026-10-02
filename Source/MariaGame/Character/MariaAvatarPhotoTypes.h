#pragma once

#include "CoreMinimal.h"
#include "MariaAvatarPhotoTypes.generated.h"

UENUM(BlueprintType)
enum class EMariaAvatarPhotoView : uint8
{
    FaceFront,
    FaceLeft45,
    FaceRight45,
    FaceLeftProfile,
    FaceRightProfile,
    BodyFront,
    BodySide,
    BodyBack
};

USTRUCT(BlueprintType)
struct FMariaAvatarSourcePhoto
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FilePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMariaAvatarPhotoView View = EMariaAvatarPhotoView::FaceFront;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bValid = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString ValidationMessage;
};

USTRUCT(BlueprintType)
struct FMariaAvatarPhotoSet
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMariaAvatarSourcePhoto> Photos;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float FaceCompleteness = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float BodyCompleteness = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bMinimumFaceSetReady = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bMinimumBodySetReady = false;
};
