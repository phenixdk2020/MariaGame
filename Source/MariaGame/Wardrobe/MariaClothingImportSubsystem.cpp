#include "MariaClothingImportSubsystem.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"

FGuid UMariaClothingImportSubsystem::CreateImportJob(const FText& DisplayName)
{
    FMariaClothingImportJob& Job = Jobs.AddDefaulted_GetRef();
    Job.JobId = FGuid::NewGuid();
    Job.DisplayName = DisplayName;
    Job.Status = EMariaClothingImportStatus::WaitingForImages;
    Job.StatusMessage = TEXT("Tilføj mindst ét frontbillede.");
    Job.Progress = 0.0f;
    return Job.JobId;
}

bool UMariaClothingImportSubsystem::ValidateSourcePath(const FString& FilePath, FString& OutMessage) const
{
    if (FilePath.IsEmpty())
    {
        OutMessage = TEXT("Filstien er tom.");
        return false;
    }

    const FString Extension = FPaths::GetExtension(FilePath).ToLower();
    const bool bSupported =
        Extension == TEXT("jpg") ||
        Extension == TEXT("jpeg") ||
        Extension == TEXT("png") ||
        Extension == TEXT("webp");

    if (!bSupported)
    {
        OutMessage = TEXT("Understøttede formater: JPG, JPEG, PNG og WEBP.");
        return false;
    }

    if (!IFileManager::Get().FileExists(*FilePath))
    {
        OutMessage = TEXT("Filen findes ikke på den angivne sti.");
        return false;
    }

    OutMessage = TEXT("OK");
    return true;
}

FMariaClothingImportJob* UMariaClothingImportSubsystem::FindMutableJob(FGuid JobId)
{
    return Jobs.FindByPredicate([&](const FMariaClothingImportJob& Job)
    {
        return Job.JobId == JobId;
    });
}

bool UMariaClothingImportSubsystem::AddSourceImage(FGuid JobId, const FString& FilePath, EMariaClothingImageView View)
{
    FMariaClothingImportJob* Job = FindMutableJob(JobId);
    if (!Job)
    {
        return false;
    }

    FMariaClothingSourceImage Image;
    Image.FilePath = FilePath;
    Image.View = View;
    Image.bValid = ValidateSourcePath(FilePath, Image.ValidationMessage);
    Job->SourceImages.Add(Image);

    const bool bHasValidFront = Job->SourceImages.ContainsByPredicate([](const FMariaClothingSourceImage& Candidate)
    {
        return Candidate.bValid && Candidate.View == EMariaClothingImageView::Front;
    });

    if (bHasValidFront)
    {
        Job->Status = EMariaClothingImportStatus::Ready;
        Job->StatusMessage = TEXT("Klar til analyse.");
        Job->Progress = 0.10f;
    }
    else
    {
        Job->Status = EMariaClothingImportStatus::WaitingForImages;
        Job->StatusMessage = TEXT("Et gyldigt frontbillede mangler.");
        Job->Progress = 0.0f;
    }

    return Image.bValid;
}

bool UMariaClothingImportSubsystem::RemoveImportJob(FGuid JobId)
{
    return Jobs.RemoveAll([&](const FMariaClothingImportJob& Job)
    {
        return Job.JobId == JobId;
    }) > 0;
}

bool UMariaClothingImportSubsystem::CancelImportJob(FGuid JobId)
{
    FMariaClothingImportJob* Job = FindMutableJob(JobId);
    if (!Job)
    {
        return false;
    }

    Job->Status = EMariaClothingImportStatus::Cancelled;
    Job->StatusMessage = TEXT("Import annulleret.");
    return true;
}

bool UMariaClothingImportSubsystem::GetImportJob(FGuid JobId, FMariaClothingImportJob& OutJob) const
{
    const FMariaClothingImportJob* Job = Jobs.FindByPredicate([&](const FMariaClothingImportJob& Candidate)
    {
        return Candidate.JobId == JobId;
    });

    if (!Job)
    {
        return false;
    }

    OutJob = *Job;
    return true;
}
