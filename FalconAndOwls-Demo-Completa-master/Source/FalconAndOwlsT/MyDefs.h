// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#pragma once

#if !UE_BUILD_SHIPPING
#define MYLOG(msg) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, msg);
#else
#define MYLOG(msg)
#endif
