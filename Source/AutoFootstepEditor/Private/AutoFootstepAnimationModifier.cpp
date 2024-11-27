// Copyright 2023-2024 Metaseven. All Rights Reserved.

#include "AutoFootstepAnimationModifier.h"
#include "Animation/AnimSequence.h"
#include "AnimPose.h"

void UAutoFootstepAnimationModifier::OnApply_Implementation(UAnimSequence* AnimationSequence)
{
	if (!ContainsPathFilter(AnimationSequence))
	{
		if (!AnimationSequence->GetPackage()->IsDirty())
		{
			ClearDirtyFlagHandle = FCoreUObjectDelegates::OnObjectModified.AddUObject(this, &UAutoFootstepAnimationModifier::ClearDirtyFlag, AnimationSequence);
		}

		return;
	}

	if (bAddNotify && !UAnimationBlueprintLibrary::IsValidAnimNotifyTrackName(AnimationSequence, NotifyTrackName))
	{
		UAnimationBlueprintLibrary::AddAnimationNotifyTrack(AnimationSequence, NotifyTrackName);
	}

	if (bAddSyncMarker && !UAnimationBlueprintLibrary::IsValidAnimNotifyTrackName(AnimationSequence, MarkerTrackName))
	{
		UAnimationBlueprintLibrary::AddAnimationNotifyTrack(AnimationSequence, MarkerTrackName);
	}

	struct FLocationsZ
	{
		TArray<double> Value;
		double MinValue = DBL_MAX;

		TArray<int32> GroundedFrames;
		bool bGroundedFirst = false;
		bool bGroundedLast = false;
	};

	TMap<FName, FLocationsZ> FootBoneLocationsZ;
	FootBoneLocationsZ.Empty(FootBoneNames.Num());

	int32 NumKeys;
	FFrameRate FrameRate;

	if (bOverrideFrameRate)
	{
		NumKeys = FrameRateOverride.AsDecimal() * AnimationSequence->GetPlayLength() + 1;
		FrameRate = FrameRateOverride;
	}
	else
	{
		NumKeys = AnimationSequence->GetNumberOfSampledKeys();
		FrameRate = AnimationSequence->GetSamplingFrameRate();
	}

	for (const FName& FootBoneName : FootBoneNames)
	{
		FootBoneLocationsZ.Emplace(FootBoneName).Value.Empty(NumKeys);

		if (bAddCurvesOfLocationZ && !UAnimationBlueprintLibrary::DoesCurveExist(AnimationSequence, FootBoneName, ERawCurveTrackTypes::RCT_Float))
		{
			UAnimationBlueprintLibrary::AddCurve(AnimationSequence, FootBoneName, ERawCurveTrackTypes::RCT_Float);
		}
	}

	// Find the location Z of each foot bone in each frame
	for (int32 Frame = 0; Frame < NumKeys; ++Frame)
	{
		const double Time = FMath::Clamp(FrameRate.AsSeconds(Frame), 0.0, AnimationSequence->GetPlayLength());

		FAnimPose AnimPose;
		UAnimPoseExtensions::GetAnimPoseAtTime(AnimationSequence, Time, FAnimPoseEvaluationOptions(), AnimPose);

		for (const FName& FootBoneName : FootBoneNames)
		{
			const double LocationZ = UAnimPoseExtensions::GetBonePose(AnimPose, FootBoneName, EAnimPoseSpaces::World).GetLocation().Z;

			if (bAddCurvesOfLocationZ)
			{
				UAnimationBlueprintLibrary::AddFloatCurveKey(AnimationSequence, FootBoneName, Time + 0.0001, LocationZ);
			}

			FLocationsZ& LocationsZ = FootBoneLocationsZ[FootBoneName];
			LocationsZ.Value.Add(LocationZ);

			if (LocationZ < LocationsZ.MinValue)
			{
				LocationsZ.MinValue = LocationZ;
			}
		}
	}

	// Find the frame where each foot bone is grounded
	for (const FName& FootBoneName : FootBoneNames)
	{
		FLocationsZ& LocationsZ = FootBoneLocationsZ[FootBoneName];
		bool bGrounded = false;

		for (int32 Frame = 0; Frame < NumKeys; ++Frame)
		{
			if (LocationsZ.Value[Frame] < LocationsZ.MinValue + GroundHeightRange)
			{
				if (Frame == 0)
				{
					LocationsZ.bGroundedFirst = true;
				}
				else if (Frame == NumKeys - 1)
				{
					LocationsZ.bGroundedLast = true;
				}

				if (!bGrounded)
				{
					bGrounded = true;
					LocationsZ.GroundedFrames.Add(Frame);
					continue;
				}

				if (LocationsZ.Value[LocationsZ.GroundedFrames.Last()] - LocationsZ.Value[Frame] > GroundedThreshold)
				{
					LocationsZ.GroundedFrames.Last() = Frame;
				}

				continue;
			}

			bGrounded = false;
		}
	}

	// Add notifies or sync markers to the frame where each foot bone is grounded
	for (const FName& FootBoneName : FootBoneNames)
	{
		FLocationsZ& LocationsZ = FootBoneLocationsZ[FootBoneName];

		if (LocationsZ.bGroundedFirst && LocationsZ.bGroundedLast)
		{
			if (LocationsZ.Value[LocationsZ.GroundedFrames.Last()] - LocationsZ.Value[LocationsZ.GroundedFrames[0]] > GroundedThreshold)
			{
				LocationsZ.GroundedFrames.Last() = LocationsZ.GroundedFrames[0];
			}

			LocationsZ.GroundedFrames.RemoveAt(0);
		}

		for (const int32 GroundedFrame : LocationsZ.GroundedFrames)
		{
			const double Time = FMath::Clamp(FrameRate.AsSeconds(GroundedFrame), 0.0, AnimationSequence->GetPlayLength());

			if (bAddNotify)
			{
				UAnimNotify* AnimNotify = UAnimationBlueprintLibrary::AddAnimationNotifyEvent(AnimationSequence, NotifyTrackName, Time, NotifyClass);
				
				if (UAutoFootstepAnimNotify* AutoFootstepAnimNotify = Cast<UAutoFootstepAnimNotify>(AnimNotify))
				{
					AutoFootstepAnimNotify->SetParams(FootBoneName, NotifyParams);
				}
				
				OnNotifyAdded(AnimNotify, FootBoneName);
			}

			if (bAddSyncMarker)
			{
				const FName MarkerName = MarkerNamesByFootBone.FindRef(FootBoneName);
				UAnimationBlueprintLibrary::AddAnimationSyncMarker(AnimationSequence, MarkerName, Time, MarkerTrackName);
			}
		}
	}
}

