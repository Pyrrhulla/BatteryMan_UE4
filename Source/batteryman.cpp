// Fill out your copyright notice in the Description page of Project Settings.


#include "batteryman.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
Abatteryman::Abatteryman()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(43.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamerBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bDead = false;
	Power = 100.0f;



}




// Called when the game starts or when spawned
void Abatteryman::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &Abatteryman::OnBeginOverlap);

	if (Player_Power_Widget_Class != nullptr) {
		Player_Power_Widget = CreateWidget(GetWorld(), Player_Power_Widget_Class);

		Player_Power_Widget->AddToViewport();
	}
	
}

// Called every frame
void Abatteryman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Power -= DeltaTime * Power_Treshold;

	if (Power <= 0) {
		
		if (!bDead) {

			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(
				UnusedHandle, this, &Abatteryman::RestartGame, 3.0f, false);

		}
	}

}

// Called to bind functionality to input
void Abatteryman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &Abatteryman::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Abatteryman::MoveRight);

}

void Abatteryman::MoveForward(float Axis)
{
	if (!bDead) {

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);

	}

}

void Abatteryman::MoveRight(float Axis)
{

	if (!bDead) {

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);

	}
}



void Abatteryman::RestartGame()
{

	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

}


void Abatteryman::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIntex, 
	bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag("Recharge")) {

		Power += 10.0f;

		if (Power > 100.0f)
			Power = 100.0f;

		OtherActor->Destroy();

	}
}


