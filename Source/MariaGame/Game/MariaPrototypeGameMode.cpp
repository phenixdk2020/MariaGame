#include "MariaPrototypeGameMode.h"
#include "Character/MariaPrototypeCharacter.h"
#include "Wardrobe/MariaWardrobeActor.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/PointLight.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"

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
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    // Guarantee a visible/possessed prototype character even when the
    // engine Entry map does not contain a PlayerStart.
    APlayerController* PC = World->GetFirstPlayerController();
    if (PC && !PC->GetPawn())
    {
        AMariaPrototypeCharacter* Maria = World->SpawnActor<AMariaPrototypeCharacter>(
            AMariaPrototypeCharacter::StaticClass(),
            FVector(0.0f, 0.0f, 0.0f),
            FRotator(0.0f, 0.0f, 0.0f));

        if (Maria)
        {
            PC->Possess(Maria);
        }
    }

    UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));

    if (CubeMesh)
    {
        // Floor
        AStaticMeshActor* Floor = World->SpawnActor<AStaticMeshActor>(
            AStaticMeshActor::StaticClass(),
            FVector(0.0f, 0.0f, -100.0f),
            FRotator::ZeroRotator);

        if (Floor)
        {
            Floor->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
            Floor->SetActorScale3D(FVector(20.0f, 20.0f, 1.0f));
        }

        // Simple back wall so the scene is obviously visible.
        AStaticMeshActor* BackWall = World->SpawnActor<AStaticMeshActor>(
            AStaticMeshActor::StaticClass(),
            FVector(700.0f, 0.0f, 150.0f),
            FRotator::ZeroRotator);

        if (BackWall)
        {
            BackWall->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
            BackWall->SetActorScale3D(FVector(0.5f, 12.0f, 5.0f));
        }
    }

    // Wardrobe in front of the character.
    World->SpawnActor<AMariaWardrobeActor>(
        AMariaWardrobeActor::StaticClass(),
        FVector(500.0f, 0.0f, 0.0f),
        FRotator(0.0f, 180.0f, 0.0f));

    // Strong temporary prototype lights. These will later be replaced by
    // a proper room/lighting setup.
    APointLight* KeyLight = World->SpawnActor<APointLight>(
        APointLight::StaticClass(),
        FVector(150.0f, 0.0f, 350.0f),
        FRotator::ZeroRotator);

    if (KeyLight && KeyLight->GetPointLightComponent())
    {
        KeyLight->GetPointLightComponent()->SetIntensity(18000.0f);
        KeyLight->GetPointLightComponent()->SetAttenuationRadius(1800.0f);
    }

    APointLight* FillLight = World->SpawnActor<APointLight>(
        APointLight::StaticClass(),
        FVector(500.0f, 300.0f, 250.0f),
        FRotator::ZeroRotator);

    if (FillLight && FillLight->GetPointLightComponent())
    {
        FillLight->GetPointLightComponent()->SetIntensity(9000.0f);
        FillLight->GetPointLightComponent()->SetAttenuationRadius(1400.0f);
    }
}
