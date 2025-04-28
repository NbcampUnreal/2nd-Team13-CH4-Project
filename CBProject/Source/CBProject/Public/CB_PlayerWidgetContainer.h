// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "CB_PlayerInfoWidget.h"
#include "CB_PlayerState.h"
#include "CB_PlayerWidgetContainer.generated.h"

/**
 * 
 */
UCLASS()
class CBPROJECT_API UCB_PlayerWidgetContainer : public UUserWidget
{
	GENERATED_BODY()

public:
    // HorizontalBox를 바인딩할 UPROPERTY 선언
    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* PlayerInfoContainer;
    //UPROPERTY(meta = (BindWidget))
	//UVerticalBox* Team1Container;
    //UPROPERTY(meta = (BindWidget))
    //UVerticalBox* Team2Container;


    // 플레이어 정보 위젯 클래스 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UCB_PlayerInfoWidget> PlayerInfoItemClass;

    // 위젯 초기화 함수
    virtual void NativeConstruct() override;

    // UI 업데이트 함수 (서버에서 데이터가 변경될 때 호출 될 예정)
    UFUNCTION()
    void UpdatePlayerList();
	
};
