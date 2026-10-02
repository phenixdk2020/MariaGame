#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Wardrobe/MariaClothingImportTypes.h"
#include "MariaClothingImportSubsystem.generated.h"

UCLASS()
class MARIAGAME_API UMariaClothingImportSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="MariaGame|Clothing Import")
    FGuid CreateImportJob(const FText& DisplayName);

    UFUNCTION(BlueprintCallable, Category="MariaGame|Clothing Import")
    bool AddSourceImage(FGuid JobId, const FString& FilePath, EMariaClothingImageView View);

    UFUNCTION(BlueprintCallable, Category="MariaGame|Clothing Import")
    bool RemoveImportJob(FGuid JobId);

    UFUNCTION(BlueprintCallable, Category="MariaGame|Clothing Import")
    bool CancelImportJob(FGuid JobId);

    UFUNCTION(BlueprintPure, Category="MariaGame|Clothing Import")
    TArray<FMariaClothingImportJob> GetImportJobs() const { return Jobs; }

    UFUNCTION(BlueprintPure, Category="MariaGame|Clothing Import")
    bool GetImportJob(FGuid JobId, FMariaClothingImportJob& OutJob) const;

private:
    UPROPERTY()
    TArray<FMariaClothingImportJob> Jobs;

    bool ValidateSourcePath(const FString& FilePath, FString& OutMessage) const;
    FMariaClothingImportJob* FindMutableJob(FGuid JobId);
};
