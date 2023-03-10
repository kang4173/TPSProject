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

	// 월드에서 ATPSPlayer 타깃 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	// ATPSPlayer 타입으로 캐스팅
	target = Cast<ATPSPlayer>(actor);
	// 소유 객체 가져오기
	me = Cast<AEnemy>(GetOwner());

	// UEnemyAnim* 할당
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	// AAIController 할당
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
	// 1. 시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 경과시간이 대기시간을 초과했다면
	if (currentTime > idleDelayTime)
	{
		// 3. 이동상태로 전환
		mState = EEnemyState::Move;
		// 4. 경과시간 초기화
		currentTime = 0;
		// 애니메이션 상태 동기화
		anim->animState = mState;
		// 최초 랜덤한 위치 정해주기
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
	
}

void UEnemyFSM::MoveState()
{
	// 1. 타깃 목적지 
	FVector destination = target->GetActorLocation();
	// 2. 방향
	FVector dir = destination - me->GetActorLocation();
	// 3. 방향으로 이동하고 싶다
	//me->AddMovementInput(dir.GetSafeNormal());

	// NavigationSystem 객체 얻어오기
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	
	// 목적지 길 찾기 경로 데이터 검색
	FPathFindingQuery query;
	FAIMoveRequest req;
	
	// 목적지에서 인지할 수 있는 범위
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);

	// 길 찾기 위한 쿼리 생성
	ai->BuildPathfindingQuery(req, query);

	//길 찾기 결과가져오기
	FPathFindingResult r = ns->FindPathSync(query);

	// 목적지 까지 길찾기 성공 여부 확인
	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(destination);
	}
	else
	{
		// 랜덤 위치 이동
		auto result = ai->MoveToLocation(randomPos);
		// 목적지 도착 하면
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			// 새로운 랜덤위치 가져오기
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	}

	// 1. 거리가 공격 범위 안에 들어오면
	if (dir.Size() < attackRange)
	{
		// 길 찾기 기능 정지
		ai->StopMovement();
		// 2. 공격 상태로 전환
		mState = EEnemyState::Attack;
		// 애니메이션 상태 동기화
		anim->animState = mState;
		// 공격 애니메이션 재생 활성화
		anim->bAttackPlay = true;
		// 공격 상태 전환 시 대기시간이 바로 끝나도록 처리
		currentTime = attackDelayTime;
	}
}

void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		anim->bAttackPlay = true;
	}
	
	// 1. 타깃과의 거리
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	// 2. 타깃과의 거리가 범위를 벗어나면
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
		// 애니메이션 상태 동기화
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
		// 애니메이션 상태 동기화
		anim->animState = mState;
	}
}

void UEnemyFSM::DieState()
{
	// 아직 죽음 애니메이션이 끝나지 않았다면
	// 바닥내려가지 않도록 처리
	if (anim->bDieDone == false)
	{
		return;
	}

	//계속 아래로 내려가고 싶다.
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	if (P.Z > 150.0f)
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

		// 피격 애니메이션 재생
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), 0);
		anim->PlayDamageAnim(FName(*sectionName));
	}
	else
	{
		mState = EEnemyState::Die;
		// 캡슐 충돌체 비활성화
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 죽음 애니메이션 재생
		anim->PlayDamageAnim(TEXT("Die"));
	}
	// 애니메이션 상태 동기화
	anim->animState = mState;
	// 길 찾기 기능정지
	ai->StopMovement();
}

// 랜덤위치 가져오기
bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;

	return result;
}

