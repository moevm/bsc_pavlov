// Fill out your copyright notice in the Description page of Project Settings.


#include "ParallelLidar.h"

#include <cmath>
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

AParallelLidar::AParallelLidar()
	: ABaseLidar()
{
	PrimaryActorTick.bCanEverTick = true;
	CalculateVerticalResolution();
	CalculateHorizontalResolution();
}

void AParallelLidar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AParallelLidar::CalculateVerticalResolution()
{
	this->VerticalResolution = (2 * this->ScanAngle * this->ScanFreq_Hz) / this->PulseFreq_Hz;
}

void AParallelLidar::CalculateHorizontalResolution()
{
	this->HorizontalResolution = this->RotatePerSec / this->ScanFreq_Hz;
}

void AParallelLidar::DoSimHorizontalStep()
{
	if (bIsRunHorizontal)
	{
		CurrentHorizontalAngle += HorizontalResolution;
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Horizontal_Step_Angle: %f"),
			CurrentHorizontalAngle);
		bIsRunHorizontal = false;
	}
}

void AParallelLidar::DoSimVerticalStep()
{
	CurrentVerticalAngle += VerticalResolution;

	if (CurrentVerticalAngle >= ScanAngle)
	{
		CurrentVerticalAngle = -ScanAngle;
		bIsRunHorizontal = true;
	}
}

void AParallelLidar::SimulateScannig(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, "Scanning");

	const uint32 PointsInOneFrame = FMath::RoundHalfFromZero(PointsPerSecond * DeltaTime);
	for (size_t i = 0; i < PointsInOneFrame; i++)
	{
		DoSimStep();
	}
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, "End Scanning");
}

void AParallelLidar::DoSimStep()
{
	DoSimHorizontalStep();
	DoSimVerticalStep();
	FHitResult HitResult;
	if (ShootLaser(CurrentVerticalAngle, CurrentHorizontalAngle, HitResult))
	{
		PointCloud.emplace_back(HitResult.ImpactPoint);
	}
}

bool AParallelLidar::IsScanCompleted()
{
	return CurrentHorizontalAngle >= HorizontalStopAngle;
}