void UAutoFootstepAnimationModifier::OnRevert_Implementation(UAnimSequence* AnimationSequence)
{
	if (bAddNotify)
	{
		AnimationSequence->Notifies.RemoveAll([this](const FAnimNotifyEvent& Event)
		{
			return Event.Notify.GetClass() == NotifyClass;
		});

		AnimationSequence->RefreshCacheData();

		if (UAnimationBlueprintLibrary::IsValidAnimNotifyTrackName(AnimationSequence, NotifyTrackName))
		{
			UAnimationBlueprintLibrary::RemoveAnimationNotifyTrack(AnimationSequence, NotifyTrackName);
		}
	}

	if (bAddSyncMarker)
	{
		for (const FName& FootBoneName : FootBoneNames)
		{
			UAnimationBlueprintLibrary::RemoveAnimationSyncMarkersByName(AnimationSequence, MarkerNamesByFootBone[FootBoneName]);
		}

		if (UAnimationBlueprintLibrary::IsValidAnimNotifyTrackName(AnimationSequence, MarkerTrackName))
		{
			UAnimationBlueprintLibrary::RemoveAnimationNotifyTrack(AnimationSequence, MarkerTrackName);
		}
	}

	if (bAddCurvesOfLocationZ)
	{
		for (const FName& FootBoneName : FootBoneNames)
		{
			if (UAnimationBlueprintLibrary::DoesCurveExist(AnimationSequence, FootBoneName, ERawCurveTrackTypes::RCT_Float))
			{
				UAnimationBlueprintLibrary::RemoveCurve(AnimationSequence, FootBoneName);
			}
		}
	}
}

void UAutoFootstepAnimationModifier::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!PropertyChangedEvent.MemberProperty)
	{
		return;
	}

	if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UAutoFootstepAnimationModifier, FootBoneNames))
	{
		TMap<FName, FName> NewMarkerNames;

		for (const FName& FootBoneName : FootBoneNames)
		{
			FName MarkerName = MarkerNamesByFootBone.FindRef(FootBoneName);

			if (MarkerName.IsNone())
			{
				MarkerName = FootBoneName;
			}

			NewMarkerNames.Emplace(FootBoneName, MarkerName);
		}

		MarkerNamesByFootBone = NewMarkerNames;
	}
	else if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UAutoFootstepAnimationModifier, NotifyClass))
	{
		bShowNotifyParams = NotifyClass && NotifyClass->IsChildOf(UAutoFootstepAnimNotify::StaticClass());
	}
}

bool UAutoFootstepAnimationModifier::ContainsPathFilter(const UAnimSequence* AnimationSequence)
{
	if (PathFilters.IsEmpty())
	{
		return true;
	}

	for (const FString& PathFilter : PathFilters)
	{
		if (AnimationSequence->GetPathName().Contains(PathFilter, ESearchCase::CaseSensitive))
		{
			return true;
		}
	}

	return false;
}

void UAutoFootstepAnimationModifier::ClearDirtyFlag(UObject* Object, UAnimSequence* AnimationSequence)
{
	if (Object == AnimationSequence)
	{
		AnimationSequence->GetPackage()->SetDirtyFlag(false);
		FCoreUObjectDelegates::OnObjectModified.Remove(ClearDirtyFlagHandle);
	}
}
