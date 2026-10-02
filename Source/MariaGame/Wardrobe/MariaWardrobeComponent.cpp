#include "MariaWardrobeComponent.h"

UMariaWardrobeComponent::UMariaWardrobeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UMariaWardrobeComponent::AddItem(const FMariaClothingItem& Item)
{
    for (const FMariaClothingItem& Existing : Items)
    {
        if (Existing.ItemId == Item.ItemId)
        {
            return false;
        }
    }

    Items.Add(Item);
    return true;
}

bool UMariaWardrobeComponent::EquipItem(FName ItemId)
{
    FMariaClothingItem Item;
    if (!FindItem(ItemId, Item))
    {
        return false;
    }

    if (Item.Slot == EMariaClothingSlot::Dress)
    {
        EquippedItems.Remove(EMariaClothingSlot::UpperBody);
        EquippedItems.Remove(EMariaClothingSlot::LowerBody);
    }
    else if (Item.Slot == EMariaClothingSlot::UpperBody || Item.Slot == EMariaClothingSlot::LowerBody)
    {
        EquippedItems.Remove(EMariaClothingSlot::Dress);
    }

    EquippedItems.Add(Item.Slot, Item.ItemId);
    return true;
}

bool UMariaWardrobeComponent::UnequipSlot(EMariaClothingSlot Slot)
{
    return EquippedItems.Remove(Slot) > 0;
}

bool UMariaWardrobeComponent::FindItem(FName ItemId, FMariaClothingItem& OutItem) const
{
    for (const FMariaClothingItem& Item : Items)
    {
        if (Item.ItemId == ItemId)
        {
            OutItem = Item;
            return true;
        }
    }

    return false;
}
