# MariaGame – Fotoindtag

Denne guide beskriver de billeder, der senere bruges til den rigtige Maria-avatar og til personligt tøj.

## Privat placering

Kildebilleder må ikke committes til GitHub.

Brug fx:

```text
Private/
  Maria/
    Face/
    Body/
  Clothing/
    Sweater01/
    Dress01/
```

eller:

```text
Imports/SourcePhotos/
```

Disse mapper er ignoreret af Git.

## Avatar – ansigt

Minimum:

1. FaceFront
2. FaceLeft45
3. FaceRight45

Bedre sæt:

4. FaceLeftProfile
5. FaceRightProfile

Retningslinjer:

- neutralt ansigtsudtryk
- kamera omtrent i øjenhøjde
- jævnt lys
- ingen filter/beauty mode
- hår væk fra så meget af ansigtet som muligt
- undgå meget vidvinkel tæt på ansigtet

## Avatar – krop

Minimum:

1. BodyFront
2. BodySide

Bedre sæt:

3. BodyBack

Tætsiddende almindeligt træningstøj er tilstrækkeligt. Formålet er silhouette og proportioner.

## Tøj

Minimum pr. stykke:

1. front

Anbefalet:

1. front
2. back
3. side-left eller side-right
4. fabric-detail
5. logo/detail, hvis relevant

Eksempel:

```text
Private/Clothing/Sweater01/
  front.jpg
  back.jpg
  side.jpg
  fabric.jpg
  detail.jpg
```

## Hvad MariaGame allerede har klar

Kodefundamentet understøtter allerede billedrollerne:

- Front
- Back
- LeftSide
- RightSide
- Detail

Importjobbet har statusfaser til:

1. WaitingForImages
2. Ready
3. Validating
4. Analyzing
5. TemplateMatch
6. MaterialBuild
7. BodyFit
8. Complete / Failed

Når billederne er til rådighed, kobles analyse- og 3D-delen ind mellem disse trin.
