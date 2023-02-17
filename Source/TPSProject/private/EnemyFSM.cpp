// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

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

	
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch(mState)
	{
			break;
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
	}
}

void UEnemyFSM::MoveState()
{
	
}

void UEnemyFSM::AttackState()
{

}

void UEnemyFSM::DamageState()
{
}

void UEnemyFSM::DieState()
{
}

