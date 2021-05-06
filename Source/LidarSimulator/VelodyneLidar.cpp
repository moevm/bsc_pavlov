// Fill out your copyright notice in the Description page of Project Settings.


#include "VelodyneLidar.h"

#include <cmath>
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "DrawDebugHelpers.h"


AVelodyneLidar::AVelodyneLidar() 
	: ABaseLidar()
{
	//FString FileName = GetNameLidar()
	PrimaryActorTick.bCanEverTick = true;
	CreateLasers();
	PointsPerChannel.resize(ChannelCount);
}

void AVelodyneLidar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AVelodyneLidar::IsScanCompleted()
{
	return false;
}

void AVelodyneLidar::CreateLasers()
{
	const auto NumberOfLasers = ChannelCount;
	const float DeltaAngle = NumberOfLasers == 1u ? 0.f :
		(UpperFovLimit - LowerFovLimit) /
		static_cast<float>(NumberOfLasers - 1);
	LaserAngles.Empty(NumberOfLasers);
	for (auto i = 0u; i < NumberOfLasers; ++i)
	{
		const float VerticalAngle = UpperFovLimit - static_cast<float>(i) * DeltaAngle;
		LaserAngles.Emplace(VerticalAngle);
	}
}

void AVelodyneLidar::SimulateScannig(float DeltaTime)
{
	PointCloud.clear();
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
				PointCloud.emplace_back(HitResult.ImpactPoint);
			}
		};
	}
	PointCloudWriter->WriteBufferToFile(PointCloudFileName, PointCloud);
	const float HorizontalAngle = std::fmod(CurrentHorizontalAngle + AngleDistanceOfTick, HorizontalFov);
	this->CurrentLidarHorizontalAngle = HorizontalAngle;
}




