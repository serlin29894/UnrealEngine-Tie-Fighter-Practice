// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsField.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Classes/Kismet/KismetMathLibrary.h"
#include "MyDefs.h"
#include "Classes/Engine/StaticMesh.h"

// Sets default values
AAsteroidsField::AAsteroidsField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hism = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("AsteroidsHISM"));
	SetRootComponent(hism);
}

void AAsteroidsField::ItemTakeDamage(float DamageAmount, int32 index)
{
	FAsteroid * a = &AsteroidsTracker.GetData()[index];
	a->TakeDamage(DamageAmount);
	//MYLOG(FString::FormatAsNumber(a->TakeDamage(DamageAmount)))
	if (a->Health < 0.f)
	{
		FTransform t;
		hism->GetInstanceTransform(index, t, true);

		//Destruir asteroide
		hism->RemoveInstance(index); // quita la intancia del hism 
		if (index < AsteroidsTracker.Num() - 1) // asigna al hueco dejado por el asteroide destuido al ultimo asteroide
		{
			AsteroidsTracker.GetData()[index] = AsteroidsTracker.Last();
		}
		AsteroidsTracker.Pop(); // destruie el ultimo objeto de la array 

		hism->MarkRenderStateDirty(); // vuelve a pintar las mallas del hism 

		//Damos la posibilidad de añadir efectos a nivel de blueprint
		DestroyEffect(t.GetLocation());
	}

}

float AAsteroidsField::GetAsteroidRadius(int32 index) 
{
	FAsteroid * a = &AsteroidsTracker.GetData()[index];
	return a->Radius;
}





// Called when the game starts or when spawned
void AAsteroidsField::BeginPlay()
{
	Super::BeginPlay();
	
	FRandomStream srand; // semilla para usar random
	srand.GenerateNewSeed(); // genera una semilla nueva 

	for (int i = 0; i < NumberOfBigAsteroids; i++)
	{
		// srand.GetUnitVector() genera una vector al azar
		//srand.FRandRange(0.f, FieldRadius) devuelve un valor entre en un valor minimo y un valor maximo  
		FVector Scale = (srand.GetUnitVector() + FVector(1.f))*0.5f*ScaleMaxBigAsteroids;
		FVector Location = srand.GetUnitVector()*srand.FRandRange(0.f, FieldRadius);
		FRotator Rotation = UKismetMathLibrary::RandomRotator();
		int32 index = hism->AddInstance(FTransform(Rotation, Location, Scale)); // devuelve el indice de la instancia
		AsteroidsTracker.Add(FAsteroid(index, Scale.Size()*10.f, hism->GetStaticMesh()->ExtendedBounds.SphereRadius * Scale.Size() * 0.5f));
	}
	for (int i = 0; i < NumberOfLittleAsteroids; i++)
	{

		FVector Scale = (srand.GetUnitVector() + FVector(1.f))*0.5f*ScaleMaxLittleAsteroids;
		FVector Location = srand.GetUnitVector()*srand.FRandRange(0.f, FieldRadius);
		FRotator Rotation = UKismetMathLibrary::RandomRotator();
		int32 index = hism->AddInstance(FTransform(Rotation, Location, Scale));
		AsteroidsTracker.Add(FAsteroid(index, Scale.Size()*10.f, hism->GetStaticMesh()->ExtendedBounds.SphereRadius * Scale.Size() * 0.5f));
		//MYLOG(FString::FormatAsNumber(Scale.Size()*100.f))
	}
}

// Called every frame
void AAsteroidsField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

