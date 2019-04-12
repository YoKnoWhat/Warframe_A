
#pragma once

#include "Runtime/AIModule/Classes/EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_CoverPointFree.generated.h"


UCLASS()
class UEnvQueryTest_CoverPointFree : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()

	/** context */
	UPROPERTY(EditDefaultsOnly, Category = Test)
	TSubclassOf<UEnvQueryContext> ConsiderFree;

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
