// Fill out your copyright notice in the Description page of Project Settings.


#include "VelodyneLidar.h"

#include <cmath>
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "DrawDebugHelpers.h"


AVelodyneLidar::AVelodyneLidar() 
	: ABaseLidar()
{
	PrimaryActorTick.bCanEverTick = true;
	CreateLasers();
	PointsPerChannel.resize(ChannelCount);
}

void AVelodyneLidar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVelodyneLidar::CreateLasers()
{
	const auto NumberOfLasers = ChannelCount;
	const float DeltaAngle = NumberOfLasers == 1 ? 0.f :
		(UpperFovLimit - LowerFovLimit) /
		static_cast<float>(NumberOfLasers - 1);
	LaserAngles.Empty(NumberOfLasers);
	for (auto i = 0; i < NumberOfLasers; ++i)
	{
		const float VerticalAngle = UpperFovLimit - static_cast<float>(i) * DeltaAngle;
		LaserAngles.Emplace(VerticalAngle);
	}
}

void AVelodyneLidar::SimulateScannig(float DeltaTime)
{
	Super::SimulateScannig(DeltaTime);
	const uint32 PointsToScanWithOneLaser = FMath::RoundHalfFromZero(
		PointsPerSecond * DeltaTime / float(ChannelCount));
	if (PointsToScanWithOneLaser <= 0)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s: no points requested this frame, try increasing the number of points per second."),
			*GetName());
		return;
	}

	const float CurrentHorizontalAngle = this->CurrentLidarHorizontalAngle;

	const float AngleDistanceOfTick = RotationFrequency * HorizontalFov * DeltaTime;

	const float AngleDistanceOfLaserMeasure = AngleDistanceOfTick / PointsToScanWithOneLaser;
	// Parallel For (DrawDebugPoints doesn't work)
	/*ParallelFor(ChannelCount, [&](int32 idxChannel) {
		for (auto idxPtsOneLaser = 0u; idxPtsOneLaser < PointsToScanWithOneLaser; idxPtsOneLaser++) {
			FHitResult HitResult;
			const float VertAngle = LaserAngles[idxChannel];
			const float HorizAngle = std::fmod(CurrentHorizontalAngle + AngleDistanceOfLaserMeasure
				* idxPtsOneLaser, HorizontalFov) - HorizontalFov / 2;

			if (ShootLaser(VertAngle, HorizAngle, HitResult)) {
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString::Printf(TEXT("HIT")));
				continue;
			}
		};
	});*/

	//.............
	for (size_t idxChannel = 0; idxChannel < ChannelCount; idxChannel++)
	{
		for (auto idxPtsOneLaser = 0u; idxPtsOneLaser < PointsToScanWithOneLaser; idxPtsOneLaser++)
		{
			FHitResult HitResult;
			const float VertAngle = LaserAngles[idxChannel];
			const float HorizAngle = std::fmod(CurrentHorizontalAngle + AngleDistanceOfLaserMeasure
				* idxPtsOneLaser, HorizontalFov) - HorizontalFov / 2;

			if (ShootLaser(VertAngle, HorizAngle, HitResult))
			{
				FTransform LidarTransform = GetActorTransform();
				LidarDetection Detection = ComputeDetection(LidarTransform, HitResult);
			//	AddGaussianNoise(Detection);
			//	ComputeIntensity(Detection);
				LidarDetectionPointCloud.emplace_back(Detection);
			}
		};
	}
	const float HorizontalAngle = std::fmod(CurrentHorizontalAngle + AngleDistanceOfTick, HorizontalFov);
	this->CurrentLidarHorizontalAngle = HorizontalAngle;
}

void AVelodyneLidar::SetChannelCount(int32 ChannelCount_)
{
	this->ChannelCount = ChannelCount_;
}

int32 AVelodyneLidar::GetChannelCount()
{
	return ChannelCount;
}

void AVelodyneLidar::SetPointsPerSecond(int32 PointsPerSecond_)
{
	this->PointsPerSecond = PointsPerSecond_;
}

int32 AVelodyneLidar::GetPointsPerSecond()
{
	return PointsPerSecond;
}

void AVelodyneLidar::SetRotationFrequency(float RotationFrequency_)
{
	this->RotationFrequency = RotationFrequency_;
}

float AVelodyneLidar::GetRotationFrequency()
{
	return RotationFrequency;
}

void AVelodyneLidar::SetUpperFovLimit(float UpperFovLimit_)
{
	this->UpperFovLimit = UpperFovLimit_;
}

float AVelodyneLidar::GetUpperFovLimit()
{
	return UpperFovLimit;
}

void AVelodyneLidar::SetLowerFovLimit(float LowerFovLimit_)
{
	this->LowerFovLimit = LowerFovLimit_;
}

float AVelodyneLidar::GetLowerFovLimit()
{
	return LowerFovLimit;
}

void AVelodyneLidar::SetHorizontalFov(float HorizontalFov_)
{
	this->HorizontalFov = HorizontalFov_;
}

float AVelodyneLidar::GetHorizontalFov()
{
	return HorizontalFov;
}

void AVelodyneLidar::ApplyVelodyneSettings(float Range_,
											float NoiseStdDev_,
											int32 ChannelCount_,
											int32 PointsPerSecond_,
											float RotationFrequency_,
											float UpperFovLimit_,
											float LowerFovLimit_,
											float HorizontalFov_)
{
	Super::ApplySettings(Range_, NoiseStdDev_);
	this->ChannelCount = ChannelCount_;
	this->PointsPerSecond = PointsPerSecond_;
	this->RotationFrequency = RotationFrequency_;
	this->UpperFovLimit = UpperFovLimit_;
	this->LowerFovLimit = LowerFovLimit_;
	this->HorizontalFov = HorizontalFov_;

	CreateLasers();
	PointsPerChannel.resize(ChannelCount);
}

void AVelodyneLidar::ResetStateVariablesToInitial()
{
	Super::ResetStateVariablesToInitial();
	this->CurrentLidarHorizontalAngle = 0.0;
}




