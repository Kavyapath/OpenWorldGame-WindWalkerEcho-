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
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Weapons/Weapon1.h"
#include "Animation/AnimMontage.h"

// Sets default values
AWindWalkerCharacter::AWindWalkerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	OriginalSpeed = GetCharacterMovement()->MaxWalkSpeed;
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
	
	if (ActionState == EActionState::EAS_Attacking) {
		return;
	}
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


void AWindWalkerCharacter::Sprint()
{
	FVector Zero(0.f, 0.f, 0.f);
	if (GetCharacterMovement()->Velocity == Zero) {
		return;

	}
		IsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		CameraBoom->TargetArmLength = 400.f;
		AddControllerPitchInput(15.f);
		RunToStop = false;
}

void AWindWalkerCharacter::StopSprinting()
{
		IsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
		CameraBoom->TargetArmLength = 300.f;
		AddControllerPitchInput(0.f);
		RunToStop = true;
}

void AWindWalkerCharacter::Attack()
{
	
	if (CanOneHandedSwordAttack()) {
		
			PlayAttackMontageOneHandedSword();
			ActionState = EActionState::EAS_Attacking;
		

	}
	else if (CharacterState == ECharacterState::ECS_EquippedTwoHandedWeapon) {

	}
	else {
		//Character state is UnEquipped

	}
}

void AWindWalkerCharacter::PlayAttackMontageOneHandedSword()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage) {

		
		AnimInstance->Montage_Play(AttackMontage);
		int32 Selection = FMath::RandRange(0, 3);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		case 3:
			SectionName = FName("Attack4");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
		
	}
}
void AWindWalkerCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage) {

		AnimInstance->Montage_Play(EquipMontage);

		AnimInstance->Montage_JumpToSection(SectionName,EquipMontage);
	}
}

void AWindWalkerCharacter::OneWeaponCharacterState()
{
	if (EquippedWeapon) {
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
	
}

void AWindWalkerCharacter::DualWeaponCharacterState()
{
	if (EquippedWeapon && EquippedWeaponLeft) {
		CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
	}
}

bool AWindWalkerCharacter::CanOneHandedSwordAttack()
{
	return CharacterState == ECharacterState::ECS_EquippedOneHandedWeapon &&
			ActionState == EActionState::EAS_UnOccupied;
}

bool AWindWalkerCharacter::CanDisArm()
{
	return ActionState == EActionState::EAS_UnOccupied &&
		CharacterState != ECharacterState::ECS_UnEquipped ;
}

bool AWindWalkerCharacter::CanArm()
{
	return ActionState == EActionState::EAS_UnOccupied &&
		CharacterState == ECharacterState::ECS_UnEquipped &&
		EquippedWeapon;
}

void AWindWalkerCharacter::QKeyPressed()
{
	if (CanDisArm())
	{
		PlayEquipMontage(FName("UnEquip"));

		CharacterState = ECharacterState::ECS_UnEquipped;
	}
	
	else if (CanArm()) {
		PlayEquipMontage(FName("Equip"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		if (EquippedWeaponLeft != nullptr) {
			CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
		}
	}
}

void AWindWalkerCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_UnOccupied;
}

void AWindWalkerCharacter::DisArm()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(),FName("RightHandSword"));
	}
	if (EquippedWeaponLeft) {
		EquippedWeaponLeft->AttachMeshToSocket(GetMesh(),FName("LeftHandSword"));
	}
}

void AWindWalkerCharacter::Arm()
{

	if (CharacterState == ECharacterState::ECS_EquippedOneHandedWeapon) {
		if (EquippedWeapon) {
		
			EquippedWeapon->AttachMeshToSocket(GetMesh(),FName("RightHandSocket"));
		}
	}

	else if(CharacterState == ECharacterState::ECS_EquippedTwoHandedWeapon){
		if (EquippedWeapon) {
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
		}

		if (EquippedWeaponLeft) {
		
		    EquippedWeaponLeft->AttachMeshToSocket(GetMesh(), FName("LeftHandSocket"));
		}
	}

	
}



void AWindWalkerCharacter::EKeyPressed()
{
	//now cast the item in to the weapon
	AWeapon* Weapon = Cast<AWeapon>(OverlappingItem);
	if (Weapon) {

		Weapon->Equip(this->GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = Weapon;


	}
	if (EquippedWeapon) {
		AWeapon1* Weapon1 = Cast<AWeapon1>(OverlappingItem);
		if (Weapon1) {

			Weapon1->Equip(this->GetMesh(), FName("LeftHandSocket"));
			CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
			OverlappingItem = nullptr;
			EquippedWeaponLeft = Weapon1;
		}
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
		EnhancedInputComponent->BindAction(WeaponEquipAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(WeaponUnEquipAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::QKeyPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AWindWalkerCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AWindWalkerCharacter::StopSprinting);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::Attack);
		EnhancedInputComponent->BindAction(OneWeaponAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::OneWeaponCharacterState);
		EnhancedInputComponent->BindAction(DualWeaponAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::DualWeaponCharacterState);
	}


}

