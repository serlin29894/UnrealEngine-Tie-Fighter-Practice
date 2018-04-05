// Fill out your copyright notice in the Description page of Project Settings.

#include "FalconPlayer.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Classes/GameFramework/SpringArmComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "Classes/Components/InputComponent.h"
#include "Classes/Engine/Engine.h"
#include "MyDefs.h"

// Sets default values
AFalconPlayer::AFalconPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FalconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FalconMesh"));
	RootComponent = FalconMesh;

	CameraSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpring"));
	CameraSpring->TargetArmLength = 1000.f;
	CameraSpring->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThstPersonCamera"));
	Camera->SetupAttachment(CameraSpring, USpringArmComponent::SocketName);

}

void AFalconPlayer::Pitch(float Value)
{
	//MYLOG(   FString("Pitch..").Append(FString::FormatAsNumber(Value))    )

	CurrentPitch = FMath::FInterpTo(CurrentPitch, MaxPitch*Value, GetWorld()->GetDeltaSeconds(), 2.5f);
}

void AFalconPlayer::Yaw(float Value)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, MaxYaw*Value, GetWorld()->GetDeltaSeconds(), 2.5f);

	if (FMath::Abs(Value) < 0.2f)
	{
		CurrentRoll = FalconMesh->GetComponentRotation().Roll*(-1.f);
	}
	else
	{
		CurrentRoll = CurrentYaw;
	}
}

void AFalconPlayer::Power(float Value)
{
	if (Value > 0.2f)
	{
		CurrentPower += Value*PowerIncrementFactor;
		CurrentPower = FMath::Min(CurrentPower, MaxPower);

		PowerFactorToMaterial = CurrentPower / MaxPower;

	}
	else
	{
		CurrentPower = 0.f;
		CurrentSpeed = FMath::Max(CurrentSpeed - DampingSpeed*GetWorld()->GetDeltaSeconds(), 0.f);

		PowerFactorToMaterial = FMath::FInterpTo(PowerFactorToMaterial, 0.f, GetWorld()->GetDeltaSeconds(), 10.f);
	}

	PowerEvent(PowerFactorToMaterial);
}

void AFalconPlayer::LockCam()
{
	bIsCamLocked = true;

	OnCamLock(bIsCamLocked);
}

void AFalconPlayer::UnlockCam()
{
	bIsCamLocked = false;

	OnCamLock(bIsCamLocked);
}

void AFalconPlayer::LookAround(float Value)
{
	if (bIsCamLocked)
	{
		float p = CameraSpring->GetRelativeTransform().Rotator().Pitch;
		//CameraSpring->AddRelativeRotation(FRotator(-p*0.25f, Value, 0.f));
		CameraSpring->AddRelativeRotation(FRotator(0.f, Value, 0.f));
		//UE_LOG(LogTemp, Warning, TEXT("--"))
	}
	else
	{
		CameraSpring->SetRelativeRotation(FMath::RInterpTo(CameraSpring->GetRelativeTransform().Rotator(), SpringRelativeRotation, GetWorld()->GetDeltaSeconds(), 10.f));
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *CameraSpring->GetRelativeTransform().Rotator().ToString());
	}
}

void AFalconPlayer::LookUp(float Value)
{
	if (bIsCamLocked)
		Camera->AddRelativeRotation(FRotator(Value, 0.f, 0.f));
	else
		Camera->SetRelativeRotation(FMath::RInterpTo(Camera->GetRelativeTransform().Rotator(), FRotator(0.f), GetWorld()->GetDeltaSeconds(), 10.f));
}

// Called when the game starts or when spawned
void AFalconPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFalconPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FalconMesh->AddLocalRotation(FRotator(CurrentPitch*DeltaTime, CurrentYaw*DeltaTime, CurrentRoll*DeltaTime));

	CurrentSpeed = FMath::Min(CurrentSpeed + CurrentPower*DeltaTime, MaxSpeed);
	FalconMesh->AddLocalOffset(FVector(CurrentSpeed*DeltaTime, 0.f, 0.f), true);

}

// Called to bind functionality to input
void AFalconPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Pitch", this, &AFalconPlayer::Pitch);
	PlayerInputComponent->BindAxis("Yaw", this, &AFalconPlayer::Yaw);
	PlayerInputComponent->BindAxis("Power", this, &AFalconPlayer::Power);
	PlayerInputComponent->BindAction("LockCam", EInputEvent::IE_Pressed, this, &AFalconPlayer::LockCam);
	PlayerInputComponent->BindAction("LockCam", EInputEvent::IE_Released, this, &AFalconPlayer::UnlockCam);
	PlayerInputComponent->BindAxis("LookAround", this, &AFalconPlayer::LookAround);
	PlayerInputComponent->BindAxis("LookUp", this, &AFalconPlayer::LookUp);
}

void AFalconPlayer::PostInitProperties()
{
	Super::PostInitProperties();

	FTransform t = CameraSpring->GetRelativeTransform();
	SpringRelativeRotation = t.GetRotation().Rotator();

}

