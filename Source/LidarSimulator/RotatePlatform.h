
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatePlatform.generated.h"

UCLASS()
class LIDARSIMULATOR_API ARotatePlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	ARotatePlatform();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		void AddPoint(float TimeInterval, float RotationSpeed, FVector RotationVector);
	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		void DeletePoints();
	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		bool GetbIsActive();
	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		void SetbIsActive(bool State);
	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		void ResetStateVariablesToInitial();
	UFUNCTION(BlueprintCallable, Category = "Platform Name")
		FString GetNameRotatePlatform();
	UFUNCTION(BlueprintCallable, Category = "Platform Name")
		void SetNameRotatePlatform(FString RotatePlatformName_);
private:
	void ChangeRotateAngle();

	FString RotatePlatformName;
	float CurrentTimeInterval;
	float CurrentRotationSpeed;
	FVector CurrentRotationVector;

	int CurrentIndexInterval = 0;

	bool bIsTurnPlatfomAtAngle = true;
	bool bIsActive = false;

	TArray<float> TimeIntervalArray;
	TArray<float> RotatingSpeedArray;
	TArray<FVector> RotationVectorArray;

	FTimerHandle RotateTimerHandle;
};
