// Copyright 2023 Metaseven. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AutoFootstepEffectContext.h"
#include "Components/SkeletalMeshComponent.h"
#include "AutoFootstepAnimNotify.generated.h"

USTRUCT(BlueprintType)
struct FAutoFootstepTraceParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	FVector Offset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	float Length = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	TEnumAsByte<ETraceTypeQuery> Channel = TraceTypeQuery1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	bool bTraceComplex = false;
};

USTRUCT(BlueprintType)
struct FAutoFootstepNoiseParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	bool bReportNoise = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep", meta = (EditCondition = "bReportNoise"))
	float Loudness = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep", meta = (EditCondition = "bReportNoise"))
	float MaxRange = 0.0f;

	static inline const FName NAME_AutoFootstep = TEXT("AutoFootstep");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep", meta = (EditCondition = "bReportNoise"))
	FName Tag = NAME_AutoFootstep;
};

USTRUCT(BlueprintType)
struct FAutoFootstepNotifyParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	TObjectPtr<UAutoFootstepEffectContext> EffectContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	FAutoFootstepTraceParams TraceParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	FAutoFootstepNiagaraParams NiagaraParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	FAutoFootstepSoundParams SoundParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	FAutoFootstepNoiseParams NoiseParams;
};

UCLASS()
class AUTOFOOTSTEP_API UAutoFootstepAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAutoFootstepAnimNotify();

	virtual FString GetNotifyName_Implementation() const override;

	void Init(const FName& InFootBoneName, const FAutoFootstepNotifyParams& InNotifyParams);

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	void OnTraceCompleted(const FTraceHandle& TraceHandle, FTraceDatum& TraceDatum, USkeletalMeshComponent* MeshComp);

	UPROPERTY(EditAnywhere, Category = "AutoFootstep", meta = (AnimNotifyBoneName = true))
	FName FootBoneName = NAME_None;

	UPROPERTY(EditAnywhere, Category = "AutoFootstep")
	FAutoFootstepNotifyParams NotifyParams;
};
