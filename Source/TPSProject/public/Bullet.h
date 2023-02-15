// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class TPSPROJECT_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* movementComp;
	UPROPERTY(VisibleAnywhere, Category = Collision)
	USphereComponent* collisionComp;
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	UStaticMeshComponent* bodyMeshComp;

	void Die();

	UPROPERTY(EditAnywhere, Category = Settings)
	float speed = 5000;
	// 액터의 특정 속성을 수정하면 호출되는 이벤트 함수
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
