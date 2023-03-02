// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 * 
 */

UCLASS()
class TPSPROJECT_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerMove();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// �¿� ȸ�� �Է� ó��
	void Turn(float value);
	// ���� ȸ�� �Է� ó��
	void LookUp(float value);

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = PlayerSetting)
		float walkSpeed = 200;
	UPROPERTY(VisibleAnywhere, Category = PlayerSetting)
		float runSpeed = 600;
	FVector direction;

	// �¿� �̵� �Է� ó��
	void Horizontal(float value);
	// ���� �̵� �Է� ó��
	void Vertical(float value);

	void InputJump();
	void Move();
	void InputRun();


};