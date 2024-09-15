// Copyright 2023-2024 Metaseven. All Rights Reserved.

#include "AutoFootstepAnimNotify.h"
#include "DrawDebugHelpers.h"
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

void UAutoFootstepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	MeshContext = MeshComp;

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	if (const AActor* MeshOwner = MeshComp->GetOwner())
	{
		const FVector Start = MeshComp->GetSocketLocation(FootBoneName) + NotifyParams.TraceParams.Offset;
		const FVector End = Start - MeshComp->GetUpVector() * NotifyParams.TraceParams.Length;
		const ECollisionChannel TraceChannel = UEngineTypes::ConvertToCollisionChannel(NotifyParams.TraceParams.Channel);

		FCollisionQueryParams Params(NAME_None, NotifyParams.TraceParams.bTraceComplex, MeshOwner);
		Params.AddIgnoredActors(MeshOwner->Children);
		Params.bReturnPhysicalMaterial = true;

		FTraceDelegate Delegate = FTraceDelegate::CreateUObject(this, &UAutoFootstepAnimNotify::OnTraceCompleted);

		World->AsyncLineTraceByChannel(EAsyncTraceType::Single, Start, End, TraceChannel, Params, FCollisionResponseParams::DefaultResponseParam, &Delegate);

#if ENABLE_DRAW_DEBUG
		if (NotifyParams.TraceParams.bDrawDebugLine)
		{
			::DrawDebugLine(World, Start, End, NotifyParams.TraceParams.DebugLineColor, false, NotifyParams.TraceParams.DebugLineLifeTime, 0, 2.0f);
		}
#endif
	}

	Received_Notify(MeshComp, Animation, EventReference);
}

FLinearColor UAutoFootstepAnimNotify::GetEditorColor()
{
#if ENABLE_DRAW_DEBUG
	if (NotifyParams.TraceParams.bDrawDebugLine)
	{
		return FLinearColor(NotifyParams.TraceParams.DebugLineColor);
	}
#endif

	return Super::GetEditorColor();
}

UWorld* UAutoFootstepAnimNotify::GetWorld() const
{
	return MeshContext.IsValid() ? MeshContext->GetWorld() : nullptr;
}

void UAutoFootstepAnimNotify::SetParams(const FName& InFootBoneName, const FAutoFootstepNotifyParams& InNotifyParams)
{
	FootBoneName = InFootBoneName;
	NotifyParams = InNotifyParams;
}

void UAutoFootstepAnimNotify::OnTraceCompleted(const FTraceHandle& TraceHandle, FTraceDatum& TraceDatum)
{
	if (TraceDatum.OutHits.IsEmpty())
	{
		return;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	const FHitResult& HitResult = TraceDatum.OutHits[0];
	const FVector& Location = HitResult.ImpactPoint;

	if (NotifyParams.EffectContext)
	{
		const EPhysicalSurface SurfaceType = HitResult.PhysMaterial.IsValid() ? HitResult.PhysMaterial->SurfaceType.GetValue() : SurfaceType_Default;
		const FRotator Rotation = MeshContext->GetComponentRotation() + FRotator(0.0, 90.0, 0.0);

		NotifyParams.EffectContext->PlayEffectBySurfaceType(World, SurfaceType, Location, Rotation, NotifyParams.NiagaraParams, NotifyParams.SoundParams);
	}

	if (NotifyParams.NoiseParams.bReportNoise)
	{
		UAISense_Hearing::ReportNoiseEvent(World, Location, NotifyParams.NoiseParams.Loudness, MeshContext->GetOwner(), NotifyParams.NoiseParams.MaxRange, NotifyParams.NoiseParams.Tag);
	}

#if ENABLE_DRAW_DEBUG
	if (NotifyParams.TraceParams.bDrawDebugLine)
	{
		::DrawDebugPoint(World, Location, 16.0f, NotifyParams.TraceParams.DebugLineColor, false, NotifyParams.TraceParams.DebugLineLifeTime);
	}
#endif

	OnTraceHit(MeshContext.Get(), HitResult);
}
