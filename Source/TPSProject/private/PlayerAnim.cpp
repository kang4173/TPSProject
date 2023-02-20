// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"

#include "TPSPlayer.h"

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
    }
}
