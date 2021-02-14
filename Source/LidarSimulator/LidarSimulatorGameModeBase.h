// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LidarSimulatorGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LIDARSIMULATOR_API ALidarSimulatorGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Pause")
	bool isPaused = true;

	bool getIsPaused()
	{
		return isPaused;
	}
};
