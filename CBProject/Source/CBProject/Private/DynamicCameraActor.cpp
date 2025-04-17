#include "Kismet/GameplayStatics.h"
#include "DynamicCameraActor.h"
#include "CB_FigtherCharacter.h"

ADynamicCameraActor::ADynamicCameraActor()
{
	PrimaryActorTick.bCanEverTick = false;

	//스프링 암 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 1200.0f;

	//카메라 설정
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetProjectionMode(ECameraProjectionMode::Perspective);
}

void ADynamicCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ACB_FigtherCharacter의 모든 캐릭터 가져오기
	TArray<AActor*> Fighters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACB_FigtherCharacter::StaticClass(), Fighters);

	//캐릭터가 없으면 종료
	if (Fighters.Num() == 0) return;

	FVector Center = FVector::ZeroVector;
	float MaxDist = 0.0f;

	//모든 캐릭터의 중심 위치 계산
	for (AActor* Actor : Fighters)
	{
		Center += Actor->GetActorLocation();
	}
	Center /= Fighters.Num();

	//가장 멀리 떨어진 두 플레이어 간 거리 계산
	for (int32 i = 0; i < Fighters.Num(); ++i)
	{
		for (int32 j = i + 1; j < Fighters.Num(); ++j)
		{
			float Dist = FVector::Dist2D(Fighters[i]->GetActorLocation(), Fighters[j]->GetActorLocation());
			MaxDist = FMath::Max(MaxDist, Dist);
		}
	}

	//카메라 위치 이동
	FVector TargetLoc = FVector(Center.X, -800.0f, Center.Z + 300.0f); //고정된 거리와 높이
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetLoc, DeltaTime, 5.0f));

	//줌
	float DesiredLength = FMath::Clamp(MaxDist * 1.2f, 1000.0f, 2500.0f);
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, DesiredLength, DeltaTime, 5.0f);
}
//게임모드에 카메라 스폰 후 게임 스테이트에서 카메라 할당?