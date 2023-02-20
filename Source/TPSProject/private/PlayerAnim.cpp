// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"

#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
    // 1. 소유 폰 얻어 오기
    auto ownerPawn = TryGetPawnOwner();
    // 2. 플레이어로 캐스팅
    auto player = Cast<ATPSPlayer>(ownerPawn);
    if (player)
    {
        // 3. 이동 속도 필요
        FVector velocity = player->GetVelocity();
        // 4. 플레이어 전방 벡터 필요
        FVector forwardVector = player->GetActorForwardVector();
        // 5. speed에 값(내적) 할당
        speed = FVector::DotProduct(forwardVector, velocity);
        // 6. 좌우 속도 할당
        FVector rightVector = player->GetActorRightVector();
        direction = FVector::DotProduct(rightVector, velocity);

        // 플레이어가 현재 공중에 있는지 여부를 기억하고 싶다
        auto movement = player->GetCharacterMovement();
        inInAir = movement->IsFalling();
    }
}

void UPlayerAnim::PlayAttackAnim()
{
    Montage_Play(attackAnimMontage);
}
