// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"


UPlayerMove::UPlayerMove()
{
	// Tick함수 호출처리
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	moveComp->MaxWalkSpeed = walkSpeed;
}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move();
}

void UPlayerMove::Turn(float value)
{
	me->AddControllerYawInput(value);
}

void UPlayerMove::LookUp(float value)
{
	me->AddControllerPitchInput(value);
}

void UPlayerMove::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMove::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMove::LookUp);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UPlayerMove::Horizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayerMove::Vertical);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UPlayerMove::InputJump);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayerMove::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerMove::InputRun);
}

void UPlayerMove::Horizontal(float value)
{
	direction.Y = value;
}

void UPlayerMove::Vertical(float value)
{
	direction.X = value;
}

void UPlayerMove::InputJump()
{
	me->Jump();
}

void UPlayerMove::Move()
{
	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void UPlayerMove::InputRun()
{
	auto movement = moveComp;
	// 현재 달리기 모드라면
	if (movement->MaxWalkSpeed > walkSpeed)
	{
		movement->MaxWalkSpeed = walkSpeed;
	}
	else
	{
		movement->MaxWalkSpeed = runSpeed;
	}
}




