// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WindWalkerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"

// Sets default values
AWindWalkerCharacter::AWindWalkerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = TEXT("head");

	EyeBrow = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrows"));
	EyeBrow->SetupAttachment(GetMesh());
	EyeBrow->AttachmentName = TEXT("head");


}

/*void AWindWalkerCharacter::Jump()
{
}*/

// Called when the game starts or when spawned
void AWindWalkerCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(WindWalkerContext, 0);
		}


	}


}

void AWindWalkerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	//controller(Mouse) can only rotate in XY axis remenber that  (Rotation along X Axis) is Controller is at (0,0,0) and Rotate by 45 Degree in XY plane now Controller Rotation becomes (45,45,0)
   //FRotationMatrix(YawRotation) Simple means Rotation Maxtrix formula Rotation Along ZAxis(YawRotation) Go And check the formula(According to unreal engine gizmo the XAxis Points to the forward direction)
	const FRotator ControllerDirection = GetControlRotation();
	const FRotator YawRotation(0.f, ControllerDirection.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//we wsant to find the forward direction when we will press w Xcos(theta)
	AddMovementInput(ForwardDirection, MovementVector.Y);

	//Finding Which way is right
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //controller(Mouse) can only rotate in XY axis remenber that  (Rotation along X Axis) is Controller is at (0,0,0) and Rotate by 45 Degree in XY plane now Controller Rotation becomes (45,45,0)
	//FRotationMatrix(YawRotation) Simple means Rotation Maxtrix formula Rotation Along ZAxis(YawRotation) Go And check the formula
	AddMovementInput(RightDirection, MovementVector.X);
}

void AWindWalkerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D AxisValue = Value.Get<FVector2D>();
	if (GetController()) {
		AddControllerPitchInput(AxisValue.Y * RotationRate * GetWorld()->GetDeltaSeconds());
		AddControllerYawInput(AxisValue.X * RotationRate * GetWorld()->GetDeltaSeconds());
	}
}

// Called every frame
void AWindWalkerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWindWalkerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);//for now bind it to charcter class jump function
	}


}

