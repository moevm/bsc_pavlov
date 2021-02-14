// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirstLidar.generated.h"


UCLASS()
class LIDARSIMULATOR_API AFirstLidar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFirstLidar();
	virtual void PrePhysTick(float DeltaSeconds) {}
	void PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateLasers();

	void SimulateLidar(float DeltaTime);

	bool ShootLaser(const float VerticalAngle, float HorizontalAngle, FHitResult& HitResult) const;

	void SetCurrentLidarHorizontalAngle(float HorizontalAngle);

	float GetCurrentLidarHorizontalAngle();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//manage pause
	UFUNCTION(BlueprintCallable, Category = "Pause")
	void StartScanning(bool state);

	UFUNCTION(BlueprintCallable, Category = "Pause")
	bool getIsPaused();

private:

	// manage settings
	UPROPERTY(EditAnywhere, Category = "Pause")
	bool isPaused = true;

	//Basic Lidar Settings

	const uint32 ChannelCount = 32u;

	float Range = 1000.0f;

	uint32 PointsPerSecond = 56000u;

	float RotationFrequency = 10.0f;

	float UpperFovLimit = 10.0f;

	float LowerFovLimit = -30.0f;

	float HorizontalFov = 360.0f;


	//Scan Settings
	TArray<float> LaserAngles;
	std::vector<uint32_t> PointsPerChannel;

	float CurrentLidarHorizontalAngle = 0.0;


	FDelegateHandle OnPostTickDelegate;

};
