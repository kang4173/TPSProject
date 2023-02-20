// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"

#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
    // 1. ���� �� ��� ����
    auto ownerPawn = TryGetPawnOwner();
    // 2. �÷��̾�� ĳ����
    auto player = Cast<ATPSPlayer>(ownerPawn);
    if (player)
    {
        // 3. �̵� �ӵ� �ʿ�
        FVector velocity = player->GetVelocity();
        // 4. �÷��̾� ���� ���� �ʿ�
        FVector forwardVector = player->GetActorForwardVector();
        // 5. speed�� ��(����) �Ҵ�
        speed = FVector::DotProduct(forwardVector, velocity);
        // 6. �¿� �ӵ� �Ҵ�
        FVector rightVector = player->GetActorRightVector();
        direction = FVector::DotProduct(rightVector, velocity);

        // �÷��̾ ���� ���߿� �ִ��� ���θ� ����ϰ� �ʹ�
        auto movement = player->GetCharacterMovement();
        inInAir = movement->IsFalling();
    }
}

void UPlayerAnim::PlayAttackAnim()
{
    Montage_Play(attackAnimMontage);
}
