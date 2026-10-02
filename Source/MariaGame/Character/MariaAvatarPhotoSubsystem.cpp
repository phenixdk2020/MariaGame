#include "MariaAvatarPhotoSubsystem.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"

bool UMariaAvatarPhotoSubsystem::ValidatePhoto(const FString& FilePath, FString& OutMessage) const
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
        OutMessage = TEXT("Filen findes ikke.");
        return false;
    }

    OutMessage = TEXT("OK");
    return true;
}

bool UMariaAvatarPhotoSubsystem::AddPhoto(const FString& FilePath, EMariaAvatarPhotoView View)
{
    FMariaAvatarSourcePhoto Photo;
    Photo.FilePath = FilePath;
    Photo.View = View;
    Photo.bValid = ValidatePhoto(FilePath, Photo.ValidationMessage);

    PhotoSet.Photos.RemoveAll([&](const FMariaAvatarSourcePhoto& Existing)
    {
        return Existing.View == View;
    });

    PhotoSet.Photos.Add(Photo);
    RecalculateCompleteness();
    return Photo.bValid;
}

void UMariaAvatarPhotoSubsystem::ClearPhotos()
{
    PhotoSet = FMariaAvatarPhotoSet();
}

void UMariaAvatarPhotoSubsystem::RecalculateCompleteness()
{
    auto HasView = [&](EMariaAvatarPhotoView View)
    {
        return PhotoSet.Photos.ContainsByPredicate([&](const FMariaAvatarSourcePhoto& Photo)
        {
            return Photo.bValid && Photo.View == View;
        });
    };

    const int32 FaceViews =
        (HasView(EMariaAvatarPhotoView::FaceFront) ? 1 : 0) +
        (HasView(EMariaAvatarPhotoView::FaceLeft45) ? 1 : 0) +
        (HasView(EMariaAvatarPhotoView::FaceRight45) ? 1 : 0) +
        (HasView(EMariaAvatarPhotoView::FaceLeftProfile) ? 1 : 0) +
        (HasView(EMariaAvatarPhotoView::FaceRightProfile) ? 1 : 0);

    const int32 BodyViews =
        (HasView(EMariaAvatarPhotoView::BodyFront) ? 1 : 0) +
        (HasView(EMariaAvatarPhotoView::BodySide) ? 1 : 0) +
        (HasView(EMariaAvatarPhotoView::BodyBack) ? 1 : 0);

    PhotoSet.FaceCompleteness = static_cast<float>(FaceViews) / 5.0f;
    PhotoSet.BodyCompleteness = static_cast<float>(BodyViews) / 3.0f;

    PhotoSet.bMinimumFaceSetReady =
        HasView(EMariaAvatarPhotoView::FaceFront) &&
        HasView(EMariaAvatarPhotoView::FaceLeft45) &&
        HasView(EMariaAvatarPhotoView::FaceRight45);

    PhotoSet.bMinimumBodySetReady =
        HasView(EMariaAvatarPhotoView::BodyFront) &&
        HasView(EMariaAvatarPhotoView::BodySide);
}
