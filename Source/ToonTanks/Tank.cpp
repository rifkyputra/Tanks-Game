// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Rotator.h"
#include "DrawDebugHelpers.h"

ATank::ATank()
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
    SpringArmComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
    Super::BeginPlay();

    PlayerControllerRef = Cast<APlayerController>(GetController());

    DrawDebugSphere(GetWorld(), GetActorLocation() + FVector(0.f, 0.f, 200.f), 100.f, 12, FColor::Blue, true, 20.f);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    

    if (PlayerControllerRef)
    {
        FHitResult HitResult;
        PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);


        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 60.f, 12, FColor::Blue, false, -1.f);
        float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

        UE_LOG(LogTemp, Display, TEXT("1. %f"), TurretMesh->GetComponentRotation().Yaw);
        UE_LOG(LogTemp, Display, TEXT("2. %f"), HitResult.ImpactPoint.Rotation().Yaw);

        RotateTurret(HitResult.ImpactPoint);


    }


}

void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
}

void ATank::Move(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector;

    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

    DeltaLocation.X = Value * DeltaTime * MoveSpeed;

    AddActorLocalOffset(DeltaLocation);
}

void ATank::Turn(float Value)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;

    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

    DeltaRotation.Yaw = DeltaTime * Value * TurnRate;

    AddActorLocalRotation(DeltaRotation);
}

void ATank::MouseX(float Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Rotating Turret : %f"), Value);
}