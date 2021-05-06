// Fill out your copyright notice in the Description page of Project Settings.


#include "LivoxLidar.h"

#include <cmath>
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

ALivoxLidar::ALivoxLidar()
	: ABaseLidar()
{
	DeltaT = 1.0 / PulseFreq;
}

void ALivoxLidar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALivoxLidar::CalculateX()
{	
	CurrentX = UKismetMathLibrary::Cos(FrequencyPrism_1 / (2 * PI) * Time) + UKismetMathLibrary::Cos(FrequencyPrism_2 / (2 * PI) * Time);
}

void ALivoxLidar::CalculateY()
{
	CurrentY = -UKismetMathLibrary::Sin(FrequencyPrism_1 / (2 * PI) * Time) - UKismetMathLibrary::Sin(FrequencyPrism_2 / (2 * PI) * Time);
}

void ALivoxLidar::SetVerticalAngle()
{
	CurrentVerticalAngle = UKismetMathLibrary::Atan(CurrentY * UKismetMathLibrary::Tan(FieldOfView / 2 * PI / 180) / 2) * 180 / PI;
}

void ALivoxLidar::SetHorizontalAngle()
{
	CurrentHorizontalAngle = UKismetMathLibrary::Atan(CurrentX * UKismetMathLibrary::Tan(FieldOfView / 2 * PI / 180) / 2) * 180 / PI;
}

void ALivoxLidar::SimulateScannig(float DeltaTime)
{
	Super::SimulateScannig(DeltaTime);
	const uint32 PointsInOneFrame = FMath::RoundHalfFromZero(PointsPerSecond * DeltaTime);
	for (size_t i = 0; i < PointsInOneFrame; i++)
	{
		Time += DeltaT;
		CalculateX();
		CalculateY();
		SetVerticalAngle();
		SetHorizontalAngle();
		FHitResult HitResult;
		if (ShootLaser(CurrentVerticalAngle, CurrentHorizontalAngle, HitResult))
		{
			FTransform LidarTransform = GetActorTransform();
			LidarDetection Detection = ComputeDetection(LidarTransform, HitResult);
			LidarDetectionPointCloud.emplace_back(Detection);
		}
	}
}

void ALivoxLidar::SetFieldOfView(float FieldOfView_)
{
	this->FieldOfView = FieldOfView_;
}

float ALivoxLidar::GetFieldOfView()
{
	return FieldOfView;
}

void ALivoxLidar::SetPointsPerSecond(int32 PointsPerSecond_)
{
	this->PointsPerSecond = PointsPerSecond_;
}

int32 ALivoxLidar::GetPointsPerSecond()
{
	return PointsPerSecond;
}

void ALivoxLidar::SetPulseFreq(float PulseFreq_)
{
	this->PulseFreq = PulseFreq_;
}

float ALivoxLidar::GetPulseFreq()
{
	return PulseFreq;
}


void ALivoxLidar::SetFrequencyPrism_1(float FrequencyPrism_1_)
{
	this->FrequencyPrism_1 = FrequencyPrism_1_;
}

float ALivoxLidar::GetFrequencyPrism_1()
{
	return FrequencyPrism_1;
}


void ALivoxLidar::SetFrequencyPrism_2(float FrequencyPrism_2_)
{
	this->FrequencyPrism_2 = FrequencyPrism_2_;
}

float ALivoxLidar::GetFrequencyPrism_2()
{
	return FrequencyPrism_2;
}

void ALivoxLidar::ApplyLivoxSettings(float Range_,
									float NoiseStdDev_,
									float FieldOfView_,
									float FrequencyPrism_1_,
									float FrequencyPrism_2_,
									float PulseFreq_,
									int32 PointsPerSecond_)
{
	Super::ApplySettings(Range_, NoiseStdDev_);
	this->FieldOfView = FieldOfView_;
	this->PointsPerSecond = PointsPerSecond_;
	this->FrequencyPrism_1 = FrequencyPrism_1_;
	this->FrequencyPrism_2 = FrequencyPrism_2_;
	this->PulseFreq = PulseFreq_;
	this->DeltaT = 1.0 / PulseFreq_;
}

void ALivoxLidar::ResetStateVariablesToInitial()
{
	Super::ResetStateVariablesToInitial();
	this->CurrentVerticalAngle = 0.0f;
	this->CurrentHorizontalAngle = 0.0f;
	this->CurrentX = 0.0f;
	this->CurrentY = 0.0f;
	this->Time = 0.0f;
}