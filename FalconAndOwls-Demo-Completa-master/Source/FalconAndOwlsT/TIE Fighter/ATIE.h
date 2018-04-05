// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ATIE.generated.h"

UCLASS()
class FALCONANDOWLST_API AATIE : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AATIE();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent * TIEMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug")
	bool ShowDebug = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Speed & Power")
	float VelocidadMax = 2000;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Speed & Power")
	float TargetStopDistance = 4000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Speed & Power")
	float DodgeStopDistance = 150;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Speed & Power")
	float decelerationFactor = 2000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Speed & Power")
	float acelerationFactor = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Speed & Power")
	float RotationSpeed = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	float DistanciaAtaque = 6000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Obstacles Avoidence")
	FVector CheckDirection = FVector(1, 0, 0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Obstacles Avoidence")
	float CheckOffset = 4500;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Obstacles Avoidence")
	float CheckBoxProfundity = 4500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Obstacles Avoidence")
	float CheckBoxHeight = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Obstacles Avoidence")
	float CheckBoxWitdth = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Obstacles Avoidence")
	float SecurtyOffset = 2000;

	UPROPERTY(BlueprintReadOnly, Category = "Speed & Power")
	float currentSpeed = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Obstacles Avoidence")
	bool IsObstacle = false;

	UFUNCTION(BlueprintImplementableEvent)
	void OnFalconHaunted (bool inRange); //Evento que se diripara al llegar a la distancia de ataque 

	UFUNCTION(BlueprintImplementableEvent)
	void OnTIEShoot();

	void ArriveAttackDistance(float distanceTarget, float limitDistance);

	void CheckObstacles();



	
private:
	APawn *Target;
	
	FTimerHandle TimerHandle;
	float decelerationAmount = 0;
	float CounterTime = 2;

	UPrimitiveComponent *temp_COMP = nullptr;
	AActor *temp_AC = nullptr;
	FVector TargetPos;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
