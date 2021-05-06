// Fill out your copyright notice in the Description page of Project Settings.


#include "SimGameMode.h"

void ASimGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ASimGameMode"));
	fw = new FileWriter();
	fw->TestMethod();
	FVector a = { 0.1, 0.3, 0.4 };
	//fw->WriteBufferToFile("f", a);
}