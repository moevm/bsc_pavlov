// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstLidar.h"

#include <cmath>
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFirstLidar::AFirstLidar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CreateLasers();
	PointsPerChannel.resize(ChannelCount);
}

// Called when the game starts or when spawned
void AFirstLidar::BeginPlay()
{
	Super::BeginPlay();
	OnPostTickDelegate = FWorldDelegates::OnWorldPostActorTick.AddUObject(
		this, &AFirstLidar::PostPhysTick);
	
}

// Called every frame
void AFirstLidar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFirstLidar::CreateLasers()
{
	const auto NumberOfLasers = ChannelCount;
	const float DeltaAngle = NumberOfLasers == 1u ? 0.f :
		(UpperFovLimit - LowerFovLimit) /
		static_cast<float>(NumberOfLasers - 1);
	LaserAngles.Empty(NumberOfLasers);
	for (auto i = 0u; i < NumberOfLasers; ++i)
	{
		const float VerticalAngle =
			UpperFovLimit - static_cast<float>(i) * DeltaAngle;
		LaserAngles.Emplace(VerticalAngle);
	}
}

void AFirstLidar::SimulateLidar(float DeltaTime)
{
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
	
	const float CurrentHorizontalAngle = GetCurrentLidarHorizontalAngle();

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
				continue;
			}
		};
	}

	const float HorizontalAngle = std::fmod(CurrentHorizontalAngle + AngleDistanceOfTick, HorizontalFov);
	SetCurrentLidarHorizontalAngle(HorizontalAngle);
}

bool AFirstLidar::ShootLaser(const float VerticalAngle, float HorizontalAngle, FHitResult& HitResult) const
{
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Laser_Trace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	FHitResult HitInfo(ForceInit);

	FTransform ActorTransf = GetTransform();
	FVector LidarBodyLoc = ActorTransf.GetLocation();
	FRotator LidarBodyRot = ActorTransf.Rotator();
	FRotator LaserRot(VerticalAngle, HorizontalAngle, 0);  // float InPitch, float InYaw, float InRoll
	FRotator ResultRot = UKismetMathLibrary::ComposeRotators(
		LaserRot,
		LidarBodyRot
	);
	FVector EndTrace = Range * UKismetMathLibrary::GetForwardVector(ResultRot) + LidarBodyLoc;

	GetWorld()->LineTraceSingleByChannel(
		HitInfo,
		LidarBodyLoc,
		EndTrace,
		ECC_GameTraceChannel2,
		TraceParams,
		FCollisionResponseParams::DefaultResponseParam
	);

	//DrawDebugLine(GetWorld(), LidarBodyLoc, EndTrace, FColor::Orange, false, 0.1f);

	if (HitInfo.bBlockingHit) {
		DrawDebugPoint(
			GetWorld(),
			HitInfo.ImpactPoint,
			5,                       //size
			FColor::Red,
			false,                    //persistent (never goes away)
			0.1                      //point leaves a trail on moving object
		);

		HitResult = HitInfo;
		return true;
	}
	else {
		return false;
	}
}

void AFirstLidar::PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaTime)
{
	if (!isPaused)
	{
		SimulateLidar(DeltaTime);
	}
	else {
		return;
	}
}

void AFirstLidar::StartScanning(bool state)
{
	this->isPaused = state;
}

bool AFirstLidar::getIsPaused()
{
	return this->isPaused;
}


void AFirstLidar::SetCurrentLidarHorizontalAngle(float HorizontalAngle)
{
	this->CurrentLidarHorizontalAngle = HorizontalAngle;
}

float AFirstLidar::GetCurrentLidarHorizontalAngle()
{
	return CurrentLidarHorizontalAngle;
}

