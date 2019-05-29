
#pragma once

#include "AIModule/Classes/EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_IsContextShootableFromCoverPoint.generated.h"


UCLASS()
class UEnvQueryTest_IsContextShootableFromCoverPoint : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_IsContextShootableFromCoverPoint(const FObjectInitializer& ObjectInitializer);

	/** context: other end of trace test */
	UPROPERTY(EditDefaultsOnly, Category = Trace)
	TSubclassOf<UEnvQueryContext> Context;

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
