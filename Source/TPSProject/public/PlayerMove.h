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
	// 좌우 회전 입력 처리
	void Turn(float value);
	// 상하 회전 입력 처리
	void LookUp(float value);

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = PlayerSetting)
		float walkSpeed = 200;
	UPROPERTY(VisibleAnywhere, Category = PlayerSetting)
		float runSpeed = 600;
	FVector direction;

	// 좌우 이동 입력 처리
	void Horizontal(float value);
	// 상하 이동 입력 처리
	void Vertical(float value);

	void InputJump();
	void Move();
	void InputRun();


};