// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class UUSerWidget;
class UParticleSystem;
class USoundBase;
class UPlayerBaseComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UInputComponent*);

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// �Է� ���ε� ��������Ʈ
	FInputBindingDelegate onInputBindingDelegate;

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* cameraComp;

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	UStaticMeshComponent* sniperGunComp;

public:
	UPROPERTY(VisibleAnywhere, Category = Component)
	UPlayerBaseComponent* playerMove;
	UPROPERTY(VisibleAnywhere, Category = Component)
	UPlayerBaseComponent* playerFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	int32 hp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	int32 initialHP = 10;
	
	// �ǰ� ������ �� ó��
	UFUNCTION(BlueprintCallable, Category = Health)
	void OnHitEvent();

	// ���� ������ �� ó�� �Լ�
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Health)
	void OnGameOver();

};
