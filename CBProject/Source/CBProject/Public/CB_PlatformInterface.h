#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CB_PlatformInterface.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class UCB_PlatformInterface : public UInterface
{
	GENERATED_BODY()
};


class CBPROJECT_API ICB_PlatformInterface
{
	GENERATED_BODY()

	
public:
	// 발판을 활성화하는 함수 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Platform")
	void ActivatePlatform();

	// 발판을 비활성화하는 함수 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Platform")
	void DeactivatePlatform();

	// 발판이 현재 활성 상태인지 확인하는 함수 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Platform")
	bool IsPlatformActive();
};
