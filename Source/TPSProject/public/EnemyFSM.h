// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

// 사용 할 상태 정의
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	
	// 상태변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState mState = EEnemyState::Idle;

	// 대기
	void IdleState();
	// 이동
	void MoveState();
	// 공격
	void AttackState();
	// 피격
	void DamageState();
	// 죽음
	void DieState();

	// 대기시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float idleDelayTime = 2;
	// 경과시간
	float currentTime = 0;

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	class ATPSPlayer* target;
	UPROPERTY()
	class AEnemy* me;

	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 150.0f;

	UPROPERTY(EditAnywhere, Category = FSM)
	float attackDelayTime = 2.0f;
	
	void OnDamageProcess();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	int32 hp = 3;
	UPROPERTY(EditAnywhere, Category = FSM)
	float damageDelayTime = 2.0f;
	// 아래로 사라지는 속도
	UPROPERTY(EditAnywhere, Category = FSM)
	float dieSpeed = 50.0f;
	// 사용중인 애니메이션 블루프린트
	UPROPERTY()
	class UEnemyAnim* anim;

	// Enemy를 소유하고있는 AIController
	UPROPERTY()
	class AAIController* ai;
	
	// 길 찾기 수행 시 랜덤 위치
	FVector randomPos;
	// 랜덤 위치 가져오기 (검색 할 기준 위치, 검색 범위, 랜덤위치 저장을 위한 참조타입 변수)
	UFUNCTION()
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

};
