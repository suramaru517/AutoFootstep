// Copyright 2023-2024 Metaseven. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "AutoFootstepAnimNotify.h"
#include "AutoFootstepAnimationModifier.generated.h"

UCLASS()
class AUTOFOOTSTEPEDITOR_API UAutoFootstepAnimationModifier : public UAnimationModifier
{
	GENERATED_BODY()

public:
	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;
	virtual void OnRevert_Implementation(UAnimSequence* AnimationSequence) override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "AutoFootstep")
	void OnNotifyAdded(UObject* AnimNotify, const FName& FootBoneName);

private:
	bool ContainsPathFilter(const UAnimSequence* AnimationSequence);
	void ClearDirtyFlag(UObject* Object, UAnimSequence* AnimationSequence);

	UPROPERTY(EditAnywhere, Category = "AutoFootstep")
	TArray<FString> PathFilters;

	FDelegateHandle ClearDirtyFlagHandle;

	static inline const FName NAME_foot_l = TEXT("foot_l");
	static inline const FName NAME_foot_r = TEXT("foot_r");

	UPROPERTY(EditAnywhere, Category = "AutoFootstep")
	TSet<FName> FootBoneNames = { NAME_foot_l, NAME_foot_r };

	UPROPERTY(EditAnywhere, Category = "AutoFootstep|Notify")
	bool bAddNotify = true;

	UPROPERTY(EditAnywhere, Category = "AutoFootstep|Notify", meta = (EditCondition = "bAddNotify"))
	TSubclassOf<UAnimNotify> NotifyClass = UAutoFootstepAnimNotify::StaticClass();

	UPROPERTY()
	bool bShowNotifyParams = true;

	UPROPERTY(EditAnywhere, Category = "AutoFootstep|Notify", meta = (EditCondition = "bAddNotify && bShowNotifyParams", EditConditionHides))
	FAutoFootstepNotifyParams NotifyParams;

	static inline const FName NAME_AutoFootstep_Notify = TEXT("AutoFootstep_Notify");

	UPROPERTY(EditAnywhere, Category = "AutoFootstep|Notify", meta = (EditCondition = "bAddNotify"))
	FName NotifyTrackName = NAME_AutoFootstep_Notify;

	UPROPERTY(EditAnywhere, Category = "AutoFootstep|Sync Marker")
	bool bAddSyncMarker = false;

	static inline const FName NAME_L = TEXT("L");
	static inline const FName NAME_R = TEXT("R");

	UPROPERTY(EditAnywhere, EditFixedSize, Category = "AutoFootstep|Sync Marker", meta = (EditCondition = "bAddSyncMarker", ReadOnlyKeys))
	TMap<FName, FName> MarkerNamesByFootBone = { { NAME_foot_l, NAME_L }, { NAME_foot_r, NAME_R } };

	static inline const FName NAME_AutoFootstep_SyncMarker = TEXT("AutoFootstep_SyncMarker");

	UPROPERTY(EditAnywhere, Category = "AutoFootstep|Sync Marker", meta = (EditCondition = "bAddSyncMarker"))
	FName MarkerTrackName = NAME_AutoFootstep_SyncMarker;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "AutoFootstep", meta = (InlineEditConditionToggle))
	bool bOverrideFrameRate = false;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "AutoFootstep", meta = (EditCondition = "bOverrideFrameRate"))
	FFrameRate FrameRateOverride = FFrameRate(60, 1);

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "AutoFootstep")
	double GroundHeightRange = 5.0;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "AutoFootstep")
	double GroundedThreshold = 0.5;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "AutoFootstep")
	bool bAddCurvesOfLocationZ = false;
};
