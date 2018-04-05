// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FalconHUD.generated.h"

/**
 * 
 */
UCLASS()
class FALCONANDOWLST_API AFalconHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	

	UPROPERTY(EditDefaultsonly)
	TSubclassOf<class UUserWidget> UWTemplate;
	UPROPERTY(BlueprintReadonly)
	UUserWidget * UWInstance;

};
