# MariaGame – 50-opgave udviklingssprint

Mål: gå fra teknisk proof-of-concept til en præsentabel Dressing Room prototype med fungerende avatar, garderobe, tøjvalg, UI og fundament til foto-/tøjimport.

## Statusnøgler
- [x] Færdig
- [ ] Ikke påbegyndt

## Sprint 01 – Foundation og stabilitet
1. [x] Unreal C++ projekt bootstrap
2. [x] UE 5.8 BuildSettingsVersion.V7
3. [x] Modul include paths rettet til UE 5.8
4. [x] Garanteret player pawn spawn
5. [x] Automatisk possession af Maria pawn
6. [x] Midlertidigt gulv genereres ved runtime
7. [x] Midlertidig bagvæg genereres ved runtime
8. [x] Midlertidigt key light
9. [x] Midlertidigt fill light
10. [ ] Egen projekt-startmap i stedet for /Engine/Maps/Entry

## Sprint 02 – Dressing room
11. [x] Byg komplet rum med gulv, fire vægge og loft
12. [x] Tilføj dressing-room zone
13. [x] Tilføj spejlramme
14. [x] Tilføj platform/podie til avatar-preview
15. [x] Tilføj garderobe-zone
16. [x] Tilføj dekorativ bænk
17. [x] Tilføj loftlys
18. [x] Tilføj varmt fill-light
19. [x] Tilføj neutral preview-light
20. [x] Fjern LIGHTING NEEDS TO BE REBUILT ved runtime-prototypen

## Sprint 03 – Avatar v0.2
21. [x] Forbedr dummyens menneskelige proportioner
22. [x] Tilføj hals
23. [x] Tilføj skuldre
24. [x] Tilføj hænder
25. [x] Tilføj fødder
26. [x] Tilføj underwear/base-layer
27. [x] Tilføj hår-placeholder
28. [x] Tilføj tre hårfarve-presets
29. [x] Tilføj runtime hair-color switching
30. [x] Forbedr kameraets højde og framing omkring avatar

## Sprint 04 – Garderobe v0.2
31. [x] Garderobesider, top og bund
32. [x] Garderobe-bagplade
33. [x] Garderobe-hængestang
34. [x] Garderobe-hylde
35. [x] Sko-hylde
36. [x] Garderobedør-hængsler
37. [x] Smooth open/close animation
38. [x] Bloker hanger interaction når døre er lukkede
39. [x] Bedre hanger-geometri
40. [x] Hanger spacing og automatisk fordeling

## Sprint 05 – Tøj og interaction
41. [x] T-shirt placeholder
42. [x] Bluse placeholder
43. [x] Sweater placeholder
44. [x] Bukser placeholder
45. [x] Kjole placeholder
46. [x] Jakke placeholder
47. [x] Sko placeholder
48. [x] Interaction prompt: E - Åbn garderobe / Tag på
49. [x] Clothing name + category popup
50. [x] Debug HUD med equipped slots, focused target og preview-mode

## Efter sprint 50
Næste større blok er:
- Outfit browser med flere save slots
- Avatar creator UI
- Fotoimport til ansigt/krop
- Clothing import screen
- Drag/drop billede af tøj
- Clothing analysis pipeline
- Rigged garments
- Body fitting / morphs
- Material extraction fra foto
- Wardrobe organization, favorites, sortering og filtre
- Produktions-assets og animationer


## Ekstra gennemført uden rigtige billeder

- [x] Vend garderoben korrekt mod spilleren
- [x] Flyt garderoben til bedre position i rummet
- [x] Farve-/roughness-pass på rum, podium, spejl og bænk
- [x] Mørkere spejl-surrogat med metallic/roughness
- [x] Mere afdæmpet lysstyrke i dressing room
- [x] Rundere mannequin-torso og pelvis
- [x] Cylinderarme og cylinderben
- [x] Primitive øjne og næse
- [x] Hudfarve på mannequin
- [x] Undertøjsfarve
- [x] Farvede prototype-garments
- [x] Tøjsilhuetter med sphere/cone/cylinder primitives
- [x] Multi-piece overdel med ærmer
- [x] Multi-piece bukser med to ben
- [x] Jakke med separate ærmer
- [x] Venstre/højre sko
- [x] Item-specifik ærmelængde
- [x] Adskil hanger preview-scale fra avatar fit-scale
- [x] Fysisk garderobe-interiørlys
- [x] Dørgreb på garderoben
- [x] Center divider i garderoben
- [x] Foldede tøjstakke på hylden
- [x] Sko-props på skohylden
- [x] Preview zoom med musehjul
- [x] Front/bag/venstre/højre preview views
- [x] Tre outfit save slots
- [x] Import-panel i HUD
- [x] Clothing import job/status data model
- [x] Clothing source image roles: front/back/sider/detail
- [x] Clothing source file validation
- [x] Avatar reference photo data model
- [x] Avatar face/body completeness model
- [x] Clothing DataAsset type
- [x] Hair style DataAsset type
- [x] Private photo folders i .gitignore
- [x] Git LFS-regler for Unreal/3D assets
- [x] Update-MariaGame.ps1
- [x] Build-MariaGame.ps1
- [x] Sync-And-Build-MariaGame.ps1
- [x] Fotoindtagsguide
