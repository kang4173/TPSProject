// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

#include "Enemy.h"
#include <EngineUtils.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemyManager::AEnemyManager()
{
	// Tick 함수 매 프레임 호출되지 않도록 설정
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	// 1. 랜덤 생성 시간
	float createTime = FMath::RandRange(minTime, maxTime);
	// 2. TimerManaget 알람 등록
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);

	// 스폰 위치 동적 할당
	FindSpawnPoints();
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::CreateEnemy()
{
	// 랜덤 위치 구하기
	int index = FMath::RandRange(0, spawnPoints.Num() - 1);
	// 적 생성 및 배치
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnPoints[index]->GetActorLocation(), FRotator(0));

	float createTime = FMath::RandRange(minTime, maxTime);
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);

}

void AEnemyManager::FindSpawnPoints()
{
	// 검색으로 찾은 결과 저장할 배열
	TArray<AActor*> allActors;
	// 원하는 타입의 액터 모두찾기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	// 찾은 결과가 있을경우 반복
	for (auto spawn : allActors)
	{
		// 찾은 액터의 이름에 해당 문자열 포함한다면
		if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
		{
			spawnPoints.Add(spawn);
		}
	}

}

