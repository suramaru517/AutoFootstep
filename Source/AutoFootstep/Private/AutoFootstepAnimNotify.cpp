// Copyright 2023 Metaseven. All Rights Reserved.

#include "AutoFootstepAnimNotify.h"
#include "Engine/World.h"
#include "Perception/AISense_Hearing.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UAutoFootstepAnimNotify::UAutoFootstepAnimNotify()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(194, 199, 177, 255);
#endif
}

FString UAutoFootstepAnimNotify::GetNotifyName_Implementation() const
{
	return FootBoneName.ToString();
}

void UAutoFootstepAnimNotify::Init(const FName& InFootBoneName, const FAutoFootstepNotifyParams& InNotifyParams)
{
	FootBoneName = InFootBoneName;
	NotifyParams = InNotifyParams;
}

void UAutoFootstepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!NotifyParams.EffectContext || !MeshComp)
	{
		return;
	}

	if (const AActor* MeshOwner = MeshComp->GetOwner())
	{
		if (UWorld* World = MeshOwner->GetWorld())
		{
			const FVector Start = MeshComp->GetSocketLocation(FootBoneName) + NotifyParams.TraceParams.Offset;
			const FVector End = Start - MeshComp->GetUpVector() * NotifyParams.TraceParams.Length;
			const ECollisionChannel TraceChannel = UEngineTypes::ConvertToCollisionChannel(NotifyParams.TraceParams.Channel);

			FCollisionQueryParams Params(NAME_None, NotifyParams.TraceParams.bTraceComplex, MeshOwner);
			Params.AddIgnoredActors(MeshOwner->Children);
			Params.bReturnPhysicalMaterial = true;

			FTraceDelegate Delegate = FTraceDelegate::CreateUObject(this, &UAutoFootstepAnimNotify::OnTraceCompleted, MeshComp);

			World->AsyncLineTraceByChannel(EAsyncTraceType::Single, Start, End, TraceChannel, Params, FCollisionResponseParams::DefaultResponseParam, &Delegate);
		}
	}
}

void UAutoFootstepAnimNotify::OnTraceCompleted(const FTraceHandle& TraceHandle, FTraceDatum& TraceDatum, USkeletalMeshComponent* MeshComp)
{
	if (TraceDatum.OutHits.IsEmpty() || !MeshComp)
	{
		return;
	}

	const FHitResult& HitResult = TraceDatum.OutHits[0];

	const TEnumAsByte<EPhysicalSurface> SurfaceType = HitResult.PhysMaterial.IsValid() ? HitResult.PhysMaterial->SurfaceType : TEnumAsByte<EPhysicalSurface>(SurfaceType_Default);
	const FVector& Location = HitResult.Location;
	const FRotator& Rotation = MeshComp->GetComponentRotation() + FRotator(0.0f, 90.0f, 0.0f);

	NotifyParams.EffectContext->PlayEffectBySurfaceType(MeshComp, SurfaceType, Location, Rotation, NotifyParams.NiagaraParams, NotifyParams.SoundParams);

	if (NotifyParams.NoiseParams.bReportNoise)
	{
		UAISense_Hearing::ReportNoiseEvent(MeshComp, Location, NotifyParams.NoiseParams.Loudness, MeshComp->GetOwner(), NotifyParams.NoiseParams.MaxRange, NotifyParams.NoiseParams.Tag);
	}
}

