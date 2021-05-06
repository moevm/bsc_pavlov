

#include "MovementPlatform.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Rotator.h"

AMovementPlatform::AMovementPlatform()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	PrimaryActorTick.bCanEverTick = true;
}

void AMovementPlatform::BeginPlay()
{
	Super::BeginPlay();
	CurrentLocation = GetActorLocation();
}

void AMovementPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsActive || (CurrentIndexPoint == PointsArray.Num()))
	{
		bIsActive = false;
		if (CurrentIndexPoint == PointsArray.Num())
		{
			ResetStateVariablesToInitial();
		}
		return;
	}

	if (bIsTagetReached)
	{
		CurrentIndexPoint++;
		bIsTagetReached = false;
	}
	else
	{
		TargetLocation = PointsArray[CurrentIndexPoint];
		if (!bIsRotated)
		{	
			FRotator PlatformRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
			SetActorRotation(PlatformRot);
			bIsRotated = true;
		}
		MoveToPoint(DeltaTime);
		if (TargetLocation == CurrentLocation)
		{
			bIsTagetReached = true;
			bIsRotated = false;
		}
	}
}

void AMovementPlatform::MoveToPoint(float DeltaSeconds)
{
	CurrentLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaSeconds, Speed);
	SetActorLocation(CurrentLocation);
}

void AMovementPlatform::AddPoint(FVector Point)
{
	PointsArray.Add(Point);
}

bool AMovementPlatform::GetbIsActive()
{
	return bIsActive;
}


void AMovementPlatform::SetbIsActive(bool State)
{
	this->bIsActive = State;
}

void AMovementPlatform::ResetStateVariablesToInitial()
{
	this->CurrentIndexPoint = 0;

}

void AMovementPlatform::DeletePoints()
{
	PointsArray.Empty();
}

FVector AMovementPlatform::GetCurrentLocation()
{
	return CurrentLocation;
}

void AMovementPlatform::SetCurrentLocation(FVector CurrentLocation_)
{
	this->CurrentLocation = CurrentLocation_;
}

float AMovementPlatform::GetSpeed()
{
	return Speed;
}

void AMovementPlatform::SetSpeed(float Speed_)
{
	this->Speed = Speed_;
}

