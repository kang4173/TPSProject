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


};
