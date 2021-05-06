// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLidar.h"
#include "ParallelLidar.generated.h"

/**
 * 
 */
UCLASS()
class LIDARSIMULATOR_API AParallelLidar : public ABaseLidar
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	AParallelLidar();

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetRotatePerSec(float RotatePerSec_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetRotatePerSec();
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetHorizontalStartAngle(float HorizontalStartAngle_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetHorizontalStartAngle();
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetHorizontalStopAngle(float HorizontalStopAngle_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetHorizontalStopAngle();
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetPulseFreq_Hz(int32 PulseFreq_Hz_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		int32 GetPulseFreq_Hz();
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetScanAngle(float ScanAngle_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetScanAngle();
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetScanFreq_Hz(int32 ScanFreq_Hz_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		int32 GetScanFreq_Hz();
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetPointsPerSecond(int32 PointsPerSecond_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		int32 GetPointsPerSecond();
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void ApplyParallelSettings(float Range_, 
									float NoiseStdDev_,
									float RotatePerSec_, 
									float HorizontalStartAngle_, 
									float HorizontalStopAngle_, 
									int32 PulseFreq_Hz_, 
									float ScanAngle_, 
									int32 ScanFreq_Hz_, 
									int32 PointsPerSecond_);
	void ResetStateVariablesToInitial() override;
protected:
	void CalculateVerticalResolution();
	void CalculateHorizontalResolution();
	void DoSimHorizontalStep();
	void DoSimVerticalStep();
	void SimulateScannig(float DeltaTime) override;
	bool IsScanCompleted() override;

private:
	//Lidar settings
	float RotatePerSec = 2.0f;
	float HorizontalStartAngle = 0.0f;  //gorizontal FOV
	float HorizontalStopAngle = 90.0f;  //gorizontal FOV
	float ScanAngle = 40.0f;			//vertical FOV
	int32 PulseFreq_Hz = 100000;
	int32 ScanFreq_Hz = 50;
	int32 PointsPerSecond = 300000;

	float RotatePerSec_rad = 0.0f;
	float HorizontalStartAngle_rad = 0.0f;
	float HorizontalStopAngle_rad = 0.0f;
	float ScanAngle_rad = 0.0f;

	//State variables
	float VerticalResolution_rad = 0.0f;
	float HorizontalResolution_rad = 0.0f;
	float CurrentHorizontalAngle_rad = 0.0f;
	float CurrentVerticalAngle_rad = 0.0f;
	bool bIsRunHorizontal = true;
	bool bIsRunVertical = true;
};
