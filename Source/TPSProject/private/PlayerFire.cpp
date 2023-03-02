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

	// 1. 스나이퍼 ui 위젯생성
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// 2. 일반 조준ui 크로스헤어 인스턴스 생성
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	// 3. 일반 조준ui 등록
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

	// 카메라 셰이크 재생
	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(cameraShake);

	// 공격 애니메이션 재생
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();

	// 유탄 총 사용 시
	if (bUSingGrenadeGun)
	{
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	// 스나이퍼 사용 시 
	else
	{
		FVector startPos = cameraComp->GetComponentLocation();
		FVector endPos = cameraComp->GetComponentLocation() + cameraComp->GetForwardVector() * 5000;
		// linetrace의 충돌 정보를 담을 변수
		FHitResult hitInfo;
		// 충돌 옵션 설정 변수
		FCollisionQueryParams params;
		// 자기 자신은 충돌에서 제외
		params.AddIgnoredActor(me);
		// channel 필터를 이용한 linetrace 충돌 검출(충돌정보, 시작,종료 위치, 검출채널, 충돌옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		if (bHit)
		{
			FTransform bulletTrans;
			// 부딪힌 위치 할당
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			// 총알 파편 효과 인스턴스 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactroy, bulletTrans);

			auto hitComp = hitInfo.GetComponent();
			// 1. 컴포넌트에 물리적용이 되어있다면
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				// 2. 날려버릴 힘과 방향이 필요
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass();
				// 3. 그 방향으로 날림
				hitComp->AddForce(force);
			}
			// 부딪힌 대상이 적인지 판단
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
	// sniper mode 가 아니면 처리 안함
	if (bUSingGrenadeGun)
	{
		return;
	}
	// Pressed 입력 처리
	if (bSniperAim == false)
	{
		bSniperAim = true;
		_sniperUI->AddToViewport();
		cameraComp->SetFieldOfView(45.0f);
		// 4. 일반 조준 ui 제거
		_crosshairUI->RemoveFromParent();
	}
	// Released 입력 처리
	else
	{
		bSniperAim = false;
		_sniperUI->RemoveFromParent();
		cameraComp->SetFieldOfView(90.0f);
		// 4. 일반 조준 ui 등록
		_crosshairUI->AddToViewport();
	}
}

