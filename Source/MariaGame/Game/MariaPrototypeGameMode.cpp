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
#include "Materials/MaterialInstanceDynamic.h"

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

    auto TintActor = [](AStaticMeshActor* Actor, const FLinearColor& Color, float Roughness, float Metallic)
    {
        if (!Actor || !Actor->GetStaticMeshComponent() || Actor->GetStaticMeshComponent()->GetNumMaterials() <= 0)
        {
            return;
        }

        if (UMaterialInstanceDynamic* Material = Actor->GetStaticMeshComponent()->CreateDynamicMaterialInstance(0))
        {
            Material->SetVectorParameterValue(TEXT("Color"), Color);
            Material->SetVectorParameterValue(TEXT("BaseColor"), Color);
            Material->SetScalarParameterValue(TEXT("Roughness"), Roughness);
            Material->SetScalarParameterValue(TEXT("Metallic"), Metallic);
        }
    };

    const FLinearColor FloorColor(0.28f, 0.24f, 0.21f, 1.0f);
    const FLinearColor WallColor(0.54f, 0.47f, 0.40f, 1.0f);
    const FLinearColor CeilingColor(0.72f, 0.68f, 0.62f, 1.0f);
    const FLinearColor RugColor(0.16f, 0.08f, 0.07f, 1.0f);
    const FLinearColor PodiumColor(0.10f, 0.105f, 0.115f, 1.0f);
    const FLinearColor WoodColor(0.17f, 0.055f, 0.02f, 1.0f);
    const FLinearColor MirrorColor(0.12f, 0.19f, 0.23f, 1.0f);
    const FLinearColor BenchFabric(0.30f, 0.15f, 0.13f, 1.0f);

    // Room shell
    TintActor(SpawnStatic(CubeMesh, FVector(0.0f, 0.0f, -105.0f), FVector(12.0f, 12.0f, 0.10f)), FloorColor, 0.62f, 0.0f);
    TintActor(SpawnStatic(CubeMesh, FVector(750.0f, 0.0f, 180.0f), FVector(0.10f, 12.0f, 3.0f)), WallColor, 0.82f, 0.0f);
    TintActor(SpawnStatic(CubeMesh, FVector(-750.0f, 0.0f, 180.0f), FVector(0.10f, 12.0f, 3.0f)), WallColor, 0.82f, 0.0f);
    TintActor(SpawnStatic(CubeMesh, FVector(0.0f, 750.0f, 180.0f), FVector(12.0f, 0.10f, 3.0f)), WallColor, 0.82f, 0.0f);
    TintActor(SpawnStatic(CubeMesh, FVector(0.0f, -750.0f, 180.0f), FVector(12.0f, 0.10f, 3.0f)), WallColor, 0.82f, 0.0f);
    TintActor(SpawnStatic(CubeMesh, FVector(0.0f, 0.0f, 455.0f), FVector(12.0f, 12.0f, 0.10f)), CeilingColor, 0.86f, 0.0f);

    // Dressing-room rug + avatar preview podium
    TintActor(SpawnStatic(CubeMesh, FVector(0.0f, 0.0f, -94.0f), FVector(3.6f, 3.6f, 0.04f)), RugColor, 0.95f, 0.0f);
    TintActor(SpawnStatic(CylinderMesh, FVector(0.0f, 0.0f, -88.0f), FVector(1.35f, 1.35f, 0.12f)), PodiumColor, 0.30f, 0.22f);

    // Mirror frame and dark reflective surrogate panel
    TintActor(SpawnStatic(CubeMesh, FVector(650.0f, -360.0f, 150.0f), FVector(0.08f, 2.0f, 2.4f)), WoodColor, 0.34f, 0.0f);
    TintActor(SpawnStatic(CubeMesh, FVector(638.0f, -360.0f, 150.0f), FVector(0.03f, 1.72f, 2.12f)), MirrorColor, 0.05f, 0.76f);

    // Decorative bench
    TintActor(SpawnStatic(CubeMesh, FVector(250.0f, 420.0f, -45.0f), FVector(1.6f, 0.55f, 0.18f)), BenchFabric, 0.88f, 0.0f);
    TintActor(SpawnStatic(CubeMesh, FVector(160.0f, 420.0f, -78.0f), FVector(0.16f, 0.16f, 0.5f)), WoodColor, 0.42f, 0.0f);
    TintActor(SpawnStatic(CubeMesh, FVector(340.0f, 420.0f, -78.0f), FVector(0.16f, 0.16f, 0.5f)), WoodColor, 0.42f, 0.0f);

    // Wardrobe
    World->SpawnActor<AMariaWardrobeActor>(
        AMariaWardrobeActor::StaticClass(),
        FVector(560.0f, 100.0f, 0.0f),
        FRotator::ZeroRotator);

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

    SpawnLight(FVector(-260.0f, -260.0f, 405.0f), 2200.0f, 850.0f);
    SpawnLight(FVector(260.0f, -260.0f, 405.0f), 2200.0f, 850.0f);
    SpawnLight(FVector(-260.0f, 260.0f, 405.0f), 2200.0f, 850.0f);
    SpawnLight(FVector(260.0f, 260.0f, 405.0f), 2200.0f, 850.0f);

    // Three-point avatar/room lighting
    SpawnLight(FVector(120.0f, -120.0f, 320.0f), 6500.0f, 1300.0f);
    SpawnLight(FVector(420.0f, 360.0f, 260.0f), 4200.0f, 1100.0f);
    SpawnLight(FVector(-250.0f, 200.0f, 280.0f), 3200.0f, 1100.0f);
}
