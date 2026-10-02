#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MariaClothingTypes.h"
#include "MariaWardrobeComponent.generated.h"

UCLASS(ClassGroup=(MariaGame), meta=(BlueprintSpawnableComponent))
class MARIAGAME_API UMariaWardrobeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMariaWardrobeComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wardrobe")
    TArray<FMariaClothingItem> Items;

    UPROPERTY(BlueprintReadOnly, Category="Wardrobe")
    TMap<EMariaClothingSlot, FName> EquippedItems;

    UFUNCTION(BlueprintCallable, Category="Wardrobe")
    bool AddItem(const FMariaClothingItem& Item);

    UFUNCTION(BlueprintCallable, Category="Wardrobe")
    bool EquipItem(FName ItemId);

    UFUNCTION(BlueprintCallable, Category="Wardrobe")
    bool UnequipSlot(EMariaClothingSlot Slot);

    UFUNCTION(BlueprintPure, Category="Wardrobe")
    bool FindItem(FName ItemId, FMariaClothingItem& OutItem) const;
};
