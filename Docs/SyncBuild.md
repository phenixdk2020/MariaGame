# MariaGame – Safe Sync + Build

Hovedscript:

```powershell
.\Tools\Sync-And-Build-MariaGame.ps1
```

Version 2.1.0 gør hele flowet:

1. tjekker Git, repo, `.uproject` og Unreal Engine
2. registrerer branch + upstream
3. beskytter lokale ændringer i en automatisk Git stash
4. kører `git fetch --prune origin`
5. kører `git pull --ff-only`
6. kører Git LFS pull, hvis Git LFS findes
7. lægger lokale ændringer tilbage
8. stopper sikkert ved merge/stash-konflikt
9. bygger `MariaGameEditor Win64 Development`
10. udtrækker relevante compiler-fejl
11. skriver fuld log + kort fejlrapport
12. kan starte Unreal Editor efter succes

## Normal brug

Fra repo-roden:

```powershell
.\Tools\Sync-And-Build-MariaGame.ps1
```

Det er den anbefalede kommando til daglig brug.

## Opdater + byg + start Unreal

```powershell
.\Tools\Sync-And-Build-MariaGame.ps1 -LaunchOnSuccess
```

## Kun Git-opdatering

```powershell
.\Tools\Sync-And-Build-MariaGame.ps1 -SkipBuild
```

## Behold lokale ændringer i stash

Hvis du vil hente GitHub-versionen, men ikke automatisk lægge dine lokale ændringer tilbage:

```powershell
.\Tools\Sync-And-Build-MariaGame.ps1 -NoRestoreStash
```

Scriptet viser stash-reference, fx:

```text
stash@{0}
```

## Kræv clean working tree

Hvis du ikke ønsker automatisk stash:

```powershell
.\Tools\Sync-And-Build-MariaGame.ps1 -NoAutoStash
```

Ved lokale ændringer stopper scriptet uden at ændre noget.

## DebugGame build

```powershell
.\Tools\Sync-And-Build-MariaGame.ps1 -Configuration DebugGame
```

## Sikkerhed

Scriptet bruger **ikke**:

- `git reset --hard`
- force checkout
- force pull
- automatisk rollback
- automatisk sletning af lokale ændringer

Hvis en stash ikke kan lægges tilbage rent, beholdes den i Git.

## Logs

Fuld sessionlog:

```text
Saved\BuildLogs\SyncBuild_YYYYMMDD_HHMMSS.log
```

Fuld buildlog:

```text
Saved\BuildLogs\Build_YYYYMMDD_HHMMSS.log
```

Seneste kondenserede fejlrapport:

```text
Saved\BuildLogs\LatestBuildErrors.txt
```

Ved compile-fejl viser konsollen de første relevante fejl direkte.

## Kendte fejl scriptet genkender

### Live Coding

Hvis build-output indeholder:

```text
Unable to build while Live Coding is active
```

får du en specifik besked om at lukke Unreal Editor eller deaktivere Live Coding.

### Nyere MSVC end UE foretrækker

Advarslen om at Visual Studio compiler-versionen er nyere end Unreal Engines foretrukne version markeres som en warning og behandles ikke som den egentlige compile-fejl.

### Git branch divergeret

`git pull --ff-only` fejler i stedet for at overskrive eller rebase automatisk. Scriptet foretager ingen destruktiv handling.

### Konflikt ved restore af lokale ændringer

Scriptet:

- stopper
- viser konfliktfilerne
- **sletter ikke stashen**

## Exit-koder

| Kode | Betydning |
|---:|---|
| 0 | Sync/build lykkedes |
| 2 | Lokale ændringer + `-NoAutoStash` |
| 10 | Manglende prerequisite/path |
| 20 | Git fetch/pull fejlede |
| 21 | Branch mangler upstream |
| 22 | Detached HEAD |
| 23 | Konflikt ved restore af auto-stash |
| 30 | Unreal build/compile-fejl |
| 31 | Live Coding blokerer build |
| 40 | Unreal Editor kunne ikke startes |
| 99 | Uventet scriptfejl |

## Typisk dagligt workflow

```powershell
Set-Location "R:\Onedrive\Unreal\MariaGame"
.\Tools\Sync-And-Build-MariaGame.ps1 -LaunchOnSuccess
```

Det er fremover den kommando, der bør erstatte separate `git pull`- og `Build.bat`-kommandoer.


## v2.1 fix

Version 2.0 kunne fejle, hvis `Tools\BuildLogs` var untracked og scriptet samtidig kørte `git stash --include-untracked`. Git kunne da stash'e selve den mappe, som scriptet skrev loggen til. Unreal/OneDrive-låste filer som `Content/Collections` kunne også få stash-operationen til at fejle.

Version 2.1 ændrer derfor modellen:

- logs ligger i `Saved\BuildLogs`
- kun **tracked** ændringer auto-stashes
- untracked filer røres ikke
- untracked filer sammenlignes med incoming Git-paths før pull
- gamle `MariaGame AutoStash` entries vises, men ændres ikke automatisk
- `Tools\BuildLogs` og `Content\Collections` ignoreres fremover

Det betyder, at editor-genererede eller låste untracked filer ikke længere skal kunne ødelægge sync/build-flowet.
