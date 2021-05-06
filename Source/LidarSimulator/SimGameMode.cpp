// Fill out your copyright notice in the Description page of Project Settings.


#include "SimGameMode.h"

void ASimGameMode::BeginPlay()
{
	Super::BeginPlay();
}


void ASimGameMode::AddLidarActorToArray(ABaseLidar* LidarActor)
{
	LidarArray.Add(LidarActor);
}

void ASimGameMode::StartSimulation()
{
	for (int i = 0; i < LidarArray.Num(); i++)
	{
		LidarArray[i]->SetPointCloudFileName("");
		LidarArray[i]->SetIsActive(true);
	}
	for (int i = 0; i < RotatePlatformArray.Num(); i++)
	{
		RotatePlatformArray[i]->SetbIsActive(true);
	}
	if (MovementPlatform != nullptr)
	{
		MovementPlatform->SetbIsActive(true);
	}
	SetSimulationTime();
}

void ASimGameMode::StopSimulation()
{
	GetWorldTimerManager().ClearTimer(SimulationTimerHandle);
	for (int i = 0; i < LidarArray.Num(); i++)
	{
		LidarArray[i]->SetIsActive(false);
		LidarArray[i]->ResetStateVariablesToInitial();
	}
	for (int i = 0; i < RotatePlatformArray.Num(); i++)
	{
		RotatePlatformArray[i]->SetbIsActive(false);
		RotatePlatformArray[i]->ResetStateVariablesToInitial();
	}
	if (MovementPlatform != nullptr)
	{
		MovementPlatform->SetbIsActive(false);
		MovementPlatform->ResetStateVariablesToInitial();
	}
}

void ASimGameMode::DeleteLidar(ABaseLidar* LidarActor)
{
	if (LidarActor != nullptr)
	{
		LidarActor->Destroy();
		LidarArray.Remove(LidarActor);
	}
}

void ASimGameMode::DeleteMovementPlatform()
{
	if (MovementPlatform != nullptr)
	{
		MovementPlatform->Destroy();
	}
}

ABaseLidar* ASimGameMode::FindLidarByName(FString LidarName_)
{
	for (int i = 0; i < LidarArray.Num(); i++)
	{
		if ((LidarArray[i]->GetNameLidar()).Compare(LidarName_) == 0)
		{
			return LidarArray[i];
		}
	}
	return nullptr;
}

TArray<ABaseLidar*> ASimGameMode::GetLidarArray()
{
	return LidarArray;
}

void ASimGameMode::SetSimulationTime()
{
	GetWorldTimerManager().SetTimer(SimulationTimerHandle, this, &ASimGameMode::StopSimulation, ScanningDurationTime, false, ScanningDurationTime);
}

void ASimGameMode::SetScanningDurationTime(float ScanningDurationTime_)
{
	this->ScanningDurationTime = ScanningDurationTime_;
}

float ASimGameMode::GetScanningDurationTime()
{
	return ScanningDurationTime;
}

AMovementPlatform* ASimGameMode::GetMovementPlatfrom()
{
	return MovementPlatform;
}

void ASimGameMode::SetMovementPlatform(AMovementPlatform* MovementPlatform_)
{
	this->MovementPlatform = MovementPlatform_;
}

void ASimGameMode::SetbIsDrawLines(bool State)
{
	this->bIsDrawLines = State;
	for (int i = 0; i < LidarArray.Num(); i++)
	{
		LidarArray[i]->SetbDrawLaserLines(bIsDrawLines);
	}
}

bool ASimGameMode::GetbIsDrawLines()
{
	return bIsDrawLines;
}

void ASimGameMode::SetbIsDrawPoints(bool State)
{
	this->bIsDrawPoints = State;
	for (int i = 0; i < LidarArray.Num(); i++)
	{
		LidarArray[i]->SetbDrawHitPoints(bIsDrawPoints);
	}
}

bool ASimGameMode::GetbIsDrawPoints()
{
	return bIsDrawPoints;
}

void ASimGameMode::SetbInWorldCoordinates(bool State)
{
	this->bInWorldCoordinates = State;
	for (int i = 0; i < LidarArray.Num(); i++)
	{
		LidarArray[i]->SetbInWorldCoordinates(bInWorldCoordinates);
	}
}

bool ASimGameMode::GetbInWorldCoordinates()
{
	return bInWorldCoordinates;
}

void ASimGameMode::SetAtmospAttenRate(float AtmospAttenRate_)
{
	this->AtmospAttenRate = AtmospAttenRate_;
	for (int i = 0; i < LidarArray.Num(); i++)
	{
		LidarArray[i]->SetAtmospAttenRate(AtmospAttenRate);
	}
}

float ASimGameMode::GetAtmospAttenRate()
{
	return AtmospAttenRate;
}

void ASimGameMode::AddRotatePlatformToArray(ARotatePlatform* RotatePlatform)
{
	RotatePlatformArray.Add(RotatePlatform);
}

void ASimGameMode::DeleteRotatePlatform(ARotatePlatform* RotatePlatform)
{
	if (RotatePlatform != nullptr)
	{
		RotatePlatform->Destroy();
		RotatePlatformArray.Remove(RotatePlatform);
	}
}

ARotatePlatform* ASimGameMode::FindRotatePlatformByName(FString RotatePlatformName_)
{
	for (int i = 0; i < RotatePlatformArray.Num(); i++)
	{
		if ((RotatePlatformArray[i]->GetNameRotatePlatform()).Compare(RotatePlatformName_) == 0)
		{
			return RotatePlatformArray[i];
		}
	}
	return nullptr;
}

TArray<ARotatePlatform*> ASimGameMode::GetRotatePlatformArray()
{
	return RotatePlatformArray;
}

ABaseModel* ASimGameMode::FindModelByName(FString ModelName_)
{
	for (int i = 0; i < ModelArray.Num(); i++)
	{
		if ((ModelArray[i]->GetModelName()).Compare(ModelName_) == 0)
		{
			return ModelArray[i];
		}
	}
	return nullptr;
}

void ASimGameMode::DeleteModel(ABaseModel* Model)
{
	if (Model != nullptr)
	{
		Model->Destroy();
		ModelArray.Remove(Model);
	}
}

void ASimGameMode::AddModelToArray(ABaseModel* Model)
{
	ModelArray.Add(Model);
}

TArray<ABaseModel*> ASimGameMode::GetModelArray()
{
	return ModelArray;
}

void ASimGameMode::AttachLidarToMovementPlatform(ABaseLidar* LidarActor)
{
	if (MovementPlatform != nullptr && LidarActor != nullptr)
	{
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, false);
		LidarActor->AttachToActor(MovementPlatform, Rules, FName("LidarUpSocket"));
	}
}

void ASimGameMode::AttachLidarToRotatePlatform(ABaseLidar* LidarActor, ARotatePlatform* RotatePlatform)
{
	if (RotatePlatform != nullptr && LidarActor != nullptr)
	{
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, false);
		LidarActor->AttachToActor(RotatePlatform, Rules, FName("LidarSocket"));
	}
}

void ASimGameMode::AttachRotatePlatformToMovementPlatform(ARotatePlatform* RotatePlatform)
{
	if (MovementPlatform != nullptr && RotatePlatform != nullptr)
	{
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, false);
		RotatePlatform->AttachToActor(MovementPlatform, Rules, FName("LidarUpSocket"));
	}
}