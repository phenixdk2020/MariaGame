#include "MariaPrototypeGameMode.h"
#include "Character/MariaPrototypeCharacter.h"
#include "Wardrobe/MariaWardrobeActor.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/PointLight.h"
#include "Components/LightComponent.h"
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

    APlayerController* PC = World->GetFirstPlayerController();
    if (PC && !PC->GetPawn())
    {
        AMariaPrototypeCharacter* Maria = World->SpawnActor<AMariaPrototypeCharacter>(
            AMariaPrototypeCharacter::StaticClass(),
            FVector(0.0f, 0.0f, 0.0f),
            FRotator::ZeroRotator);

        if (Maria)
        {
            PC->Possess(Maria);
        }
    }

    UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));

    if (CubeMesh)
    {
        AStaticMeshActor* Floor = World->SpawnActor<AStaticMeshActor>(
            AStaticMeshActor::StaticClass(),
            FVector(0.0f, 0.0f, -100.0f),
            FRotator::ZeroRotator);

        if (Floor)
        {
            Floor->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
            Floor->SetActorScale3D(FVector(20.0f, 20.0f, 1.0f));
        }

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

    World->SpawnActor<AMariaWardrobeActor>(
        AMariaWardrobeActor::StaticClass(),
        FVector(500.0f, 0.0f, 0.0f),
        FRotator(0.0f, 180.0f, 0.0f));

    APointLight* KeyLight = World->SpawnActor<APointLight>(
        APointLight::StaticClass(),
        FVector(150.0f, 0.0f, 350.0f),
        FRotator::ZeroRotator);

    if (KeyLight)
    {
        if (UPointLightComponent* Light = Cast<UPointLightComponent>(KeyLight->GetLightComponent()))
        {
            Light->SetIntensity(18000.0f);
            Light->SetAttenuationRadius(1800.0f);
        }
    }

    APointLight* FillLight = World->SpawnActor<APointLight>(
        APointLight::StaticClass(),
        FVector(500.0f, 300.0f, 250.0f),
        FRotator::ZeroRotator);

    if (FillLight)
    {
        if (UPointLightComponent* Light = Cast<UPointLightComponent>(FillLight->GetLightComponent()))
        {
            Light->SetIntensity(9000.0f);
            Light->SetAttenuationRadius(1400.0f);
        }
    }
}
