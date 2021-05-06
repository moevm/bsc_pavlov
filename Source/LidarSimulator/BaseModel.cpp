
#include "BaseModel.h"

ABaseModel::ABaseModel()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

FString ABaseModel::GetModelName()
{
	return ModelName;
}

void ABaseModel::SetModelName(FString ModelName_)
{
	this->ModelName = ModelName_;
}


