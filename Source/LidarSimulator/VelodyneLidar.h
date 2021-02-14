// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VelodyneLidar.generated.h"

UCLASS()
class LIDARSIMULATOR_API AVelodyneLidar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVelodyneLidar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	//lidar description

	int number_of_lasers = 32;

	float vertical_FOV_upper = 30.0;

	float current_horizontal_angle_ = 0.0f;
	TArray<float> LaserAngles;
};
