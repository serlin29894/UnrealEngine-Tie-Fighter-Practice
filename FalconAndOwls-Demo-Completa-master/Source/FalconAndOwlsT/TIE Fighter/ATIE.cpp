// Fill out your copyright notice in the Description page of Project Settings.

#include "ATIE.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MyDefs.h"
#include "Classes/Engine/Engine.h"
#include "TimerManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "AsteroidsField/AsteroidsField.h"
#include "DrawDebugHelpers.h"


// Sets default values
AATIE::AATIE()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	        
	TIEMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tie Fighter Mesh"));
	RootComponent = TIEMesh;

}

void AATIE::ArriveAttackDistance(float distanceTarget, float limitDistance)
{
	// check if arrive limit distance 
	if (distanceTarget < limitDistance)
	{
		//Send event On Falcon Hanuted
		OnFalconHaunted(true);

		// decrese counter time
		CounterTime -= GetWorld()->GetDeltaSeconds();
		if (CounterTime <= 0)
		{
			OnTIEShoot();
			// reset counter time
			CounterTime = FMath::RandRange(0.5f, 2.0f);
		}

	}
	else
	{

		OnFalconHaunted(false);
		// reset counter time if is greater than 2 or les than 0.5 
		if (CounterTime > 2 || CounterTime < 0.5)
		{
			CounterTime = FMath::RandRange(0.5f, 2.0f);
		}
	}
}

void AATIE::CheckObstacles()
{
	TArray<FHitResult> Hit;


	FVector RotatedDirection = GetActorRotation().RotateVector(CheckDirection);
	FVector Start = GetActorLocation() + RotatedDirection*CheckOffset;
	FVector End = Start + FVector(0.f, 0.f, 1.f);

	FCollisionObjectQueryParams oqp;
	oqp.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	oqp.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	FCollisionQueryParams qp;
	qp.AddIgnoredActor(this);
	qp.bTraceComplex = false;
	qp.bReturnPhysicalMaterial = false;

	if (ShowDebug) 
	{
		const FName tag("QueryCheckObstacles");
		qp.TraceTag = tag;
		GetWorld()->DebugDrawTraceTag = tag;
	}

	GetWorld()->SweepMultiByObjectType(Hit, Start, End, this->GetActorQuat(), oqp, FCollisionShape::MakeBox(FVector(CheckBoxProfundity,CheckBoxWitdth, CheckBoxHeight)), qp);
	
	for (FHitResult hit : Hit) 
	{
		if (hit.Item < 0)
		{
		}
		else
		{
	
			// get  component of hit 
			temp_COMP = hit.GetComponent();
			// get  actor of hit 
			temp_AC = hit.GetActor();
			
			UHierarchicalInstancedStaticMeshComponent *hism = Cast<UHierarchicalInstancedStaticMeshComponent>(temp_COMP);
			
			AAsteroidsField *asf = Cast<AAsteroidsField>(temp_AC);
			
			if (asf && hism)
			{
				//get target hit transform
				FTransform targetHitTransform;
				hism->GetInstanceTransform(hit.Item, targetHitTransform, true);
				
				// get target hit object radius
				float temp_Radius;
				temp_Radius = asf->GetAsteroidRadius(hit.Item);
				
				FString RadiusString = FString::SanitizeFloat(temp_Radius);
				
				if (ShowDebug)
				MYLOG(RadiusString)

				// get target hit location
				FVector targetPos = targetHitTransform.GetLocation();
				
				// add tie vectors to targetpos to get points where is possible go to avoid obstacle
				TArray<FVector> points;
				
				FVector pointUp = targetPos + ((temp_Radius + SecurtyOffset) * GetActorUpVector());
				FVector pointDown = targetPos + ((temp_Radius + SecurtyOffset)  * -GetActorUpVector());
				FVector pointRight = targetPos + ((temp_Radius + SecurtyOffset)  * GetActorRightVector());
				FVector pointLeft = targetPos + ((temp_Radius + SecurtyOffset)  * -GetActorRightVector());
				points.Add(pointUp);
				points.Add(pointDown);
				points.Add(pointRight);
				points.Add(pointLeft);


				float minDistance = 999999999999999.0f;

				for (FVector point : points) 
				{
					if (ShowDebug)
					DrawDebugLine(GetWorld(),targetPos,point,FColor(100, 0, 0),true, 10, 0,300);

					float distance = (TargetPos - point).Size();

					if (distance < minDistance) 
					{
						minDistance = distance;
						TargetPos = point;
					}

				}

				

				IsObstacle = true;
				
			}
		}
	}
}

// Called when the game starts or when spawned
void AATIE::BeginPlay()
{
	Super::BeginPlay();
	
	// Get player actor
	Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); 
	

	// Call check obstacle in loop 
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AATIE::CheckObstacles, 1, true, 0);

}

// Called every frame
void AATIE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!IsObstacle)
	TargetPos = Target->GetActorLocation();
	
	FVector Targetdirection = (TargetPos - GetActorLocation()).GetSafeNormal();
	FRotator TargetRot = FRotationMatrix::MakeFromX(Targetdirection).Rotator();
	float TargetDistance = FVector::Distance(GetActorLocation(), TargetPos);
	
	// Rotate TIE to look the target
	this->SetActorRelativeRotation( FMath::RInterpTo(this->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 500));
	
	
	// Increase speed and clamp at velocidadMax
	currentSpeed = FMath::Min(currentSpeed + acelerationFactor*GetWorld()->GetDeltaSeconds(), VelocidadMax);
	//MYLOG(FString::SanitizeFloat(currentSpeed))

	// Decrease speed and clamp at 0
	currentSpeed = FMath::Max(currentSpeed - decelerationAmount*GetWorld()->GetDeltaSeconds(), 0.0f);
	//MYLOG(FString::SanitizeFloat(currentSpeed))
	
	
	// Check if arrive attack distance 
	ArriveAttackDistance(TargetDistance, DistanciaAtaque);


	// Movement of tie fighter based in if a obstacle is detected or not
	if (!IsObstacle) 
	{
		if (TargetDistance > TargetStopDistance) 
		{
			decelerationAmount = 0;
		}
		else
		{
			decelerationAmount = decelerationFactor;
		}	
	}
	else 
	{
		if (TargetDistance > DodgeStopDistance) 
		{
			decelerationAmount = 0;
		}
		else
		{
			IsObstacle = false;
		}
	}

	this->AddActorLocalOffset(FVector(currentSpeed * GetWorld()->GetDeltaSeconds(), 0, 0), true);

	
}

