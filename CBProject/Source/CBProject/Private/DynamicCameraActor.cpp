#include "Kismet/GameplayStatics.h"
#include "DynamicCameraActor.h"
#include "CB_FigtherCharacter.h"

ADynamicCameraActor::ADynamicCameraActor()
{
	PrimaryActorTick.bCanEverTick = false;

	//������ �� ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 1200.0f;

	//ī�޶� ����
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetProjectionMode(ECameraProjectionMode::Perspective);
}

void ADynamicCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ACB_FigtherCharacter�� ��� ĳ���� ��������
	TArray<AActor*> Fighters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACB_FigtherCharacter::StaticClass(), Fighters);

	//ĳ���Ͱ� ������ ����
	if (Fighters.Num() == 0) return;

	FVector Center = FVector::ZeroVector;
	float MaxDist = 0.0f;

	//��� ĳ������ �߽� ��ġ ���
	for (AActor* Actor : Fighters)
	{
		Center += Actor->GetActorLocation();
	}
	Center /= Fighters.Num();

	//���� �ָ� ������ �� �÷��̾� �� �Ÿ� ���
	for (int32 i = 0; i < Fighters.Num(); ++i)
	{
		for (int32 j = i + 1; j < Fighters.Num(); ++j)
		{
			float Dist = FVector::Dist2D(Fighters[i]->GetActorLocation(), Fighters[j]->GetActorLocation());
			MaxDist = FMath::Max(MaxDist, Dist);
		}
	}

	//ī�޶� ��ġ �̵�
	FVector TargetLoc = FVector(Center.X, -800.0f, Center.Z + 300.0f); //������ �Ÿ��� ����
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetLoc, DeltaTime, 5.0f));

	//��
	float DesiredLength = FMath::Clamp(MaxDist * 1.2f, 1000.0f, 2500.0f);
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, DesiredLength, DeltaTime, 5.0f);
}
//���Ӹ�忡 ī�޶� ���� �� ���� ������Ʈ���� ī�޶� �Ҵ�?