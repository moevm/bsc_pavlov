
#include "BaseLidar.h"

#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

ABaseLidar::ABaseLidar()
{
	++ID;
	LidarName = GetName() + "_" + FString::FromInt(ID);
	PointCloudFileName = LidarName + "_" + FDateTime::Now().ToString() + ".xyz";
	PrimaryActorTick.bCanEverTick = true;
	PointCloudWriter = new FileWriter();
	UE_LOG(LogTemp, Warning, TEXT("LidarName = %s"), *LidarName);
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
	/*if (this->bIsPaused || IsScanCompleted())*/
	if (this->bIsPaused || bIsPointCloudRecorded)
	{
		return;
	}
	else if (IsScanCompleted())
	{
		UE_LOG(LogTemp, Warning, TEXT("Write to file"));
		PointCloudWriter->WriteBufferToFile(PointCloudFileName, PointCloud);
		PointCloud.clear();
		bIsPointCloudRecorded = true;
	}
	else
	{
		SimulateScannig(DeltaSeconds);
	}
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
				5,                       //size
				FColor::Red,
				true,                    //persistent (never goes away)
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
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Base")
	);
}


void ABaseLidar::StopScannig()
{
	SetActorTickEnabled(false);
	FWorldDelegates::OnWorldPostActorTick.Remove(OnPostTickDelegate);
}

void ABaseLidar::PauseScanning(bool State)
{
	this->bIsPaused = State;
}

bool ABaseLidar::GetIsPaused()
{
	return this->bIsPaused;
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





