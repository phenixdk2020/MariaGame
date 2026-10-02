#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MariaPrototypeHUD.generated.h"

UCLASS()
class MARIAGAME_API AMariaPrototypeHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

private:
    FString SlotToString(EMariaClothingSlot Slot) const;
};
