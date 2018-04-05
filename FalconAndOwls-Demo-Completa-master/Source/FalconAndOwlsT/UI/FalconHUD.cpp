// Fill out your copyright notice in the Description page of Project Settings.

#include "FalconHUD.h"
#include "Blueprint/UserWidget.h"
#include "Classes/Kismet/GameplayStatics.h"
void AFalconHUD::BeginPlay()
{
	UWInstance = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), UWTemplate);

	if (UWInstance)
		UWInstance->AddToViewport();
}