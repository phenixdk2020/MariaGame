# MariaGame – Character System

## Mål

Spilleren skal kunne oprette en 3D-figur med udgangspunkt i et billede af en rigtig person.

Figuren skal være modulær, så udseendet kan ændres uden at skulle generere hele karakteren igen.

## Grundprincip

Et foto bruges primært som reference til:

- ansigtsform
- hudtone
- øjenfarve
- øjenbryn
- næse
- mund
- generelle ansigtstræk

Frisure og tøj skal **ikke** være permanent integreret i hoved- eller kropsmodellen.

De skal ligge som separate udskiftelige 3D-assets.

## Modulær karakter

Karakteren opdeles i:

1. Head / Face
2. Hair
3. Torso
4. Legs
5. Feet
6. Hands
7. Accessories

Mulige accessories:

- briller
- hat
- smykker
- taske
- jakke
- andet udstyr

## Skelet

Alle kropsdele og alt kompatibelt tøj skal bruge samme skelet.

Det betyder, at:

- animationer kan genbruges
- tøj følger karakterens bevægelser
- hår og accessories kan monteres på sockets
- vi undgår at skulle lave separate animationer til hvert outfit

## Hår

Frisurer laves som separate meshes.

Eksempel:

- Hair_Short_01
- Hair_Long_01
- Hair_Ponytail_01
- Hair_Curly_01

Hår vælges via Character Customization-systemet.

Senere kan der tilføjes:

- hårfarve
- længde
- highlights
- fysik på langt hår

## Tøj

Tøj opdeles i slots.

Eksempel:

- UpperBody
- LowerBody
- Shoes
- Jacket
- Headwear
- Accessory

Eksempel på outfits:

- T-shirt
- skjorte
- kjole
- jeans
- nederdel
- jakke
- sneakers
- støvler

## Character Customization Data

Karakterens valgte udseende gemmes som data frem for som én samlet model.

Eksempel:

```
CharacterAppearance
{
    FaceId
    SkinTone
    EyeColor
    HairId
    HairColor
    UpperBodyId
    LowerBodyId
    ShoesId
    AccessoryIds
}
```

## Workflow fra billede

Foreslået pipeline:

1. Brugeren vælger eller uploader et billede.
2. Ansigtet analyseres eller modelleres ud fra billedet.
3. Der oprettes en neutral karakter med standardkrop.
4. Ansigtsmodellen tilpasses billedet.
5. Karakteren rigges til MariaGame-skelettet.
6. Spilleren vælger selv:
   - frisure
   - hårfarve
   - tøj
   - sko
   - accessories
7. Udseendet gemmes som CharacterAppearance-data.

## Vigtigt designvalg

Vi bør ikke forsøge at lave hele personen inklusive hår og tøj direkte som én 3D-model fra et enkelt foto.

Det vil gøre senere customization meget vanskelig.

Fotoet bør hovedsageligt bruges til identiteten/ansigtet, mens resten bygges som genbrugelige modulære dele.

## Unreal-arkitektur

Foreslåede klasser:

- AMariaCharacter
- UMariaCharacterAppearanceComponent
- UMariaCharacterCustomizationSubsystem
- UCharacterAppearanceDataAsset
- UClothingItemDataAsset
- UHairStyleDataAsset

Character Blueprint:

```
BP_MariaCharacter
 ├─ Body
 ├─ Head
 ├─ Hair
 ├─ UpperBody
 ├─ LowerBody
 ├─ Shoes
 └─ Accessories
```

Alle krops- og beklædningsdele skal så vidt muligt være kompatible med samme animation blueprint.

## Første prototype

Version 0.1 bør indeholde:

- én standard karakter
- 1 ansigt
- 3 frisurer
- 3 overdele
- 3 underdele
- 2 par sko
- skift af hår i runtime
- skift af tøj i runtime
- gem/load af udseende

Når dette fungerer stabilt, kobles foto-til-ansigt workflowet på.
