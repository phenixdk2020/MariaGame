# MariaGame – Kør prototype 0.2

## Clone / update

Første gang:

```powershell
git clone https://github.com/phenixdk2020/MariaGame.git
cd MariaGame
```

Hvis projektet allerede findes:

```powershell
git pull
```

## Unreal Engine

Projektet er sat op til Unreal Engine 5.8.

1. Højreklik på `MariaGame.uproject`.
2. Vælg **Generate Visual Studio project files**, hvis nødvendigt.
3. Build target: **MariaGameEditor / Development / Win64**.
4. Åbn `MariaGame.uproject`.
5. Tryk **Play**.

Direkte build:

```powershell
& "I:\Spil\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" `
    MariaGameEditor Win64 Development `
    -Project="R:\Onedrive\Unreal\MariaGame\MariaGame.uproject" `
    -WaitMutex
```

## Controls

- **WASD** – bevæg Maria-prototypen
- **Mus** – kamera
- **E** – åbn/luk garderobe eller tag fokuseret tøj på
- **P** – 360° preview til/fra
- **1** – blond hårfarve
- **2** – brun hårfarve
- **3** – sort hårfarve
- **F5** – gem aktuelt outfit
- **F9** – indlæs gemt outfit

## Prototype 0.2 indeholder

Ved runtime oprettes nu:

- lukket dressing-room med gulv, vægge og loft
- avatar-podie
- spejlområde
- bænk
- loftlys + avatar key/fill lights
- forbedret mannequin med hals, skuldre, hænder og fødder
- underwear/base layer
- hår-placeholder med tre farve-presets
- fysisk garderobe med kabinet, hylder, skohylde og hængestang
- animerede garderobedøre med hængsler
- 7 strukturerede bøjler
- T-shirt
- bluse
- sweater
- bukser
- kjole
- jakke
- sko
- center-reticle
- interaction prompt
- tøjnavn og kategori
- debug HUD med preview-status, hårfarve og equipped slots

## Testflow

1. Start Play.
2. Kontrollér at HUD vises.
3. Gå hen til garderoben.
4. Peg på garderoben; HUD skal vise **E - Åbn garderobe**.
5. Tryk **E** og kontrollér den glidende døranimation.
6. Peg på en bøjle.
7. HUD skal vise tøjnavn, kategori og **E - Tag ... på**.
8. Tryk **E** og kontrollér at tøjet flyttes fra bøjlen til avatarens slot.
9. Vælg et andet stykke tøj i samme slot og kontrollér, at det gamle returneres.
10. Test **1 / 2 / 3** for hårfarve.
11. Test **P** og roter avatar med musen.
12. Test **F5** og **F9**.

## Kendt prototype-begrænsning

Geometrien er stadig genereret af Unreal Engine-primitiver. Formålet med 0.2 er at få rum, interaktion, garderobe, HUD og slot-logik på plads. Næste større trin er humanoid/rigged avatar, rigtige garments, outfit-browser og foto-/tøjimport.
