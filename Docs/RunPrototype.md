# MariaGame – Kør prototype 0.1

## Clone

```powershell
git clone https://github.com/phenixdk2020/MariaGame.git
cd MariaGame
```

## Unreal Engine

Projektet er sat op til Unreal Engine 5.8.

1. Højreklik på `MariaGame.uproject`.
2. Vælg **Generate Visual Studio project files**.
3. Åbn `MariaGame.sln`.
4. Build target: **Development Editor / Win64**.
5. Åbn `MariaGame.uproject`.
6. Tryk **Play**.

## Prototype controls

- **WASD** – bevæg dummy
- **Mus** – kamera
- **E** – interact

## Forventet prototype

Ved runtime oprettes:

- et simpelt gulv
- dummy-Maria
- et prototype-garderobeskab
- 5 bøjler

Peg på garderobeskabet og tryk **E** for at åbne/lukke dørene.

Bøjler implementerer allerede interaction og kan senere holde rigtige clothing meshes.

## Bemærkning

Der bruges midlertidigt simple Unreal Engine-primitiver. De erstattes senere af den rigtige Maria-avatar, garderobemodeller, bøjler og tøjassets uden at ændre den overordnede systemarkitektur.
