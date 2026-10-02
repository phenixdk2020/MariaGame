#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Wardrobe/MariaClothingTypes.h"
#include "MariaOutfitSaveGame.generated.h"

UCLASS()
class MARIAGAME_API UMariaOutfitSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category="Outfit")
    TMap<EMariaClothingSlot, FName> EquippedItems;
};
