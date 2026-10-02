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

## Image-to-Clothing

Spilleren skal kunne trække eller vælge et billede af et stykke tøj, f.eks. en trøje, skjorte, kjole eller jakke.

Systemet bruger billedet som visuel reference og opretter eller tilpasser et 3D-beklædningsstykke, der passer til den aktuelle karakters krop.

### Mål

Workflowet skal opleves sådan:

1. Brugeren dropper et billede af tøjet.
2. Systemet identificerer tøjtypen.
3. Farver, mønstre, logo-/motivplacering, halsudskæring, ærmer og andre synlige detaljer analyseres.
4. Der vælges en passende eksisterende 3D-basis-model fra MariaGames Clothing Library.
5. Basis-modellen tilpasses karakterens kropsmål.
6. Materialer og teksturer genereres eller tilpasses ud fra referencebilledet.
7. Tøjet rigges til MariaGame-skelettet.
8. Brugeren får en 3D-preview direkte på sin karakter.
9. Brugeren kan gemme tøjet i sit wardrobe.

### Første version

Den første praktiske version bør ikke forsøge at konstruere vilkårlig beklædning helt fra nul ud fra ét billede.

I stedet bruges en hybridmodel:

```
Foto af trøje
      │
      ▼
Analyse af type/form/design
      │
      ├──> Find nærmeste Clothing Template
      │
      ▼
Tilpas mesh til figurens kropsmål
      │
      ▼
Generér/tilpas materiale og tekstur
      │
      ▼
Skin / rig til fælles skelet
      │
      ▼
Preview på karakter
```

Dette giver langt mere stabile resultater end ren image-to-3D-generering.

### Clothing Templates

Eksempler:

- TShirt_Fitted
- TShirt_Loose
- Sweatshirt
- Hoodie
- Shirt_LongSleeve
- Shirt_ShortSleeve
- Blouse
- Sweater
- Jacket
- Dress_Short
- Dress_Long
- Skirt
- Jeans
- Trousers
- Shorts

Template-meshet indeholder allerede korrekt topology, UV-layout og skin weights til MariaGame-skelettet.

### Automatisk tilpasning til kroppen

Karakterens krop skal have standardiserede mål/morph targets, f.eks.:

- højde
- skulderbredde
- bryst
- talje
- hofte
- armlængde
- benlængde

Tøjet bruger tilsvarende morph targets, så det kan formes til den aktuelle karakter.

Eksempel:

```
Character Body
    Chest = 0.34
    Waist = -0.12
    Hip = 0.18

        ↓

Clothing Fit System

        ↓

Sweater mesh tilpasses samme proportioner
```

### Lag og clipping

Systemet skal tage højde for, at tøj ligger oven på kroppen og eventuelt oven på andet tøj.

Derfor skal beklædning have:

- Clothing Layer
- Fit Offset
- Collision profile
- Body masking

Eksempel på lag:

1. Body
2. Underwear
3. Shirt / T-shirt
4. Sweater
5. Jacket
6. Accessories

Kropspolygoner, der er helt dækket af tøj, kan skjules for at reducere clipping.

### Materialer fra billedet

Billedet bør bruges til at udlede:

- base color
- mønster
- stofkarakter
- print
- synlige syninger
- roughness-estimat
- normal/detail-struktur

Materialet opbygges som et Unreal Material Instance, så farver og stofegenskaber senere kan redigeres.

### Flere billeder giver bedre resultat

Systemet skal kunne acceptere mere end ét referencebillede af samme beklædningsgenstand:

- front
- bag
- venstre side
- højre side
- detaljebillede

Ét frontbillede skal stadig understøttes, men usynlige sider vil i så fald være estimerede.

### Preview

Efter import vises karakteren i Character Creator med det nye tøj monteret.

Brugeren skal kunne:

- rotere karakteren 360°
- zoome
- skifte pose
- gå mellem originalfoto og 3D-resultat
- justere pasform
- ændre farve
- gemme eller kassere tøjet

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
- UImageToClothingSubsystem
- UClothingFitComponent
- UClothingTemplateDataAsset

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
- import af ét trøjebillede
- mapping til en eksisterende T-shirt/sweater-template
- generering af materiale/tekstur fra referencebilledet
- automatisk tilpasning af trøjen til karakterens kropsmål
- 3D-preview på karakteren

Når dette fungerer stabilt, kobles mere avanceret foto-til-ansigt og fuld image-to-clothing-generering på.
