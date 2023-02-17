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
	// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ����ð��� ���ð��� �ʰ��ߴٸ�
	if (currentTime > idleDelayTime)
	{
		// 3. �̵����·� ��ȯ
		mState = EEnemyState::Move;
		// 4. ����ð� �ʱ�ȭ
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
