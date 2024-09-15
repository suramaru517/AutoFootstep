// Copyright 2023-2024 Metaseven. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraCommon.h"
#include "AutoFootstepEffectContext.generated.h"

class USoundBase;
enum EPhysicalSurface : int;

USTRUCT(BlueprintType)
struct FAutoFootstepNiagaraParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	FVector LocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	FRotator RotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	FVector Scale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	ENCPoolMethod PoolingMethod = ENCPoolMethod::AutoRelease;
};

USTRUCT(BlueprintType)
struct FAutoFootstepSoundParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	FVector LocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	FRotator RotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	float VolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	float PitchMultiplier = 1.0f;
};

USTRUCT(BlueprintType)
struct FAutoFootstepEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	TSoftObjectPtr<UNiagaraSystem> Niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoFootstep")
	TSoftObjectPtr<USoundBase> Sound;
};

UCLASS()
class AUTOFOOTSTEP_API UAutoFootstepEffectContext : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AutoFootstep", meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "NiagaraParams, SoundParams", AdvancedDisplay = 4))
	void PlayEffectBySurfaceType(
		const UObject* WorldContextObject,
		const EPhysicalSurface SurfaceType,
		const FVector& Location,
		const FRotator& Rotation,
		const FAutoFootstepNiagaraParams& NiagaraParams = FAutoFootstepNiagaraParams(),
		const FAutoFootstepSoundParams& SoundParams = FAutoFootstepSoundParams());

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "AutoFootstep")
	void AddAllSurfaceTypeElements();
#endif

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AutoFootstep")
	TMap<TEnumAsByte<EPhysicalSurface>, FAutoFootstepEffect> EffectsBySurfaceType;
};
