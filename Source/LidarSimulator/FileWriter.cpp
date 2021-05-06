// Fill out your copyright notice in the Description page of Project Settings.


#include "FileWriter.h"

FileWriter::FileWriter()
{
	UE_LOG(LogTemp, Warning, TEXT("Constructor FileWriter"));
}

FileWriter::~FileWriter()
{
	UE_LOG(LogTemp, Warning, TEXT("Destructor FileWriter"));
}

void FileWriter::TestMethod()
{
	UE_LOG(LogTemp, Warning, TEXT("TestMethod from FileWriter"));
}

void FileWriter::CreateOutputDirectory()
{

}
void FileWriter::CreateOutputFile(FString OutputFileName)
{

}
void FileWriter::WriteBufferToFile(FString OutputFileName, std::vector<FVector>& Buffer)
{
	FString ProjectDirectory = FPaths::ProjectDir();
	FString OutputDirectory = ProjectDirectory + (TEXT("Output"));
	/*FString OutputPathToFile = OutputDirectory + FGenericPlatformMisc::GetDefaultPathSeparator() + (TEXT("Test.txt"));*/
	FString OutputPathToFile = OutputDirectory + FGenericPlatformMisc::GetDefaultPathSeparator() + OutputFileName;

	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	bool bOutputDirectoryIsCreated = false;

	if (FileManager.DirectoryExists(*OutputDirectory))
	{
		UE_LOG(LogTemp, Warning, TEXT("Directory Exists"));
		bOutputDirectoryIsCreated = true;
	}
	else
	{
		if (FileManager.CreateDirectory(*OutputDirectory))
		{
			UE_LOG(LogTemp, Warning, TEXT("Directory was created"));
			bOutputDirectoryIsCreated = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error: Directory was not created"));
			return;
		}
	}

	if (bOutputDirectoryIsCreated)
	{
		FString ResultPointCloud = "";
		for (size_t i = 0; i < Buffer.size(); i++)
		{
			FVector Point = Buffer[i];
			ResultPointCloud.Append(FString::SanitizeFloat(Point[0]) + " ")
							.Append(FString::SanitizeFloat(Point[1]) + " ")
							.Append(FString::SanitizeFloat(Point[2]) + "\n");
		}
		if (FFileHelper::SaveStringToFile(ResultPointCloud, *OutputPathToFile, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append))
		{
			UE_LOG(LogTemp, Warning, TEXT("Sucsesfuly PointCloud Written to the text file"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error: Failed to write PointCloud to file."));
		}
	}
}
