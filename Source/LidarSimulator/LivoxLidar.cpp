// Fill out your copyright notice in the Description page of Project Settings.


#include "LivoxLidar.h"

#include <cmath>
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

ALivoxLidar::ALivoxLidar()
	: ABaseLidar()
{
	CalculateAngleBetweenPulses();
	CalculateAngleBetweenRoses();
}

void ALivoxLidar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALivoxLidar::CalculateAngleBetweenPulses() 
{
	AngleBetweenPulses = Theta / PointsInOneRose;
}

void ALivoxLidar::CalculateAngleBetweenRoses()
{
	AngleBetweenRoses = W / (NumberRoses - 1);
}

void ALivoxLidar::CalculateCurrentThetaAngle()
{
	if (CurrentThetaAngle >= PI)
	{
		CurrentThetaAngle = 0.0;
		CurrentW += AngleBetweenRoses;
		CurrentNumberRoses++;
		/*UE_LOG(
			LogTemp,
			Warning,
			TEXT("currentW: %f"),
			currentW
		);*/
	}
	else
	{
		CurrentThetaAngle += AngleBetweenPulses;
	}
}

void ALivoxLidar::SetVerticalAngle()
{
	CurrentVerticalAngle = UKismetMathLibrary::Atan(CurrentR * UKismetMathLibrary::Sin(CurrentThetaAngle) 
													* UKismetMathLibrary::Tan(FieldOfView * PI / (2 * 180))) * 180 / PI;
}

void ALivoxLidar::SetHorizontalAngle()
{
	CurrentHorizontalAngle = UKismetMathLibrary::Atan(CurrentR * UKismetMathLibrary::Cos(CurrentThetaAngle) 
													* UKismetMathLibrary::Tan(FieldOfView * PI / (2 * 180))) * 180 / PI;
}

float ALivoxLidar::ScanningFunction()
{
	return UKismetMathLibrary::Sin(NumberPetals * (CurrentThetaAngle - CurrentW));
}

bool ALivoxLidar::IsScanCompleted()
{
	return CurrentNumberRoses >= NumberRoses;
}

void ALivoxLidar::DoSimStep()
{
	CountPoints++;
	CurrentR = ScanningFunction();
	SetVerticalAngle();
	SetHorizontalAngle();
	/*UE_LOG(
		LogTemp,
		Warning,
		TEXT("Point: %d; Theta =  %f; r = %f; x_angle = %f; y_angle = %f"),
		countPoints, currentThetaAngle, currentR, currentHorizontalAngle, currentVerticalAngle
	);*/
	CalculateCurrentThetaAngle();

	FHitResult HitResult;
	if (ShootLaser(CurrentVerticalAngle, CurrentHorizontalAngle, HitResult))
	{
		PointCloud.emplace_back(HitResult.ImpactPoint);
	}
}

void ALivoxLidar::SimulateScannig(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString::Printf(TEXT("Scanning")));
	const uint32 PointsInOneFrame = FMath::RoundHalfFromZero(PointsPerSecond * DeltaTime);
	for (size_t i = 0; i < PointsInOneFrame; i++)
	{
		DoSimStep();
	}
}