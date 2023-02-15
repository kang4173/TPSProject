// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("BlcokAll"));
	collisionComp->SetSphereRadius(13);
	RootComponent = collisionComp;

	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	// MovementComp 가 갱신시킬 컴포넌트
	movementComp->SetUpdatedComponent(collisionComp);
	movementComp->InitialSpeed = 5000;
	movementComp->MaxSpeed = 5000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.3f;
	
	

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer, 
		FTimerDelegate::CreateLambda([this]()->void { Destroy(); }), 2.0f, false);
	// 람다 사용 
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Die()
{
	
}

void ABullet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// speed 값이 수정되었는지 확인
	if (PropertyChangedEvent.GetPropertyName() == TEXT("speed"))
	{
		// 변경 되었을경우 변경된 값을 적용
		movementComp->InitialSpeed = speed;
		movementComp->MaxSpeed = speed;
	}
}

