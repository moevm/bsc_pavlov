// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "BaseLidar.h"
#include "VelodyneLidar.generated.h"

/**
 * 
 */
UCLASS()
class LIDARSIMULATOR_API AVelodyneLidar : public ABaseLidar
{
	GENERATED_BODY()

public:
	AVelodyneLidar();
	void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetChannelCount(int32 ChannelCount_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		int32 GetChannelCount();
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetPointsPerSecond(int32 PointsPerSecond_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		int32 GetPointsPerSecond();
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetRotationFrequency(float RotationFrequency_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetRotationFrequency();
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetUpperFovLimit(float UpperFovLimit_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetUpperFovLimit();
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetLowerFovLimit(float LowerFovLimit_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetLowerFovLimit();
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetHorizontalFov(float HorizontalFov_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetHorizontalFov();
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void ApplyVelodyneSettings(float Range_, 
									float NoiseStdDev_,
									int32 ChannelCount_, 
									int32 PointsPerSecond_, 
									float RotationFrequency_, 
									float UpperFovLimit_, 
									float LowerFovLimit_,
									float HorizontalFov_);
	void ResetStateVariablesToInitial() override;
protected:
	void SimulateScannig(float DeltaTime) override;
	void CreateLasers();

private:
	//Lidar settings
	int32 ChannelCount = 32;
	int32 PointsPerSecond = 56000;
	float RotationFrequency = 10.0f;
	float UpperFovLimit = 10.0f;
	float LowerFovLimit = -30.0f;
	float HorizontalFov = 360.0f;


	//Scan Settings
	TArray<float> LaserAngles;
	std::vector<int32_t> PointsPerChannel;
	float CurrentLidarHorizontalAngle = 0.0;
};
