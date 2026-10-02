#include "MariaCharacter.h"
#include "Wardrobe/MariaWardrobeComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AMariaCharacter::AMariaCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
    HeadMesh->SetupAttachment(GetMesh());

    HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
    HairMesh->SetupAttachment(GetMesh());

    UpperBodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UpperBody"));
    UpperBodyMesh->SetupAttachment(GetMesh());

    LowerBodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LowerBody"));
    LowerBodyMesh->SetupAttachment(GetMesh());

    DressMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Dress"));
    DressMesh->SetupAttachment(GetMesh());

    JacketMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Jacket"));
    JacketMesh->SetupAttachment(GetMesh());

    ShoesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));
    ShoesMesh->SetupAttachment(GetMesh());

    Wardrobe = CreateDefaultSubobject<UMariaWardrobeComponent>(TEXT("Wardrobe"));

    ConfigureFollower(HeadMesh);
    ConfigureFollower(HairMesh);
    ConfigureFollower(UpperBodyMesh);
    ConfigureFollower(LowerBodyMesh);
    ConfigureFollower(DressMesh);
    ConfigureFollower(JacketMesh);
    ConfigureFollower(ShoesMesh);
}

void AMariaCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (HairMesh && HairMesh->GetNumMaterials() > 0)
    {
        HairMaterialInstance = HairMesh->CreateDynamicMaterialInstance(0);
    }
}

void AMariaCharacter::ConfigureFollower(USkeletalMeshComponent* Component)
{
    if (!Component)
    {
        return;
    }

    Component->SetLeaderPoseComponent(GetMesh());
    Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

USkeletalMeshComponent* AMariaCharacter::GetMeshForSlot(EMariaClothingSlot Slot) const
{
    switch (Slot)
    {
        case EMariaClothingSlot::Hair:      return HairMesh;
        case EMariaClothingSlot::UpperBody: return UpperBodyMesh;
        case EMariaClothingSlot::LowerBody: return LowerBodyMesh;
        case EMariaClothingSlot::Dress:     return DressMesh;
        case EMariaClothingSlot::Jacket:    return JacketMesh;
        case EMariaClothingSlot::Shoes:     return ShoesMesh;
        default:                            return nullptr;
    }
}

bool AMariaCharacter::WearItem(const FMariaClothingItem& Item)
{
    USkeletalMeshComponent* TargetMesh = GetMeshForSlot(Item.Slot);
    if (!TargetMesh || !Item.SkeletalMesh)
    {
        return false;
    }

    if (Item.Slot == EMariaClothingSlot::Dress)
    {
        UpperBodyMesh->SetVisibility(false, true);
        LowerBodyMesh->SetVisibility(false, true);
    }
    else if (Item.Slot == EMariaClothingSlot::UpperBody ||
             Item.Slot == EMariaClothingSlot::LowerBody)
    {
        DressMesh->SetVisibility(false, true);
    }

    TargetMesh->SetSkeletalMesh(Item.SkeletalMesh);
    TargetMesh->SetVisibility(true, true);

    return Wardrobe ? Wardrobe->EquipItem(Item.ItemId) : true;
}

void AMariaCharacter::RemoveItem(EMariaClothingSlot Slot)
{
    if (USkeletalMeshComponent* TargetMesh = GetMeshForSlot(Slot))
    {
        TargetMesh->SetSkeletalMesh(nullptr);
        TargetMesh->SetVisibility(false, true);
    }

    if (Slot == EMariaClothingSlot::Dress)
    {
        UpperBodyMesh->SetVisibility(true, true);
        LowerBodyMesh->SetVisibility(true, true);
    }

    if (Wardrobe)
    {
        Wardrobe->UnequipSlot(Slot);
    }
}

void AMariaCharacter::SetHairColor(FLinearColor NewColor)
{
    if (!HairMaterialInstance && HairMesh && HairMesh->GetNumMaterials() > 0)
    {
        HairMaterialInstance = HairMesh->CreateDynamicMaterialInstance(0);
    }

    if (HairMaterialInstance)
    {
        HairMaterialInstance->SetVectorParameterValue(TEXT("HairColor"), NewColor);
    }
}
