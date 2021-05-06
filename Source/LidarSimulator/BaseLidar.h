
#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "FileWriter.h"
#include "GameFramework/Actor.h"
#include "BaseLidar.generated.h"

UCLASS()
class LIDARSIMULATOR_API ABaseLidar : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseLidar();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	void PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds);
	virtual void SimulateScannig(float DeltaTime) {}
	bool ShootLaser(const float VerticalAngle, float HorizontalAngle, FHitResult& HitResult) const;
	virtual bool IsScanCompleted();
	void StopScannig();
	FString GetNameLidar();

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void PauseScanning(bool State);
	UFUNCTION(BlueprintCallable, Category = "Pause")
	bool GetIsPaused();

	void SetIsActive(bool State);
	bool GetIsActive();

	FileWriter* PointCloudWriter;

	//Point cloud
	std::vector<FVector> PointCloud;
	FString PointCloudFileName;

private:

	//Simulation settings
	bool bDrawLaserLines = true;
	bool bDrawHitPoints = false;
	bool bIsPointCloudRecorded = false;
	bool bIsPaused = true;
	bool bIsActive = false;
	FDelegateHandle OnPostTickDelegate;


	//Lidar settings
	FString LidarName;
	static uint32 ID;
	float Range = 1000.0f;
};

uint32 ABaseLidar::ID = 0;
