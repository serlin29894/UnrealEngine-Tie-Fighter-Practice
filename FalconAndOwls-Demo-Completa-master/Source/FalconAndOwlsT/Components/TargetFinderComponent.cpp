// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetFinderComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "MyDefs.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/Camera/PlayerCameraManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"


// Sets default values for this component's properties
UTargetFinderComponent::UTargetFinderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTargetFinderComponent::Start()
{
	if (TimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	switch (WorkingMode)
	{
	case ETargetFinderMode::Inactive:
		break;
	case ETargetFinderMode::SingleTarget:
		FindTargets();
		break;
	case ETargetFinderMode::TargetSweeping:
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTargetFinderComponent::FindTargets, TimeInTarget, true, 0.f);
		break;
	case ETargetFinderMode::CameraSlave:
		pcm = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		break;
	}

	bIsWorking = true;
}

void UTargetFinderComponent::Stop()
{
	bIsWorking = false;

	if (TimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

// Called when the game starts
void UTargetFinderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTargetFinderComponent::FindTargets()
{
	//UE_LOG(LogTemp, Warning, TEXT("Searching Targets..."))

	TArray<FHitResult> Hit;
	// GetAttachmentRootActor consigue la rotacion del objeto padre
	FVector RotatedDirection = GetAttachmentRootActor()->GetActorRotation().RotateVector(TargetArea.Direction);
	FVector Start = GetComponentLocation() + RotatedDirection*TargetArea.Offset;
	FVector End = Start + FVector(0.f, 0.f, 1.f);
	FCollisionObjectQueryParams oqp;
	oqp.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	oqp.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	FCollisionQueryParams qp;
	qp.AddIgnoredActor(GetOwner());
	qp.bTraceComplex = false;
	qp.bReturnPhysicalMaterial = false;


#if !UE_BUILD_SHIPPING
	//const FName tag("QueryTargetFinder");
	//qp.TraceTag = tag;
	//GetWorld()->DebugDrawTraceTag = tag;
#endif

	GetWorld()->SweepMultiByObjectType(Hit, Start, End, GetOwner()->GetActorQuat(), oqp, FCollisionShape::MakeBox(FVector(TargetArea.Radius)), qp);
	if (Hit.Num() == 0)
	{
		CurrentTarget = nullptr;
		CurrentTargetComponent = nullptr;
		return;
	}


	
	for (FHitResult hit : Hit)
	{
		if (TargetTypes.Contains(hit.GetActor()->GetClass()))
		{
			if (hit.GetActor() != CurrentTarget)
			{
				CurrentTarget = hit.GetActor();
				CurrentTargetComponent = hit.GetComponent();
				

				if (hit.Item < 0) 
				{
					CurrentTargetLocation = hit.GetComponent()->GetComponentLocation();
				}
				else 
				{
					UHierarchicalInstancedStaticMeshComponent *hism = Cast<UHierarchicalInstancedStaticMeshComponent>(CurrentTargetComponent);

					if (hism) 
					{
						FTransform t;

						hism->GetInstanceTransform(hit.Item, t,true);

						CurrentTargetLocation = t.GetLocation();

					}

				}
				
				//MYLOG(FString::FormatAsNumber(hit.Item))
				IsTargetFixedNotified = false;
				break;
			}
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Number of targets: %d"), Hit.Num());
	//MYLOG(FString("Number of targets:").Append(FString::FormatAsNumber(Hit.Num())))
	//MYLOG(CurrentTarget->GetClass()->GetName())
}

FHitResult UTargetFinderComponent::FindPlayerCameraHit()
{
	FCollisionQueryParams qp;
	qp.AddIgnoredActor(GetOwner());
	qp.bTraceComplex = false;
	qp.bReturnPhysicalMaterial = false;

	/*
#if !UE_BUILD_SHIPPING
	const FName tag("QueryTargetFinder");
	qp.TraceTag = tag;
	GetWorld()->DebugDrawTraceTag = tag;
#endif
*/
	FHitResult hit;
	FVector Start = pcm->GetCameraLocation();
	FVector End = Start + pcm->GetCameraRotation().RotateVector(FVector(CameraScope, 0.f, 0.f));
	GetWorld()->LineTraceSingleByChannel(hit, Start, End, ECollisionChannel::ECC_Visibility, qp);
	return hit;
}


// Called every frame
void UTargetFinderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!bIsWorking)
		return;

	FRotator Rot;
	switch (WorkingMode)
	{
	case ETargetFinderMode::Inactive:
		return;
		break;
	case ETargetFinderMode::CameraSlave:
	{	
		FHitResult hit = FindPlayerCameraHit();
		if (hit.bBlockingHit)
		{
			Rot = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), hit.Location);
			DistanceWhenCameraSlave = hit.Distance;
			TargetTypeWhenCameraSlave = hit.Actor->GetName();
		}
		else
		{
			DistanceWhenCameraSlave = 0.f;
			TargetTypeWhenCameraSlave = TEXT("--");
		}

		//MYLOG(pcm->GetCameraRotation().ToString())
	}
		break;
	case ETargetFinderMode::SingleTarget:
		if (CurrentTargetComponent == nullptr)
		{
			FindTargets(); // no hay break, reutiliza el codigo de target sweeping
		}
	case ETargetFinderMode::TargetSweeping:
		if (CurrentTargetComponent == nullptr)
			return;

		Rot = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), CurrentTargetLocation);
			
		//MYLOG(Rot.ToString())
		if(!IsTargetFixedNotified)
			if ((GetComponentRotation() - Rot).IsNearlyZero(1.f))
			{
				IsTargetFixedNotified = true;
				OnTargetFixed.Broadcast(CurrentTarget, CurrentTargetComponent);
			}
		break;
	}

	SetWorldRotation(FMath::RInterpTo(GetComponentRotation(), Rot, DeltaTime, 80.f));
}

