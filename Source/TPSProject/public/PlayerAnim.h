// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float speed = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float direction = 0;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 플레이어가 공중에 있는지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	bool inInAir = false;

	// 재생할 공격 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
	class UAnimMontage* attackAnimMontage;
	// 공격 애니메이션 재생 함수
	void PlayAttackAnim();

	// 피격 애니메이션 재생 함수
	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
	void PlayDamageAnim(FName sectionName);

};
