// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "Misc/FileHelper.h"
#include "GenericPlatform/GenericPlatformMisc.h"

/**
 * 
 */
class LIDARSIMULATOR_API FileWriter
{
public:
	FileWriter();
	~FileWriter();
	void TestMethod();
	void CreateOutputDirectory();
	void CreateOutputFile(FString OutputFileName);
	void WriteBufferToFile(FString OutputFileName, std::vector<FVector>& Buffer);
private:
};
