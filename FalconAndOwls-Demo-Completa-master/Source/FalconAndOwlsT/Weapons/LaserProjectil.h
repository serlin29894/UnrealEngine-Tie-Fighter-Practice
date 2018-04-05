// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserProjectil.generated.h"

UCLASS()
class FALCONANDOWLST_API ALaserProjectil : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserProjectil();

	UPROPERTY(EditDefaultsonly, BlueprintReadOnly)
	class UStaticMeshComponent *Projectil;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	float Speed;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn))
	float Scope;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn))
	float Power;

	UPROPERTY(EditDefaultsonly, BlueprintReadWrite)
	FVector DessignScale;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn))
	float Scale;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn))
	AActor * NoCollisionActor;

	UPROPERTY(EditDefaultsonly)
	class UParticleSystem *ParticleSystemTemplate;

	UFUNCTION()
	void ProjectilHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//void ProjectilHit(AActor* OverlappedActor, AActor* OtherActor);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
