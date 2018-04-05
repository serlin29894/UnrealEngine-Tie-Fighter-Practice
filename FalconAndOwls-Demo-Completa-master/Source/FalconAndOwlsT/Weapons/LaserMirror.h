// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserMirror.generated.h"

UCLASS()
class FALCONANDOWLST_API ALaserMirror : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserMirror();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent * mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent * collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> objectToMirrorType;
	
	UFUNCTION()
	void MirrorHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnLaserHit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
