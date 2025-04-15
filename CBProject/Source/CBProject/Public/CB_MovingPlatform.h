#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CB_PlatformInterface.h"
#include "Components/SceneComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "CB_MovingPlatform.generated.h"

UCLASS()
class CBPROJECT_API ACB_MovingPlatform : public AActor, public ICB_PlatformInterface
{
	GENERATED_BODY()
	
public:	
	
	ACB_MovingPlatform();

	// 현재 발판이 활성화(이동 중) 상태인지 여부
	UPROPERTY(ReplicatedUsing = OnRep_IsActive)
	bool bIsActive = false;

	UFUNCTION()
	void OnRep_IsActive();
	
protected:
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:	
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* PlatformMesh;

	// 이동 속도 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Movement")
	float MoveSpeed = 100.0f;

	// 시작 지점과 끝 지점 사이를 이동할지, 아니면 한 방향으로만 이동할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Movement", meta = (ToolTip = "If true, moves back and forth between start and end points. If false, moves only towards the end point."))
	bool bPingPong = true;

	// 각 끝 지점에서 대기할 시간 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Movement", meta = (EditCondition = "bPingPong")) // bPingPong이 true일 때만 편집 가능
	float WaitTime = 1.0f;

	// 목표 지점 (액터의 초기 위치 기준 상대 좌표)
	// MakeEditWidget을 사용하면 레벨 에디터에서 이 위치를 시각적으로 편집할 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Movement", meta = (MakeEditWidget = true))
	FVector TargetRelativeLocation = FVector(0.0f, 0.0f, 500.0f); // 기본값: 위로 500cm 이동

	// 게임 시작 시 자동으로 이동을 시작할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Activation")
	bool bStartActive = true;

	

	//인터페이스
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Platform")
	void ActivatePlatform();
	virtual void ActivatePlatform_Implementation() override; 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Platform")
	void DeactivatePlatform();
	virtual void DeactivatePlatform_Implementation() override; 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Platform")
	bool IsPlatformActive();
	virtual bool IsPlatformActive_Implementation() override;

	
private:
	// 월드 좌표계의 시작 위치
	FVector GlobalStartLocation;

	// 월드 좌표계의 목표 위치
	FVector GlobalTargetLocation;

	// 현재 목표 지점으로 이동 중인지 여부 (true: TargetLocation으로, false: StartLocation으로)
	bool bMovingToTarget = true;

	// 현재 대기 시간 카운터
	float CurrentWaitTime = 0.0f;
	
	// 이동 방향 벡터 (정규화됨)
	FVector MoveDirection;

	
};
