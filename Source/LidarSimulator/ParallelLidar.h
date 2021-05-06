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

protected:
	void CalculateVerticalResolution();
	void CalculateHorizontalResolution();
	void DoSimHorizontalStep();
	void DoSimVerticalStep();
	void SimulateScannig(float DeltaTime) override;
	bool IsScanCompleted() override;
	void DoSimStep();

private:
	//Lidar settings
	float RotatePerSec = 2.0f;
	float HorizontalStartAngle = 0.0f;  //gorizontal FOV
	float HorizontalStopAngle = 90.0f;  //gorizontal FOV

	uint32 PulseFreq_Hz = 100000u;
	float ScanAngle = 40.0f; //vertical FOV
	uint32 ScanFreq_Hz = 50u;

	//State variables
	float CurrentHorizontalAngle = 0.0f;
	float CurrentVerticalAngle = 0.0;
	bool bIsRunHorizontal = true;
	bool bIsRunVertical = true;

	//State variables for scanning
	float VerticalResolution = 0.0f;
	float HorizontalResolution = 0.0f;
	uint32 PointsPerSecond = 300000u;
};
