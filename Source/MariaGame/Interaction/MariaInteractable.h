#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MariaInteractable.generated.h"

UINTERFACE(BlueprintType)
class UMariaInteractable : public UInterface
{
    GENERATED_BODY()
};

class MARIAGAME_API IMariaInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    void Interact(AActor* Interactor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    void SetFocused(bool bFocused);
};
