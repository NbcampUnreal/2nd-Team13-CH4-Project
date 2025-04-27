// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CB_CharacterSelect.generated.h"

class UVerticalBox;
class UButton;
class UCheckBox;
class UCB_PlayerSelectCharacterWidget;

UCLASS()
class CBPROJECT_API UCB_CharacterSelect : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox_ClientList;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Character1;
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Character2;
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Character3;
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Character4;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* CheckBox_Team;
	
	UPROPERTY(meta = (BindWidget))
	UButton* GameStartButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Select")
	TSubclassOf<UCB_PlayerSelectCharacterWidget> PlayerSelectCharacterWidgetClass;

	virtual void NativeConstruct() override;


	void CreatePlayerSlots(); // 플레이어를 리스트에 추가
	void UpdatePlayerTeam(); // 플레이어 리스트 업데이트
	

	void SetStartButtonEnabled();// 플레이어가 모두 캐릭터를 선택했을 때 시작 버튼 활성화

	UFUNCTION()
	void SetIsTeam();// 팀	선택 체크박스 클릭 시 팀 변경
	
};
