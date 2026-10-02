#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Character/MariaAvatarPhotoTypes.h"
#include "MariaAvatarPhotoSubsystem.generated.h"

UCLASS()
class MARIAGAME_API UMariaAvatarPhotoSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="MariaGame|Avatar Photos")
    bool AddPhoto(const FString& FilePath, EMariaAvatarPhotoView View);

    UFUNCTION(BlueprintCallable, Category="MariaGame|Avatar Photos")
    void ClearPhotos();

    UFUNCTION(BlueprintPure, Category="MariaGame|Avatar Photos")
    FMariaAvatarPhotoSet GetPhotoSet() const { return PhotoSet; }

private:
    UPROPERTY()
    FMariaAvatarPhotoSet PhotoSet;

    bool ValidatePhoto(const FString& FilePath, FString& OutMessage) const;
    void RecalculateCompleteness();
};
