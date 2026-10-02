# MariaGame – Kør prototype 0.1

## Clone

```powershell
git clone https://github.com/phenixdk2020/MariaGame.git
cd MariaGame
```

Har du allerede projektet:

```powershell
git pull
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
- **E** – interager med skab eller bøjle
- **P** – dressing/360° preview til/fra
- **F5** – gem aktuelt outfit
- **F9** – indlæs gemt outfit

## Forventet prototype

Ved runtime oprettes:

- simpelt gulv
- dummy-Maria
- prototype-garderobeskab
- 5 bøjler
- 5 placeholder-beklædningsgenstande

### Testflow

1. Gå hen til garderoben.
2. Peg på skabet og tryk **E** for at åbne dørene.
3. Peg på en bøjle og tryk **E**.
4. Tøjet fjernes visuelt fra bøjlen og vises på dummyen.
5. Vælg et andet stykke tøj i samme slot.
6. Det gamle stykke tøj lægges automatisk tilbage på sin bøjle.
7. Tryk **P** for at gå i preview-mode.
8. Bevæg musen vandret for at rotere dummyen.
9. Tryk **F5** for at gemme outfittet.
10. Tryk **F9** for at gendanne det.

Kjole og overdel/underdel håndteres som gensidigt eksklusive lag i prototypen.

## Bemærkning

Der bruges midlertidigt Unreal Engine-primitiver til dummy, skab, bøjler og tøj. Systemarkitekturen er lavet sådan, at de senere kan erstattes af den rigtige Maria-avatar og riggede beklædningsmodeller uden at omskrive wardrobe-logikken.
