
#pragma once

#include <vector>
#include <chrono>
#include <random>
#include <memory>
#include "CoreMinimal.h"
#include "LidarDetection.h"
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

	//static mesh lidar
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;

	UFUNCTION(BlueprintCallable, Category = "Lidar Name")
		FString GetNameLidar();
	UFUNCTION(BlueprintCallable, Category = "Lidar Name")
		void SetLidarName(FString LidarName_);

	UFUNCTION(BlueprintCallable, Category = "PointCloudFileName")
		FString GetPointCloudFileName();
	UFUNCTION(BlueprintCallable, Category = "PointCloudFileName")
		void SetPointCloudFileName(FString PointCloudFileName_);

	UFUNCTION(BlueprintCallable, Category = "Pause")
		void SetIsActive(bool State);
	UFUNCTION(BlueprintCallable, Category = "Pause")
		bool GetIsActive();

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetRange(float Range_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetRange();

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		virtual void ApplySettings(float Range_, float NoiseStdDev_);

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		virtual void ResetStateVariablesToInitial();

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetbDrawLaserLines(bool State);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		bool GetbDrawLaserLines();

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetbDrawHitPoints(bool State);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		bool GetbDrawHitPoints();

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetbInWorldCoordinates(bool State);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		bool GetbInWorldCoordinates();

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetNoiseStdDev(float NoiseStdDev_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetNoiseStdDev();

	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		void SetAtmospAttenRate(float AtmospAttenRate_);
	UFUNCTION(BlueprintCallable, Category = "LidarSettings")
		float GetAtmospAttenRate();

protected:
	virtual void BeginPlay() override;
	void PostPhysTick(UWorld* World, ELevelTick TickType, float DeltaSeconds);
	virtual void SimulateScannig(float DeltaTime);
	bool ShootLaser(const float VerticalAngle, float HorizontalAngle, FHitResult& HitResult) const;
	virtual bool IsScanCompleted();

	void AddGaussianNoise(LidarDetection& Detection);
	void ComputeIntensity(LidarDetection& Detection);
	LidarDetection ComputeDetection(FTransform& LidarTransform, FHitResult& HitResult);

	std::unique_ptr<FileWriter> PointCloudWriter;

	//Point cloud
	std::vector<LidarDetection> LidarDetectionPointCloud;
	FString PointCloudFileName;

private:
	//Simulation settings
	bool bDrawLaserLines = false;
	bool bDrawHitPoints = false;
	bool bIsActive = false;
	bool bInWorldCoordinates = false;
	float TimeStampLidar = 0.0;
	FDelegateHandle OnPostTickDelegate;

	//Lidar settings
	FString LidarName;
	float Range = 1500.0f;

	//Noise settings
	float NoiseStdDev = 0.1;
	float AtmospAttenRate = 0.004f;
};
