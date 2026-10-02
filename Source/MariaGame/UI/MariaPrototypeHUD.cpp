#include "MariaPrototypeHUD.h"
#include "Character/MariaPrototypeCharacter.h"
#include "Wardrobe/MariaWardrobeComponent.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "GameFramework/PlayerController.h"

FString AMariaPrototypeHUD::SlotToString(EMariaClothingSlot Slot) const
{
    switch (Slot)
    {
        case EMariaClothingSlot::Hair: return TEXT("Hår");
        case EMariaClothingSlot::Headwear: return TEXT("Hovedbeklædning");
        case EMariaClothingSlot::UpperBody: return TEXT("Overdel");
        case EMariaClothingSlot::LowerBody: return TEXT("Underdel");
        case EMariaClothingSlot::Dress: return TEXT("Kjole");
        case EMariaClothingSlot::Jacket: return TEXT("Jakke");
        case EMariaClothingSlot::Shoes: return TEXT("Sko");
        case EMariaClothingSlot::Accessory: return TEXT("Tilbehør");
        default: return TEXT("Ukendt");
    }
}

void AMariaPrototypeHUD::DrawHUD()
{
    Super::DrawHUD();

    if (!Canvas || !PlayerOwner)
    {
        return;
    }

    AMariaPrototypeCharacter* Maria = Cast<AMariaPrototypeCharacter>(PlayerOwner->GetPawn());
    if (!Maria)
    {
        return;
    }

    UFont* Font = GEngine ? GEngine->GetSmallFont() : nullptr;
    const float Width = Canvas->ClipX;
    const float Height = Canvas->ClipY;

    DrawText(TEXT("MariaGame  v0.2  Dressing Room"), FLinearColor::White, 28.0f, 24.0f, Font, 1.25f, false);
    DrawText(TEXT("WASD: bevæg  |  Mus: kamera  |  E: interager  |  P: preview  |  F5/F9: gem/hent outfit"), 
        FLinearColor(0.75f, 0.78f, 0.82f, 1.0f), 28.0f, 52.0f, Font, 0.95f, false);
    DrawText(TEXT("1/2/3: hårfarve  |  F6/F7/F8: outfit-slot 1/2/3"), 
        FLinearColor(0.75f, 0.78f, 0.82f, 1.0f), 28.0f, 72.0f, Font, 0.95f, false);
    DrawText(TEXT("Preview: F1 front  F2 bag  F3 venstre  F4 højre  |  Musehjul: zoom"), 
        FLinearColor(0.75f, 0.78f, 0.82f, 1.0f), 28.0f, 92.0f, Font, 0.95f, false);

    // Crosshair
    DrawRect(FLinearColor::White, Width * 0.5f - 8.0f, Height * 0.5f - 1.0f, 16.0f, 2.0f);
    DrawRect(FLinearColor::White, Width * 0.5f - 1.0f, Height * 0.5f - 8.0f, 2.0f, 16.0f);

    const FString FocusText = Maria->GetFocusedItemText();
    const FString Prompt = Maria->GetInteractionPrompt();

    if (!FocusText.IsEmpty())
    {
        DrawText(FocusText, FLinearColor::White, Width * 0.5f - 110.0f, Height - 118.0f, Font, 1.10f, false);
    }

    if (!Prompt.IsEmpty())
    {
        DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 0.55f), Width * 0.5f - 155.0f, Height - 88.0f, 310.0f, 42.0f);
        DrawText(Prompt, FLinearColor(1.0f, 0.92f, 0.55f, 1.0f), Width * 0.5f - 125.0f, Height - 78.0f, Font, 1.15f, false);
    }

    float DebugY = 132.0f;
    const float DebugX = Width - 280.0f;
    DrawText(TEXT("DEBUG"), FLinearColor(0.65f, 0.85f, 1.0f, 1.0f), DebugX, DebugY, Font, 1.0f, false);
    DebugY += 20.0f;

    DrawText(FString::Printf(TEXT("Preview: %s"), Maria->IsPreviewModeActive() ? TEXT("ON") : TEXT("OFF")),
        FLinearColor::White, DebugX, DebugY, Font, 0.9f, false);
    DebugY += 18.0f;

    DrawText(FString::Printf(TEXT("Hår: %s"), *Maria->GetHairPresetName()),
        FLinearColor::White, DebugX, DebugY, Font, 0.9f, false);
    DebugY += 18.0f;

    DrawText(FString::Printf(TEXT("Outfit-slot: %d"), Maria->GetOutfitSlotIndex()),
        FLinearColor::White, DebugX, DebugY, Font, 0.9f, false);
    DebugY += 24.0f;

    DrawText(TEXT("Påklædning:"), FLinearColor(0.75f, 0.85f, 1.0f, 1.0f), DebugX, DebugY, Font, 0.9f, false);
    DebugY += 18.0f;

    if (Maria->Wardrobe)
    {
        const EMariaClothingSlot Slots[] = {
            EMariaClothingSlot::UpperBody,
            EMariaClothingSlot::LowerBody,
            EMariaClothingSlot::Dress,
            EMariaClothingSlot::Jacket,
            EMariaClothingSlot::Shoes
        };

        for (EMariaClothingSlot Slot : Slots)
        {
            const FName* ItemId = Maria->Wardrobe->EquippedItems.Find(Slot);
            const FString Value = ItemId ? ItemId->ToString() : TEXT("-");
            DrawText(FString::Printf(TEXT("%s: %s"), *SlotToString(Slot), *Value),
                FLinearColor::White, DebugX, DebugY, Font, 0.82f, false);
            DebugY += 16.0f;
        }
    }
}
