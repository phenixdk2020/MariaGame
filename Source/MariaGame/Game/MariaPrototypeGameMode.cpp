#include "MariaPrototypeGameMode.h"
#include "Character/MariaPrototypeCharacter.h"
#include "Wardrobe/MariaWardrobeActor.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

AMariaPrototypeGameMode::AMariaPrototypeGameMode()
{
    DefaultPawnClass = AMariaPrototypeCharacter::StaticClass();
}

void AMariaPrototypeGameMode::BeginPlay()
{
    Super::BeginPlay();
    BuildPrototypeWorld();
}

void AMariaPrototypeGameMode::BuildPrototypeWorld()
{
    if (!GetWorld())
    {
        return;
    }

    UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh)
    {
        AStaticMeshActor* Floor = GetWorld()->SpawnActor<AStaticMeshActor>(
            AStaticMeshActor::StaticClass(),
            FVector(0.0f, 0.0f, -55.0f),
            FRotator::ZeroRotator);

        if (Floor)
        {
            Floor->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
            Floor->SetActorScale3D(FVector(20.0f, 20.0f, 1.0f));
        }
    }

    GetWorld()->SpawnActor<AMariaWardrobeActor>(
        AMariaWardrobeActor::StaticClass(),
        FVector(500.0f, 0.0f, 0.0f),
        FRotator::ZeroRotator);
}
