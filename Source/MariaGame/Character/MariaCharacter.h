#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Wardrobe/MariaClothingTypes.h"
#include "MariaCharacter.generated.h"

class USkeletalMeshComponent;
class UMariaWardrobeComponent;

UCLASS()
class MARIAGAME_API AMariaCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMariaCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance")
    TObjectPtr<USkeletalMeshComponent> HeadMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance")
    TObjectPtr<USkeletalMeshComponent> HairMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance")
    TObjectPtr<USkeletalMeshComponent> UpperBodyMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance")
    TObjectPtr<USkeletalMeshComponent> LowerBodyMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance")
    TObjectPtr<USkeletalMeshComponent> DressMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance")
    TObjectPtr<USkeletalMeshComponent> JacketMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance")
    TObjectPtr<USkeletalMeshComponent> ShoesMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance")
    TObjectPtr<UMariaWardrobeComponent> Wardrobe;

    UFUNCTION(BlueprintCallable, Category="Appearance")
    bool WearItem(const FMariaClothingItem& Item);

    UFUNCTION(BlueprintCallable, Category="Appearance")
    void RemoveItem(EMariaClothingSlot Slot);

    UFUNCTION(BlueprintCallable, Category="Appearance|Hair")
    void SetHairColor(FLinearColor NewColor);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(Transient)
    TObjectPtr<UMaterialInstanceDynamic> HairMaterialInstance;

    USkeletalMeshComponent* GetMeshForSlot(EMariaClothingSlot Slot) const;
    void ConfigureFollower(USkeletalMeshComponent* Component);
};
