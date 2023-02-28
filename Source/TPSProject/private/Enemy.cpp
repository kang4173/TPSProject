// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyFSM.h"
#include "Animation/AnimInstance.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT
	("SkeletalMesh'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(0.84f));
	}
	
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
	
	// 애니메이션 블루프린트 할당
	// (ABP_Enemy 뒤에 _C 를 붙어야 블루프린트 클래스로 인식하기 때문에 _C 가 없으면 읽기 오류가 발생)
	ConstructorHelpers::FClassFinder<UAnimInstance>tempClass(TEXT("AnimBlueprint'/Game/Blueprints/ABP_Enemy.ABP_Enemy_C'"));
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}
	
	// 월드에 배치 or 스폰 될 때 자동으로 AIController 부터 Possess 될 수 있도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

