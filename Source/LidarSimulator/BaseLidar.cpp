
#include "BaseLidar.h"

#include "Math/Vector.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

ABaseLidar::ABaseLidar()
{
	//create static mesh lidar
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	LidarName = GetName();
	PointCloudFileName = LidarName + "_" + FDateTime::Now().ToString() + ".csv";
	PrimaryActorTick.bCanEverTick = true;
	PointCloudWriter = std::make_unique<FileWriter>();
}

void ABaseLidar::BeginPlay()
{
	Super::BeginPlay();
	OnPostTickDelegate = FWorldDelegates::OnWorldPostActorTick.AddUObject(
		this, &ABaseLidar::PostPhysTick);
}

void ABaseLidar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseLidar::PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	if (!bIsActive)
	{
		return;
	}
	else
	{
		LidarDetectionPointCloud.clear();
		SimulateScannig(DeltaSeconds);
		PointCloudWriter->WriteBufferToFile(PointCloudFileName, LidarDetectionPointCloud);
		LidarDetectionPointCloud.clear();
	}
}

void ABaseLidar::SimulateScannig(float DeltaTime)
{
	TimeStampLidar += DeltaTime;
}


bool ABaseLidar::ShootLaser(const float VerticalAngle, float HorizontalAngle, FHitResult& HitResult) const
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

	if (bDrawLaserLines)
	{
		DrawDebugLine(GetWorld(), LidarBodyLoc, EndTrace, FColor::Orange, false, 0.1f);
	}

	if (HitInfo.bBlockingHit) 
	{
		if (bDrawHitPoints)
		{
			DrawDebugPoint(
				GetWorld(),
				HitInfo.ImpactPoint,
				2,                       //size
				FColor::Red,
				false,                    //persistent (never goes away)
				0.1                      //point leaves a trail on moving object
			);
		}
		HitResult = HitInfo;
		return true;
	}
	else 
	{
		return false;
	}
}

bool ABaseLidar::IsScanCompleted()
{
	return false;
}

LidarDetection ABaseLidar::ComputeDetection(FTransform& LidarTransform, FHitResult& HitResult)
{
	LidarDetection Detection;
	if (bInWorldCoordinates)
	{
		Detection.Point = HitResult.ImpactPoint;
	}
	else
	{
		const FVector HitPoint = HitResult.ImpactPoint;
		Detection.Point = LidarTransform.Inverse().TransformPosition(HitPoint);
	}
	AddGaussianNoise(Detection);
	ComputeIntensity(Detection);
	Detection.TimeStamp = TimeStampLidar;
	return Detection;
}

void ABaseLidar::AddGaussianNoise(LidarDetection& Detection)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::normal_distribution<double> distribution(0.0, NoiseStdDev);
	FVector UnitVector = Detection.Point;
	UnitVector.Normalize();
	FVector Noise = UnitVector * distribution(generator);
	Detection.Point += Noise;
}

void ABaseLidar::ComputeIntensity(LidarDetection& Detection)
{
	float Distance = Detection.Point.Size();
	float Intensity = exp(-AtmospAttenRate * Distance);
	Detection.Intensity = Intensity;
}

void ABaseLidar::SetIsActive(bool State)
{
	this->bIsActive = State;
}
bool ABaseLidar::GetIsActive()
{
	return this->bIsActive;
}

FString ABaseLidar::GetNameLidar()
{
	return LidarName;
}

void ABaseLidar::SetLidarName(FString LidarName_)
{
	this->LidarName = LidarName_;
}

FString ABaseLidar::GetPointCloudFileName()
{
	return PointCloudFileName;
}

void ABaseLidar::SetPointCloudFileName(FString PointCloudFileName_)
{
	this->PointCloudFileName = LidarName + "_" + FDateTime::Now().ToString() + ".csv";
}

void ABaseLidar::SetRange(float Range_)
{
	this->Range = Range_;
}

float ABaseLidar::GetRange()
{
	return Range;
}

void ABaseLidar::ApplySettings(float Range_, float NoiseStdDev_)
{
	this->Range = Range_;
	this->NoiseStdDev = NoiseStdDev_;
}

void ABaseLidar::ResetStateVariablesToInitial()
{
	this->TimeStampLidar = 0.0;
}

void ABaseLidar::SetbDrawLaserLines(bool State)
{
	this->bDrawLaserLines = State;
}

bool ABaseLidar::GetbDrawLaserLines()
{
	return bDrawLaserLines;
}

void ABaseLidar::SetbDrawHitPoints(bool State)
{
	this->bDrawHitPoints = State;
}

bool ABaseLidar::GetbDrawHitPoints()
{
	return bDrawHitPoints;
}

void ABaseLidar::SetbInWorldCoordinates(bool State)
{
	this->bInWorldCoordinates = State;
}
bool ABaseLidar::GetbInWorldCoordinates()
{
	return bInWorldCoordinates;
}

void ABaseLidar::SetNoiseStdDev(float NoiseStdDev_)
{
	this->NoiseStdDev = NoiseStdDev_;
}

float ABaseLidar::GetNoiseStdDev()
{
	return NoiseStdDev;
}

void ABaseLidar::SetAtmospAttenRate(float AtmospAttenRate_)
{
	this->AtmospAttenRate = AtmospAttenRate_;
}

float ABaseLidar::GetAtmospAttenRate()
{
	return AtmospAttenRate;
}






