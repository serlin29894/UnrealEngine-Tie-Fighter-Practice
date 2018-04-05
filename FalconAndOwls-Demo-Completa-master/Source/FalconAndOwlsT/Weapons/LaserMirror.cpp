// Fill out your copyright notice in the Description page of Project Settings.

#include "LaserMirror.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Classes/Components/SphereComponent.h"
#include "MyDefs.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"

// Sets default values
ALaserMirror::ALaserMirror()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;

	collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	collider->SetupAttachment(mesh);

}

void ALaserMirror::MirrorHit(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	  
	//comprueba que other actor se de la clase objectToMirrorType
	//MYLOG(OtherActor->GetClass()->GetName())
	if (OtherActor->IsA(objectToMirrorType)) 
	{
		//MYLOG(TEXT("dodo"))
		OnLaserHit();
		
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), OtherActor->GetActorLocation());
		//OtherActor->SetLifeSpan(20);
		OtherActor->SetActorRotation(rot);
			
	}

}

// Called when the game starts or when spawned
void ALaserMirror::BeginPlay()
{
	Super::BeginPlay();
	// bindea LA FUNCION projectil hit a on component begin overlap para que se llame cuando se ejecute on component begin overlap
	collider->OnComponentBeginOverlap.AddDynamic(this, &ALaserMirror::MirrorHit);

}

// Called every frame
void ALaserMirror::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

