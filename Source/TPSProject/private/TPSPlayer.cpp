// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Blueprint/UserWidget.h>

#include "Bullet.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> BornMesh(TEXT
	("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (BornMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(BornMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);
	cameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	JumpMaxCount = 2;

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempGunMesh(TEXT
	("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}

	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempSniperMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	if (TempSniperMesh.Succeeded())
	{
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	ChangeToSniperGun();
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Move();
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::Horizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::Vertical);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATPSPlayer::InputJump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::InputFire);
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &ATPSPlayer::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &ATPSPlayer::ChangeToSniperGun);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATPSPlayer::SniperAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATPSPlayer::SniperAim);
}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value);
}

void ATPSPlayer::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::Horizontal(float value)
{
	direction.Y = value;
}

void ATPSPlayer::Vertical(float value)
{
	direction.X = value;
}

void ATPSPlayer::InputJump()
{
	Jump();
}

void ATPSPlayer::Move()
{
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	// 컨트롤러의 방향으로 이동 하므로 밑의 설정은 스킵됨.
//	FVector P0 = GetActorLocation();
//	FVector vt = direction * walkSpeed * DeltaTime;
//	FVector P = P0 + vt;
//	SetActorLocation(P);
	// zeroVecor 값이 000 인지 확인 해볼 것
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void ATPSPlayer::InputFire()
{
	FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
}

void ATPSPlayer::ChangeToGrenadeGun()
{
	if (bSniperAim != true)
	{
		bUSingGrenadeGun = true;
		sniperGunComp->SetVisibility(false);
		gunMeshComp->SetVisibility(true);
	}
}

void ATPSPlayer::ChangeToSniperGun()
{
	bUSingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void ATPSPlayer::SniperAim()
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
	}
	// Released 입력 처리
	else 
	{
		bSniperAim = false;
		_sniperUI->RemoveFromParent();
		cameraComp->SetFieldOfView(90.0f);
	}
}

