# MariaGame – Game Design

## Vision

MariaGame er en virtuel avatar- og garderobeoplevelse bygget i Unreal Engine.

Spilleren kan:

1. Oprette en 3D-avatar ud fra referencebilleder.
2. Ændre frisure og hårfarve.
3. Importere sit eget tøj fra billeder.
4. Gemme tøjet i en virtuel garderobe.
5. Gå hen til garderobeskabet og vælge tøj, som hænger på bøjler.
6. Klæde avataren på og se outfittet i 3D.
7. Gemme outfits.

## Hovedsystemer

### Avatar Creator
- Ansigt fra referencebilleder.
- Kropsproportioner.
- Hårvalg.
- Hårfarve.
- Hudtone.
- Gem/load af avatar.

### Clothing Import
- Import af billede eller flere billeder af et stykke tøj.
- Genkendelse af beklædningstype.
- Mapping til eksisterende clothing template.
- Materiale/tekstur fra reference.
- Automatisk fit til avatarens krop.
- Gem i wardrobe.

### Virtual Wardrobe

Wardrobe skal være et fysisk 3D-skab i verdenen, ikke kun en menu.

Skabet indeholder:

- bøjlestang
- bøjler
- hylder
- skuffer
- sko-sektion
- accessory-sektion

Tøjtyper som skjorter, trøjer, kjoler og jakker hænger på bøjler.

Bukser kan enten hænge på bøjler med clips eller ligge foldet på hylder.

Sko står på hylder.

Accessories ligger i skuffer eller på dedikerede holdere.

### Wardrobe Interaction

Spilleren kan:

- åbne/lukke skabsdøre
- gå tæt på skabet
- pege på et stykke tøj
- fremhæve det
- se navn/kategori
- vælge "Prøv"
- vælge "Tag på"
- vælge "Læg tilbage"
- bladre mellem bøjler
- gemme outfit

Når et stykke tøj tages ud af skabet, fjernes det visuelt fra bøjlen, indtil det lægges tilbage eller avataren tager det på.

### Dressing / Outfit System

Slots:

- Hair
- Headwear
- UpperBody
- LowerBody
- Dress
- Jacket
- Shoes
- Accessory

Systemet forhindrer ugyldige kombinationer, fx:
- Dress kan skjule UpperBody og LowerBody.
- Jacket ligger uden på UpperBody.
- Headwear kan påvirke Hair.

### Preview Room

Spilleren kan stå foran et spejl eller i et preview-område og:

- rotere kamera
- zoome
- skifte pose
- gå
- dreje sig
- se outfit forfra/bagfra
- gemme outfit

## MVP

Første spilbare prototype:

1. En standard avatar.
2. Et fysisk wardrobe-skab.
3. En bøjlestang.
4. 5 bøjler.
5. 3 stykker testtøj.
6. Interaktion med skabet.
7. Vælg tøj fra en bøjle.
8. Tøjet vises på avataren.
9. Læg tøjet tilbage.
10. Gem/load outfit.

## Senere

- Foto-til-avatar.
- Image-to-clothing.
- Flere skabstyper.
- Flere rum.
- Spejl.
- Walking closet.
- Outfit presets.
- Automatisk forslag til outfits.
