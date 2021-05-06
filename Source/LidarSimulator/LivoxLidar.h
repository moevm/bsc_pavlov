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

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetFieldOfView(float FieldOfView_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetFieldOfView();

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetPointsPerSecond(int32 PointsPerSecond_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		int32 GetPointsPerSecond();

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetPulseFreq(float PulseFreq_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetPulseFreq();

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetFrequencyPrism_1(float FrequencyPrism_1_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetFrequencyPrism_1();

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetFrequencyPrism_2(float FrequencyPrism_2_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetFrequencyPrism_2();

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void ApplyLivoxSettings(float Range_,
							float NoiseStdDev_,
							float FieldOfView_, 
							float FrequencyPrism_1_,
							float FrequencyPrism_2_,
							float PulseFreq_,
							int32 PointsPerSecond_);
	void ResetStateVariablesToInitial() override;
protected:
	void CalculateX();
	void CalculateY();
	void SetVerticalAngle();
	void SetHorizontalAngle();

	void SimulateScannig(float DeltaTime) override;
private:
	//Lidar settings
	float FieldOfView = 38.4f;
	int32 PointsPerSecond = 56000;
	float PulseFreq = 100000.0f;
	float FrequencyPrism_1 = 7294.0;
	float FrequencyPrism_2 = -4664.0;


	//State variables
	float CurrentVerticalAngle = 0.0f;
	float CurrentHorizontalAngle = 0.0f;
	float CurrentX = 0.0f;
	float CurrentY = 0.0f;
	float Time = 0.0f;
	float DeltaT = 0.0f;
};
