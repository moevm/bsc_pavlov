// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LIDARSIMULATOR_API LidarDetection
{
public:
	float TimeStamp;
	FVector Point;
	float Intensity;

	LidarDetection() :
		TimeStamp{ 0.0f }, Point(0.0f, 0.0f, 0.0f), Intensity{ 0.0f } { }
	LidarDetection(float timeStamp, float x, float y, float z, float intensity) :
		TimeStamp{ timeStamp }, Point(x, y, z), Intensity{ intensity } { }

	~LidarDetection();
};
