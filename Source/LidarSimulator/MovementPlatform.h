
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovementPlatform.generated.h"

UCLASS()
class LIDARSIMULATOR_API AMovementPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovementPlatform();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		void AddPoint(FVector Point);
	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		bool GetbIsActive();
	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		void SetbIsActive(bool State);
	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		void ResetStateVariablesToInitial();
	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		void DeletePoints();
	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		FVector GetCurrentLocation();
	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		void SetCurrentLocation(FVector CurrentLocation_);
	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		float GetSpeed();
	UFUNCTION(BlueprintCallable, Category = "Platform settings")
		void SetSpeed(float Speed_);

private:
	void MoveToPoint(float DeltaSeconds);

	float Speed = 150.0f;
	FVector CurrentLocation;
	FVector TargetLocation;
	TArray<FVector> PointsArray;

	bool bIsActive = false;
	bool bIsTagetReached = false;
	//rotate
	bool bIsRotated = false;

	//state variables
	int CurrentIndexPoint = 0;
};
