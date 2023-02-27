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

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

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

	// �¿� ȸ�� �Է� ó��
	void Turn(float value);
	// ���� ȸ�� �Է� ó��
	void LookUp(float value);

	UPROPERTY(VisibleAnywhere, Category = PlayerSetting)
	float walkSpeed = 200;
	UPROPERTY(VisibleAnywhere, Category = PlayerSetting)
	float runSpeed = 600;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector direction;

	// �¿� �̵� �Է� ó��
	void Horizontal(float value);
	// ���� �̵� �Է� ó��
	void Vertical(float value);

	void InputJump();
	void Move();

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	USkeletalMeshComponent* gunMeshComp;
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	void InputFire();

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	UStaticMeshComponent* sniperGunComp;

	bool bUSingGrenadeGun = true;
	void ChangeToGrenadeGun();
	void ChangeToSniperGun();

	void SniperAim();
	bool bSniperAim = false;
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> sniperUIFactory;
	
	UUserWidget* _sniperUI;

	// �Ѿ� ����ȿ�� ����
	UPROPERTY(EditAnywhere, Category = BulletEffect)
	UParticleSystem* bulletEffectFactroy;

	// �Ϲ� ���� ũ�ν����ui ����
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	UUserWidget* _crosshairUI;

	// �޸��� �̺�Ʈ ó�� �Լ�
	void InputRun();

	// ī�޶� ����ũ �������Ʈ ���� ����
	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;

};
