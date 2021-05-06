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

protected:
	void SimulateScannig(float DeltaTime) override;
	bool IsScanCompleted() override;
	void CreateLasers();

private:

	//Lidar settings
	const uint32 ChannelCount = 32u;
	uint32 PointsPerSecond = 56000u;
	float RotationFrequency = 10.0f;
	float UpperFovLimit = 10.0f;
	float LowerFovLimit = -30.0f;
	float HorizontalFov = 360.0f;


	//Scan Settings
	TArray<float> LaserAngles;
	std::vector<uint32_t> PointsPerChannel;
	float CurrentLidarHorizontalAngle = 0.0;
	
};
