// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FileWriter.h"
#include "GameFramework/GameMode.h"
#include "SimGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LIDARSIMULATOR_API ASimGameMode : public AGameMode
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
private:
	FileWriter * fw;
};
