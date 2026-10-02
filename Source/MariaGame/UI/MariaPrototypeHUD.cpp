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

    if (Maria->IsHelpVisible())
    {
        DrawText(TEXT("MariaGame  v0.2  Dressing Room"), FLinearColor::White, 28.0f, 24.0f, Font, 1.25f, false);
        DrawText(TEXT("WASD: bevæg  |  Mus: kamera  |  E: interager  |  P: preview  |  F5/F9: gem/hent outfit"), 
            FLinearColor(0.75f, 0.78f, 0.82f, 1.0f), 28.0f, 52.0f, Font, 0.95f, false);
        DrawText(TEXT("1/2/3: hårfarve  |  F6/F7/F8: outfit-slot 1/2/3"), 
            FLinearColor(0.75f, 0.78f, 0.82f, 1.0f), 28.0f, 72.0f, Font, 0.95f, false);
        DrawText(TEXT("Preview: F1 front  F2 bag  F3 venstre  F4 højre  |  Musehjul: zoom"), 
            FLinearColor(0.75f, 0.78f, 0.82f, 1.0f), 28.0f, 92.0f, Font, 0.95f, false);
        DrawText(TEXT("I: tøjimport  |  F10: debug  |  H: hjælp"), 
            FLinearColor(0.75f, 0.78f, 0.82f, 1.0f), 28.0f, 112.0f, Font, 0.95f, false);
    }


    if (Maria->IsImportPanelVisible())
    {
        const float PanelX = 70.0f;
        const float PanelY = 155.0f;
        const float PanelW = FMath::Min(620.0f, Width - 140.0f);
        const float PanelH = 360.0f;

        DrawRect(FLinearColor(0.025f, 0.03f, 0.04f, 0.92f), PanelX, PanelY, PanelW, PanelH);
        DrawText(TEXT("TØJIMPORT - KLAR TIL BILLEDER"), FLinearColor(0.92f, 0.82f, 0.58f, 1.0f),
            PanelX + 24.0f, PanelY + 22.0f, Font, 1.25f, false);

        DrawText(TEXT("Billedesæt"), FLinearColor(0.66f, 0.82f, 1.0f, 1.0f),
            PanelX + 24.0f, PanelY + 62.0f, Font, 1.0f, false);

        DrawText(TEXT("Front: krævet"), FLinearColor::White,
            PanelX + 42.0f, PanelY + 88.0f, Font, 0.95f, false);
        DrawText(TEXT("Bag: anbefalet"), FLinearColor::White,
            PanelX + 42.0f, PanelY + 108.0f, Font, 0.95f, false);
        DrawText(TEXT("Venstre/højre side: anbefalet"), FLinearColor::White,
            PanelX + 42.0f, PanelY + 128.0f, Font, 0.95f, false);
        DrawText(TEXT("Detaljer/stof/logo: valgfrit"), FLinearColor::White,
            PanelX + 42.0f, PanelY + 148.0f, Font, 0.95f, false);

        DrawText(TEXT("Planlagt pipeline"), FLinearColor(0.66f, 0.82f, 1.0f, 1.0f),
            PanelX + 24.0f, PanelY + 188.0f, Font, 1.0f, false);
        DrawText(TEXT("1. Filvalidering  ->  2. Tøjtype  ->  3. 3D-template"), FLinearColor::White,
            PanelX + 42.0f, PanelY + 214.0f, Font, 0.92f, false);
        DrawText(TEXT("4. Materiale/tekstur  ->  5. Kropstilpasning"), FLinearColor::White,
            PanelX + 42.0f, PanelY + 236.0f, Font, 0.92f, false);
        DrawText(TEXT("6. Preview  ->  7. Gem i fysisk garderobe"), FLinearColor::White,
            PanelX + 42.0f, PanelY + 258.0f, Font, 0.92f, false);

        DrawText(TEXT("Importmotoren accepterer JPG / JPEG / PNG / WEBP."), FLinearColor(0.72f, 0.76f, 0.80f, 1.0f),
            PanelX + 24.0f, PanelY + 302.0f, Font, 0.88f, false);
        DrawText(TEXT("Personlige kildebilleder er ekskluderet fra Git."), FLinearColor(0.72f, 0.76f, 0.80f, 1.0f),
            PanelX + 24.0f, PanelY + 324.0f, Font, 0.88f, false);
    }

    // Crosshair
    if (!Maria->IsPreviewModeActive() && !Maria->IsImportPanelVisible())
    {
        DrawRect(FLinearColor::White, Width * 0.5f - 8.0f, Height * 0.5f - 1.0f, 16.0f, 2.0f);
        DrawRect(FLinearColor::White, Width * 0.5f - 1.0f, Height * 0.5f - 8.0f, 2.0f, 16.0f);
    }


    const FString StatusMessage = Maria->GetStatusMessage();
    if (!StatusMessage.IsEmpty())
    {
        DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 0.60f), Width * 0.5f - 180.0f, 28.0f, 360.0f, 38.0f);
        DrawText(StatusMessage, FLinearColor(0.92f, 0.86f, 0.62f, 1.0f),
            Width * 0.5f - 135.0f, 38.0f, Font, 1.0f, false);
    }

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

    if (Maria->IsDebugHudVisible())
    {
        float DebugY = 152.0f;
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

}
