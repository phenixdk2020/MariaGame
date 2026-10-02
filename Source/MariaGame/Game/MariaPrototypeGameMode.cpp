#include "MariaPrototypeGameMode.h"
#include "Character/MariaPrototypeCharacter.h"
#include "Wardrobe/MariaWardrobeActor.h"
#include "UI/MariaPrototypeHUD.h"
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
    HUDClass = AMariaPrototypeHUD::StaticClass();
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
    UStaticMesh* CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));

    auto SpawnStatic = [&](UStaticMesh* Mesh, const FVector& Location, const FVector& Scale, const FRotator& Rotation = FRotator::ZeroRotator)
    {
        if (!Mesh)
        {
            return static_cast<AStaticMeshActor*>(nullptr);
        }

        AStaticMeshActor* Actor = World->SpawnActor<AStaticMeshActor>(
            AStaticMeshActor::StaticClass(),
            Location,
            Rotation);

        if (Actor && Actor->GetStaticMeshComponent())
        {
            Actor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
            Actor->GetStaticMeshComponent()->SetStaticMesh(Mesh);
            Actor->SetActorScale3D(Scale);
        }

        return Actor;
    };

    // Room shell
    SpawnStatic(CubeMesh, FVector(0.0f, 0.0f, -105.0f), FVector(12.0f, 12.0f, 0.10f));
    SpawnStatic(CubeMesh, FVector(750.0f, 0.0f, 180.0f), FVector(0.10f, 12.0f, 3.0f));
    SpawnStatic(CubeMesh, FVector(-750.0f, 0.0f, 180.0f), FVector(0.10f, 12.0f, 3.0f));
    SpawnStatic(CubeMesh, FVector(0.0f, 750.0f, 180.0f), FVector(12.0f, 0.10f, 3.0f));
    SpawnStatic(CubeMesh, FVector(0.0f, -750.0f, 180.0f), FVector(12.0f, 0.10f, 3.0f));
    SpawnStatic(CubeMesh, FVector(0.0f, 0.0f, 455.0f), FVector(12.0f, 12.0f, 0.10f));

    // Dressing-room zone + avatar preview podium
    SpawnStatic(CubeMesh, FVector(0.0f, 0.0f, -94.0f), FVector(3.6f, 3.6f, 0.04f));
    SpawnStatic(CylinderMesh, FVector(0.0f, 0.0f, -88.0f), FVector(1.35f, 1.35f, 0.12f));

    // Mirror frame and mirror panel
    SpawnStatic(CubeMesh, FVector(650.0f, -360.0f, 150.0f), FVector(0.08f, 2.0f, 2.4f));
    SpawnStatic(CubeMesh, FVector(638.0f, -360.0f, 150.0f), FVector(0.03f, 1.72f, 2.12f));

    // Decorative bench
    SpawnStatic(CubeMesh, FVector(250.0f, 420.0f, -45.0f), FVector(1.6f, 0.55f, 0.18f));
    SpawnStatic(CubeMesh, FVector(160.0f, 420.0f, -78.0f), FVector(0.16f, 0.16f, 0.5f));
    SpawnStatic(CubeMesh, FVector(340.0f, 420.0f, -78.0f), FVector(0.16f, 0.16f, 0.5f));

    // Wardrobe
    World->SpawnActor<AMariaWardrobeActor>(
        AMariaWardrobeActor::StaticClass(),
        FVector(520.0f, 220.0f, 0.0f),
        FRotator(0.0f, 180.0f, 0.0f));

    auto SpawnLight = [&](const FVector& Location, float Intensity, float Radius)
    {
        APointLight* LightActor = World->SpawnActor<APointLight>(
            APointLight::StaticClass(),
            Location,
            FRotator::ZeroRotator);

        if (LightActor)
        {
            if (UPointLightComponent* Light = Cast<UPointLightComponent>(LightActor->GetLightComponent()))
            {
                Light->SetMobility(EComponentMobility::Movable);
                Light->SetIntensity(Intensity);
                Light->SetAttenuationRadius(Radius);
            }
        }
    };

    // Ceiling fixtures
    SpawnStatic(CylinderMesh, FVector(-260.0f, -260.0f, 435.0f), FVector(0.28f, 0.28f, 0.05f));
    SpawnStatic(CylinderMesh, FVector(260.0f, -260.0f, 435.0f), FVector(0.28f, 0.28f, 0.05f));
    SpawnStatic(CylinderMesh, FVector(-260.0f, 260.0f, 435.0f), FVector(0.28f, 0.28f, 0.05f));
    SpawnStatic(CylinderMesh, FVector(260.0f, 260.0f, 435.0f), FVector(0.28f, 0.28f, 0.05f));

    SpawnLight(FVector(-260.0f, -260.0f, 405.0f), 3500.0f, 850.0f);
    SpawnLight(FVector(260.0f, -260.0f, 405.0f), 3500.0f, 850.0f);
    SpawnLight(FVector(-260.0f, 260.0f, 405.0f), 3500.0f, 850.0f);
    SpawnLight(FVector(260.0f, 260.0f, 405.0f), 3500.0f, 850.0f);

    // Three-point avatar/room lighting
    SpawnLight(FVector(120.0f, -120.0f, 320.0f), 12000.0f, 1500.0f);
    SpawnLight(FVector(420.0f, 360.0f, 260.0f), 7000.0f, 1200.0f);
    SpawnLight(FVector(-250.0f, 200.0f, 280.0f), 5000.0f, 1200.0f);
}
