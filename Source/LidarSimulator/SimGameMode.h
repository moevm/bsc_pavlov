// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLidar.h"
#include "MovementPlatform.h"
#include "RotatePlatform.h"
#include "BaseModel.h"
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
public:
	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void AddLidarActorToArray(ABaseLidar* LidarActor);

	UFUNCTION(BlueprintCallable, Category = "Pause")
		void StartSimulation();
	UFUNCTION(BlueprintCallable, Category = "Pause")
		void StopSimulation();

	UFUNCTION(BlueprintCallable, Category = "DeleteActors")
		void DeleteLidar(ABaseLidar* LidarActor);

	UFUNCTION(BlueprintCallable, Category = "FindLidarByName")
		ABaseLidar* FindLidarByName(FString LidarName_);

	UFUNCTION(BlueprintCallable, Category = "GetLidarArray")
		TArray<ABaseLidar*> GetLidarArray();

	UFUNCTION(BlueprintCallable, Category = "SimulationTime")
		void SetScanningDurationTime(float ScanningDurationTime_);
	UFUNCTION(BlueprintCallable, Category = "SimulationTime")
		float GetScanningDurationTime();

	UFUNCTION(BlueprintCallable, Category = "SimulationTime")
		void SetSimulationTime();

	UFUNCTION(BlueprintCallable, Category = "MovementPlatfrom")
		AMovementPlatform* GetMovementPlatfrom();
	UFUNCTION(BlueprintCallable, Category = "MovementPlatfrom")
		void SetMovementPlatform(AMovementPlatform* MovementPlatform_);

	UFUNCTION(BlueprintCallable, Category = "MovementPlatfrom")
		void DeleteMovementPlatform();

	UFUNCTION(BlueprintCallable, Category = "Simulation")
		void SetbIsDrawLines(bool State);
	UFUNCTION(BlueprintCallable, Category = "Simulation")
		bool GetbIsDrawLines();

	UFUNCTION(BlueprintCallable, Category = "Simulation")
		void SetbIsDrawPoints(bool State);
	UFUNCTION(BlueprintCallable, Category = "Simulation")
		bool GetbIsDrawPoints();

	UFUNCTION(BlueprintCallable, Category = "InWorldCoordinates")
		void SetbInWorldCoordinates(bool State);
	UFUNCTION(BlueprintCallable, Category = "InWorldCoordinates")
		bool GetbInWorldCoordinates();

	UFUNCTION(BlueprintCallable, Category = "AtmospAttenRate")
		void SetAtmospAttenRate(float AtmospAttenRate_);
	UFUNCTION(BlueprintCallable, Category = "AtmospAttenRate")
		float GetAtmospAttenRate();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void AddRotatePlatformToArray(ARotatePlatform* RotatePlatform);
	UFUNCTION(BlueprintCallable, Category = "DeleteActors")
		void DeleteRotatePlatform(ARotatePlatform* RotatePlatform);
	UFUNCTION(BlueprintCallable, Category = "FindRotatePlatformByName")
		ARotatePlatform* FindRotatePlatformByName(FString RotatePlatformName_);
	UFUNCTION(BlueprintCallable, Category = "GetRotatePlatformArray")
		TArray<ARotatePlatform*> GetRotatePlatformArray();

	UFUNCTION(BlueprintCallable, Category = "FindModelByName")
		ABaseModel* FindModelByName(FString ModelName_);
	UFUNCTION(BlueprintCallable, Category = "DeleteActors")
		void DeleteModel(ABaseModel* Model);
	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void AddModelToArray(ABaseModel* Model);
	UFUNCTION(BlueprintCallable, Category = "GetModelArray")
		TArray<ABaseModel*> GetModelArray();

	UFUNCTION(BlueprintCallable, Category = "Attach")
		void AttachLidarToMovementPlatform(ABaseLidar* LidarActor);
	UFUNCTION(BlueprintCallable, Category = "Attach")
		void AttachLidarToRotatePlatform(ABaseLidar* LidarActor, ARotatePlatform* RotatePlatform);
	UFUNCTION(BlueprintCallable, Category = "Attach")
		void AttachRotatePlatformToMovementPlatform(ARotatePlatform* RotatePlatform);
	
private:
	bool bIsDrawLines = false;
	bool bIsDrawPoints = false;
	bool bInWorldCoordinates = false;
	float AtmospAttenRate = 0.004f;

	FTimerHandle SimulationTimerHandle;
	float ScanningDurationTime = 10.0f;

	TArray<ABaseLidar*> LidarArray;
	AMovementPlatform* MovementPlatform;
	TArray<ARotatePlatform*> RotatePlatformArray;
	TArray<ABaseModel*> ModelArray;
};
