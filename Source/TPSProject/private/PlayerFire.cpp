// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"

#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "Camera/CameraComponent.h"

#include "Bullet.h"
#include "PlayerAnim.h"
#include "EnemyFSM.h"


UPlayerFire::UPlayerFire()
{
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/SniperGun/Rifle.Rifle'"));
	if (tempSound.Succeeded())
	{
		bulletSound = tempSound.Object;
	}
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	cameraComp = me->cameraComp;
	gunMeshComp = me->gunMeshComp;
	sniperGunComp = me->sniperGunComp;

	// 1. �������� ui ��������
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// 2. �Ϲ� ����ui ũ�ν���� �ν��Ͻ� ����
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	// 3. �Ϲ� ����ui ���
	_crosshairUI->AddToViewport();

	ChangeToSniperGun();
}

void UPlayerFire::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &UPlayerFire::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &UPlayerFire::ChangeToSniperGun);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &UPlayerFire::SniperAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &UPlayerFire::SniperAim);
}

void UPlayerFire::InputFire()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);

	// ī�޶� ����ũ ���
	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(cameraShake);

	// ���� �ִϸ��̼� ���
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();

	// ��ź �� ��� ��
	if (bUSingGrenadeGun)
	{
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	// �������� ��� �� 
	else
	{
		FVector startPos = cameraComp->GetComponentLocation();
		FVector endPos = cameraComp->GetComponentLocation() + cameraComp->GetForwardVector() * 5000;
		// linetrace�� �浹 ������ ���� ����
		FHitResult hitInfo;
		// �浹 �ɼ� ���� ����
		FCollisionQueryParams params;
		// �ڱ� �ڽ��� �浹���� ����
		params.AddIgnoredActor(me);
		// channel ���͸� �̿��� linetrace �浹 ����(�浹����, ����,���� ��ġ, ����ä��, �浹�ɼ�)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		if (bHit)
		{
			FTransform bulletTrans;
			// �ε��� ��ġ �Ҵ�
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			// �Ѿ� ���� ȿ�� �ν��Ͻ� ����
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactroy, bulletTrans);

			auto hitComp = hitInfo.GetComponent();
			// 1. ������Ʈ�� ���������� �Ǿ��ִٸ�
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				// 2. �������� ���� ������ �ʿ�
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass();
				// 3. �� �������� ����
				hitComp->AddForce(force);
			}
			// �ε��� ����� ������ �Ǵ�
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();
			}

		}
	}
}

void UPlayerFire::ChangeToGrenadeGun()
{
	if (bSniperAim != true)
	{
		bUSingGrenadeGun = true;
		sniperGunComp->SetVisibility(false);
		gunMeshComp->SetVisibility(true);
	}
}

void UPlayerFire::ChangeToSniperGun()
{
	bUSingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void UPlayerFire::SniperAim()
{
	// sniper mode �� �ƴϸ� ó�� ����
	if (bUSingGrenadeGun)
	{
		return;
	}
	// Pressed �Է� ó��
	if (bSniperAim == false)
	{
		bSniperAim = true;
		_sniperUI->AddToViewport();
		cameraComp->SetFieldOfView(45.0f);
		// 4. �Ϲ� ���� ui ����
		_crosshairUI->RemoveFromParent();
	}
	// Released �Է� ó��
	else
	{
		bSniperAim = false;
		_sniperUI->RemoveFromParent();
		cameraComp->SetFieldOfView(90.0f);
		// 4. �Ϲ� ���� ui ���
		_crosshairUI->AddToViewport();
	}
}

