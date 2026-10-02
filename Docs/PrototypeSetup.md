# MariaGame – Prototype Setup

## Første Unreal-scene

Opret en map, fx:

- Content/Maps/WardrobePrototype

Placér:

1. Gulv.
2. Lys.
3. Kamera eller third-person character.
4. BP_MariaWardrobe.
5. BP_MariaCharacter.

## Blueprint: BP_MariaWardrobe

Parent class:

- AMariaWardrobeActor

Tilføj et wardrobe/cabinet static mesh.

Flyt HangerRail component ind i skabet, hvor bøjlestangen skal være.

Sæt:

- HangerClass = BP_MariaHanger
- HangerCount = 5
- HangerSpacing = 18

## Blueprint: BP_MariaHanger

Parent class:

- AMariaHangerActor

Tilføj en hanger mesh.

ClothingMesh bruges til det tøj, der skal hænge på bøjlen.

## Næste implementation

- interaction trace
- wardrobe door animation
- clothing pickup/select
- equip på character
- preview mirror
- outfit save
- clothing import UI
