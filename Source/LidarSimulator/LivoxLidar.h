// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "BaseLidar.h"
#include "LivoxLidar.generated.h"

/**
 * 
 */
UCLASS()
class LIDARSIMULATOR_API ALivoxLidar : public ABaseLidar
{
	GENERATED_BODY()

public:
	ALivoxLidar();
	virtual void Tick(float DeltaTime) override;
protected:
	float ScanningFunction();
	void SetVerticalAngle();
	void SetHorizontalAngle();
	void CalculateAngleBetweenPulses();
	void CalculateCurrentThetaAngle();
	void CalculateAngleBetweenRoses();

	void SimulateScannig(float DeltaTime) override;
	bool IsScanCompleted() override;
	void DoSimStep();

private:
	//Lidar settings
	float FieldOfView = 38.4f;
	uint32 NumberPetals = 5u;
	float Theta = PI;
	float W = 2 * PI;
	uint32 PointsInOneRose = 1000u;
	uint32 NumberRoses = 15u;
	uint32 CountPoints = 0u;


	//State variables
	float AngleBetweenPulses = 0.0f;
	float AngleBetweenRoses = 0.0f;
	float CurrentThetaAngle = 0.0f;
	float CurrentR = 0.0f;
	float CurrentW = 0.0f;
	float CurrentVerticalAngle = 0.0f;
	float CurrentHorizontalAngle = 0.0f;
	uint32 CurrentNumberRoses = 0;

	uint32 PointsPerSecond = 1000u;
	//point cloud
};
