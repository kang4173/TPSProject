// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (PlayerComponent), meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerFire();
	
	virtual void BeginPlay() override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY()
	class UCameraComponent* cameraComp;
	UPROPERTY()
	class USkeletalMeshComponent* gunMeshComp;
	UPROPERTY()
	class UStaticMeshComponent* sniperGunComp;

public:
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	void InputFire();

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

	// ī�޶� ����ũ �������Ʈ ���� ����
	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;

	// �� �߻� ���� ���
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* bulletSound;



};
