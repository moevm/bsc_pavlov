
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseModel.generated.h"

UCLASS()
class LIDARSIMULATOR_API ABaseModel : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;
	ABaseModel();
	UFUNCTION(BlueprintCallable, Category = "Model Name")
		FString GetModelName();
	UFUNCTION(BlueprintCallable, Category = "Model Name")
		void SetModelName(FString ModelName_);
private:
	FString ModelName;
};
