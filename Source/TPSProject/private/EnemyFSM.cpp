// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "TPSPlayer.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "TPSProject.h"
#include <Components/CapsuleComponent.h>
#include "EnemyAnim.h"
#include <AIController.h>
#include <NavigationSystem.h>


// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// ���忡�� ATPSPlayer Ÿ�� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	// ATPSPlayer Ÿ������ ĳ����
	target = Cast<ATPSPlayer>(actor);
	// ���� ��ü ��������
	me = Cast<AEnemy>(GetOwner());

	// UEnemyAnim* �Ҵ�
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	// AAIController �Ҵ�
	ai = Cast<AAIController>(me->GetController());

}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch(mState)
	{
		case EEnemyState::Idle:
			IdleState();   
			break;
		case EEnemyState::Move:
			MoveState();  
			break;
		case EEnemyState::Attack:
			AttackState(); 
			break;
		case EEnemyState::Damage:
			DamageState(); 
			break;
		case EEnemyState::Die:
			DieState();    
			break;
	}
}

void UEnemyFSM::IdleState()
{
	// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ����ð��� ���ð��� �ʰ��ߴٸ�
	if (currentTime > idleDelayTime)
	{
		// 3. �̵����·� ��ȯ
		mState = EEnemyState::Move;
		// 4. ����ð� �ʱ�ȭ
		currentTime = 0;
		// �ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
		// ���� ������ ��ġ �����ֱ�
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
	
}

void UEnemyFSM::MoveState()
{
	// 1. Ÿ�� ������ 
	FVector destination = target->GetActorLocation();
	// 2. ����
	FVector dir = destination - me->GetActorLocation();
	// 3. �������� �̵��ϰ� �ʹ�
	//me->AddMovementInput(dir.GetSafeNormal());

	// NavigationSystem ��ü ������
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	
	// ������ �� ã�� ��� ������ �˻�
	FPathFindingQuery query;
	FAIMoveRequest req;
	
	// ���������� ������ �� �ִ� ����
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);

	// �� ã�� ���� ���� ����
	ai->BuildPathfindingQuery(req, query);

	//�� ã�� �����������
	FPathFindingResult r = ns->FindPathSync(query);

	// ������ ���� ��ã�� ���� ���� Ȯ��
	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(destination);
	}
	else
	{
		// ���� ��ġ �̵�
		auto result = ai->MoveToLocation(randomPos);
		// ������ ���� �ϸ�
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			// ���ο� ������ġ ��������
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	}

	// 1. �Ÿ��� ���� ���� �ȿ� ������
	if (dir.Size() < attackRange)
	{
		// �� ã�� ��� ����
		ai->StopMovement();
		// 2. ���� ���·� ��ȯ
		mState = EEnemyState::Attack;
		// �ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
		// ���� �ִϸ��̼� ��� Ȱ��ȭ
		anim->bAttackPlay = true;
		// ���� ���� ��ȯ �� ���ð��� �ٷ� �������� ó��
		currentTime = attackDelayTime;
	}
}

void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	
	if (currentTime > attackDelayTime)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Black, TEXT("ATTACK!!!"));
		currentTime = 0;
		anim->bAttackPlay = true;
	}
	
	// 1. Ÿ����� �Ÿ�
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	// 2. Ÿ����� �Ÿ��� ������ �����
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
		// �ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
		
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > damageDelayTime)
	{
		mState = EEnemyState::Idle;

		currentTime = 0;
		// �ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
	}
}

void UEnemyFSM::DieState()
{
	// ���� ���� �ִϸ��̼��� ������ �ʾҴٸ�
	// �ٴڳ������� �ʵ��� ó��
	if (anim->bDieDone == false)
	{
		return;
	}

	//��� �Ʒ��� �������� �ʹ�.
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	if (P.Z > 200.0f)
	{
		me->Destroy();
	}
}

void UEnemyFSM::OnDamageProcess()
{
	hp--;
	if (hp > 0)
	{
		mState = EEnemyState::Damage;

		currentTime = 0;

		// �ǰ� �ִϸ��̼� ���
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), 0);
		anim->PlayDamageAnim(FName(*sectionName));
	}
	else
	{
		mState = EEnemyState::Die;
		// ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// ���� �ִϸ��̼� ���
		anim->PlayDamageAnim(TEXT("Die"));
	}
	// �ִϸ��̼� ���� ����ȭ
	anim->animState = mState;
	// �� ã�� �������
	ai->StopMovement();
}

// ������ġ ��������
bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;

	return result;
}

