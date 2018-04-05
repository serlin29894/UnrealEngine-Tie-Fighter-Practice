// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FalconPlayer.generated.h"

UCLASS()
class FALCONANDOWLST_API AFalconPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFalconPlayer();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent *FalconMesh;

	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent *CameraSpring;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCameraComponent *Camera;

	void Pitch(float Value);
	void Yaw(float Value);
	void Power(float Value);

	UPROPERTY(BlueprintReadOnly)
	bool bIsCamLocked = false;
	UFUNCTION(BlueprintImplementableEvent, Category = "Player Control")
	void OnCamLock(bool CamLocked);

	FRotator SpringRelativeRotation;
	void LockCam();
	void UnlockCam();
	void LookAround(float Value);
	void LookUp(float Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Speed & Power")
	float CurrentSpeed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed & Power")
	float CurrentPower = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed & Power")
	float MaxSpeed = 2000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed & Power")
	float MaxPower =500.f;
	UPROPERTY(EditDefaultsOnly, Category = "Speed & Power")
	float PowerIncrementFactor = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Speed & Power")
	float DampingSpeed = 2.f;

	UPROPERTY(EditDefaultsOnly, Category ="Steering")
	float MaxPitch = 90.f;
	float CurrentPitch = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Steering")
	float MaxYaw = 90.f;
	float CurrentYaw = 0.f;

	float CurrentRoll = 0.f;

	UFUNCTION(BlueprintImplementableEvent)
	void PowerEvent(float PowerFactor);
	float PowerFactorToMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitProperties() override;
	
	
};
