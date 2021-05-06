// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "Misc/FileHelper.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "LidarDetection.h"

/**
 * 
 */
class LIDARSIMULATOR_API FileWriter
{
public:
	FileWriter();
	~FileWriter();
	void WriteBufferToFile(FString OutputFileName, std::vector<LidarDetection>& Buffer);
private:
};
