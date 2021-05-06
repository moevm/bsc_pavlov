
#include "RotatePlatform.h"

ARotatePlatform::ARotatePlatform()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	RotatePlatformName = GetName();
	PrimaryActorTick.bCanEverTick = true;
}

void ARotatePlatform::BeginPlay()
{
	Super::BeginPlay();
}

void ARotatePlatform::ChangeRotateAngle()
{
	bIsTurnPlatfomAtAngle = true;
}


void ARotatePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsActive || (CurrentIndexInterval == TimeIntervalArray.Num() + 1))
	{
		return;
	}

	if (bIsTurnPlatfomAtAngle)
	{
		if (CurrentIndexInterval == TimeIntervalArray.Num())
		{
			ResetStateVariablesToInitial();
			bIsActive = false;
			return;
		}

		CurrentTimeInterval = TimeIntervalArray[CurrentIndexInterval];
		CurrentRotationSpeed = RotatingSpeedArray[CurrentIndexInterval];
		CurrentRotationVector = RotationVectorArray[CurrentIndexInterval];
		CurrentIndexInterval++;
		//actor world rotate
		SetActorRotation(FRotator(CurrentRotationVector.X, CurrentRotationVector.Y, CurrentRotationVector.Z));

		bIsTurnPlatfomAtAngle = false;
		GetWorldTimerManager().SetTimer(RotateTimerHandle, this, &ARotatePlatform::ChangeRotateAngle, CurrentTimeInterval, false, CurrentTimeInterval);
	}

	// actor local rotation
	float DeltaRotation = DeltaTime * CurrentRotationSpeed;    //Rotation per second is equal to the angle of RotationSpeed
	FRotator NewRotation(0.0, DeltaRotation, 0.0);
	AddActorLocalRotation(NewRotation, false, 0, ETeleportType::None);
}

void ARotatePlatform::AddPoint(float TimeInterval, float RotationSpeed, FVector RotationVector) 
{
	TimeIntervalArray.Add(TimeInterval);
	RotatingSpeedArray.Add(RotationSpeed);
	RotationVectorArray.Add(RotationVector);
}

void ARotatePlatform::DeletePoints()
{
	TimeIntervalArray.Empty();
	RotatingSpeedArray.Empty();
	RotationVectorArray.Empty();
}

bool ARotatePlatform::GetbIsActive()
{
	return bIsActive;
}

void ARotatePlatform::SetbIsActive(bool State)
{
	this->bIsActive = State;
}

void ARotatePlatform::ResetStateVariablesToInitial()
{
	this->CurrentIndexInterval = 0;
}

FString ARotatePlatform::GetNameRotatePlatform()
{
	return RotatePlatformName;
}

void ARotatePlatform::SetNameRotatePlatform(FString RotatePlatformName_)
{
	this->RotatePlatformName = RotatePlatformName_;
}
