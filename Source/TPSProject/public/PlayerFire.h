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

	// 총알 파편효과 공장
	UPROPERTY(EditAnywhere, Category = BulletEffect)
	UParticleSystem* bulletEffectFactroy;

	// 일반 조준 크로스헤어ui 위젯
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	UUserWidget* _crosshairUI;

	// 카메라 셰이크 블루프린트 저장 변수
	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;

	// 총 발사 사운드 재생
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* bulletSound;



};
