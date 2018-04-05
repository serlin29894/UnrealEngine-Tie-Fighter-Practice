// Fill out your copyright notice in the Description page of Project Settings.

#include "LaserProjectil.h"
#include "Components/StaticMeshComponent.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MyDefs.h"
#include "LaserMirror.h"
#include "AsteroidsField/AsteroidsField.h"

// Sets default values
ALaserProjectil::ALaserProjectil()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorEnableCollision(true);

	Projectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserProjectil"));
	RootComponent = Projectil;
	Projectil->bGenerateOverlapEvents = true;
	//OnActorBeginOverlap.AddDynamic(this, &ALaserProjectil::ProjectilHit);
	Projectil->OnComponentBeginOverlap.AddDynamic(this, &ALaserProjectil::ProjectilHit);// bindea LA FUNCION projectil hit a on component begin overlap para que se llame cuando se ejecute on component begin overlap
}


void ALaserProjectil::ProjectilHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//void ALaserProjectil::ProjectilHit(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor == NoCollisionActor)
		return;

	//MYLOG("DESTROY")
	AAsteroidsField *asteroids = Cast<AAsteroidsField>(OtherActor); // casteo a estoride 
	if (asteroids)
	{
		asteroids->ItemTakeDamage(Power, OtherBodyIndex);
	}
	else
	{
		FDamageEvent de;

		OtherActor->TakeDamage(Power, de, UGameplayStatics::GetPlayerController(GetWorld(), 0), this);
	}

	if (ParticleSystemTemplate != nullptr)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystemTemplate,FTransform(GetActorRotation(), GetActorLocation(), Scale*FVector(1.f, 1.f, 1.f)));

	if (!OtherActor->IsA(ALaserMirror::StaticClass()))
		Destroy();
}

// Called when the game starts or when spawned
void ALaserProjectil::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(DessignScale*Scale);
	SetLifeSpan(Scope / Speed + 10);
}

// Called every frame
void ALaserProjectil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalOffset(FVector(Speed*DeltaTime, 0.f, 0.f), true);
}

