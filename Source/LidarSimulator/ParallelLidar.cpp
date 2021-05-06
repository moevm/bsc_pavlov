// Fill out your copyright notice in the Description page of Project Settings.


#include "ParallelLidar.h"

#include <cmath>
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

AParallelLidar::AParallelLidar()
	: ABaseLidar()
{
	PrimaryActorTick.bCanEverTick = true;

	RotatePerSec_rad = FMath::DegreesToRadians(RotatePerSec);
	HorizontalStartAngle_rad = FMath::DegreesToRadians(HorizontalStartAngle);
	HorizontalStopAngle_rad = FMath::DegreesToRadians(HorizontalStopAngle);
	ScanAngle_rad = FMath::DegreesToRadians(ScanAngle);

	CurrentHorizontalAngle_rad = HorizontalStartAngle_rad;
	CalculateVerticalResolution();
	CalculateHorizontalResolution();
}

void AParallelLidar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AParallelLidar::CalculateVerticalResolution()
{
	this->VerticalResolution_rad = (2 * this->ScanAngle_rad * this->ScanFreq_Hz) / this->PulseFreq_Hz;
}

void AParallelLidar::CalculateHorizontalResolution()
{
	this->HorizontalResolution_rad = this->RotatePerSec_rad / this->ScanFreq_Hz;
}

void AParallelLidar::DoSimHorizontalStep()
{
	if (bIsRunHorizontal)
	{
		CurrentHorizontalAngle_rad += HorizontalResolution_rad;
		bIsRunHorizontal = false;
	}
}

void AParallelLidar::DoSimVerticalStep()
{
	CurrentVerticalAngle_rad += VerticalResolution_rad;

	if (CurrentVerticalAngle_rad >= ScanAngle_rad)
	{
		CurrentVerticalAngle_rad = -ScanAngle_rad;
		bIsRunHorizontal = true;
	}
}

void AParallelLidar::SimulateScannig(float DeltaTime)
{
	Super::SimulateScannig(DeltaTime);
	const uint32 PointsInOneFrame = FMath::RoundHalfFromZero(PointsPerSecond * DeltaTime);
	for (size_t i = 0; i < PointsInOneFrame; i++)
	{
		if (IsScanCompleted())
		{
			SetIsActive(false);
			return;
		}
		DoSimHorizontalStep();
		DoSimVerticalStep();
		FHitResult HitResult;
		if (ShootLaser(FMath::RadiansToDegrees(CurrentVerticalAngle_rad), FMath::RadiansToDegrees(CurrentHorizontalAngle_rad), HitResult))
		{
			FTransform LidarTransform = GetActorTransform();
			LidarDetection Detection = ComputeDetection(LidarTransform, HitResult);
			LidarDetectionPointCloud.emplace_back(Detection);
		}
	}
}

bool AParallelLidar::IsScanCompleted()
{
	return CurrentHorizontalAngle_rad >= HorizontalStopAngle_rad;
}

void AParallelLidar::SetRotatePerSec(float RotatePerSec_)
{
	this->RotatePerSec = RotatePerSec_;
}

float AParallelLidar::GetRotatePerSec()
{
	return RotatePerSec;
}

void AParallelLidar::SetHorizontalStartAngle(float HorizontalStartAngle_)
{
	this->HorizontalStartAngle = HorizontalStartAngle_;
}

float AParallelLidar::GetHorizontalStartAngle()
{
	return HorizontalStartAngle;
}

void AParallelLidar::SetHorizontalStopAngle(float HorizontalStopAngle_)
{
	this->HorizontalStopAngle = HorizontalStopAngle_;
}

float AParallelLidar::GetHorizontalStopAngle()
{
	return HorizontalStopAngle;
}

void AParallelLidar::SetPulseFreq_Hz(int32 PulseFreq_Hz_)
{
	this->PulseFreq_Hz = PulseFreq_Hz_;
}

int32 AParallelLidar::GetPulseFreq_Hz()
{
	return PulseFreq_Hz;
}

void AParallelLidar::SetScanAngle(float ScanAngle_)
{
	this->ScanAngle = ScanAngle_;
}

float AParallelLidar::GetScanAngle()
{
	return ScanAngle;
}

void AParallelLidar::SetScanFreq_Hz(int32 ScanFreq_Hz_)
{
	this->ScanFreq_Hz = ScanFreq_Hz_;
}

int32 AParallelLidar::GetScanFreq_Hz()
{
	return ScanFreq_Hz;
}

void AParallelLidar::SetPointsPerSecond(int32 PointsPerSecond_)
{
	this->PointsPerSecond = PointsPerSecond_;
}

int32 AParallelLidar::GetPointsPerSecond()
{
	return PointsPerSecond;
}

void AParallelLidar::ApplyParallelSettings(float Range_,
							float NoiseStdDev_,
							float RotatePerSec_,
							float HorizontalStartAngle_,
							float HorizontalStopAngle_,
							int32 PulseFreq_Hz_,
							float ScanAngle_,
							int32 ScanFreq_Hz_,
							int32 PointsPerSecond_)
{
	Super::ApplySettings(Range_, NoiseStdDev_);
	this->RotatePerSec = RotatePerSec_;
	this->HorizontalStartAngle = HorizontalStartAngle_;
	this->HorizontalStopAngle = HorizontalStopAngle_;
	this->PulseFreq_Hz = PulseFreq_Hz_;
	this->ScanAngle = ScanAngle_;
	this->ScanFreq_Hz = ScanFreq_Hz_;
	this->PointsPerSecond = PointsPerSecond_;
	RotatePerSec_rad = FMath::DegreesToRadians(RotatePerSec);
	HorizontalStartAngle_rad = FMath::DegreesToRadians(HorizontalStartAngle);
	HorizontalStopAngle_rad = FMath::DegreesToRadians(HorizontalStopAngle);
	ScanAngle_rad = FMath::DegreesToRadians(ScanAngle);
	
	this->CurrentHorizontalAngle_rad = HorizontalStartAngle_rad;
	CalculateVerticalResolution();
	CalculateHorizontalResolution();
}

void AParallelLidar::ResetStateVariablesToInitial()
{
	Super::ResetStateVariablesToInitial();
	this->CurrentHorizontalAngle_rad = HorizontalStartAngle_rad;
	this->CurrentVerticalAngle_rad = 0.0;
	this->bIsRunHorizontal = true;
	this->bIsRunVertical = true;
}