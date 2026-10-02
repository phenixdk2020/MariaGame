#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MariaPrototypeGameMode.generated.h"

UCLASS()
class MARIAGAME_API AMariaPrototypeGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMariaPrototypeGameMode();

protected:
    virtual void BeginPlay() override;

private:
    void BuildPrototypeWorld();
};
