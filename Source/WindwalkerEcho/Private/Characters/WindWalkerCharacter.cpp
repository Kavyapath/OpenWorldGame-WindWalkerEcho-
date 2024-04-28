// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WindWalkerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HUD/WindWalkerHUD.h"
#include "HUD/WindWalkerOverlay.h"
#include "Components/AttributeComponent.h"
#include "Items/Soul.h"
#include "Items/Treasure/Treasure.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WindwalkerEcho/DebugMacros.h"

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

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.f;


	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Effect"));
	Niagara->SetupAttachment(ViewCamera);

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

float AWindWalkerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if (ActionState == EActionState::EAS_Blocking) { 
		HandleDamage(0.f);
		SetHUDHealth();
	return 0.f;
	}
	else {
		HandleDamage(DamageAmount);
		SetHUDHealth();
		return DamageAmount;
	}
}

void AWindWalkerCharacter::SetHUDHealth()
{
	if (WindWalkerOverlay && Attribute) {
		WindWalkerOverlay->SetHealthBarPercent(Attribute->GetHealthPercent());
	}
}

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
	InitializeOverlay(PlayerController);
	Tags.Add(FName("WindWalker"));

	
	Niagara->Deactivate();

}

void AWindWalkerCharacter::InitializeOverlay(APlayerController* PlayerController)
{
	if (PlayerController) {
		AWindWalkerHUD* WindWalkerHUD = Cast<AWindWalkerHUD>(PlayerController->GetHUD());//it return the HUD type pointer and we will cast it in to its child class
		WindWalkerOverlay = WindWalkerHUD->GetWindWalkerOverlay();
		if (WindWalkerOverlay && Attribute)
		{
			WindWalkerOverlay->SetHealthBarPercent(Attribute->GetHealthPercent());
			WindWalkerOverlay->SetStaminaBarPercent(1.f);
			WindWalkerOverlay->SetGold(0.f);
			WindWalkerOverlay->SetSoul(0.f);
		}
	}
}

int32 AWindWalkerCharacter::PlayAttackMontageTwoHandedSword()
{
	return PlayRandomMontageSections(DualBladeMontage, DualAttackMontageSections);
}

void AWindWalkerCharacter::Move(const FInputActionValue& Value)
{
	DefaultCameraSetting();
	
	if (ActionState != EActionState::EAS_UnOccupied) {
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

		Niagara->Activate();
		IsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		CustomCameraSetting(450.f,1.f);
		RunToStop = false;
}

void AWindWalkerCharacter::StopSprinting()
{
		Niagara->Deactivate();
		IsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		DefaultCameraSetting();
		RunToStop = true;
}

void AWindWalkerCharacter::StartBlock()
{
	if (!IsAlive()) { return; }
	ActionState = EActionState::EAS_Blocking;
	IsBlocking = true;
	BlockToStop = false;
	
}

void AWindWalkerCharacter::EndBlock()
{
	if (!IsAlive()) { return; }
	ActionState = EActionState::EAS_UnOccupied;
	IsBlocking = false;
	BlockToStop = true;
	
}

void AWindWalkerCharacter::Attack()
{
	Super::Attack();
	if (!IsAlive()) { return; }
	if (CanOneHandedSwordAttack()) {
		
			PlayAttackMontage();
			ActionState = EActionState::EAS_Attacking;
		

	}
	
	
}

void AWindWalkerCharacter::DualBladeAttack()
{
	if (!IsAlive()) { return; }
	Super::Attack();
	if (CanTwoHandedSwordAttack()) {
		PlayAttackMontageTwoHandedSword();
		ActionState = EActionState::EAS_Attacking;
	}
}

void AWindWalkerCharacter::DashDodgeFront()
{
	if (!IsAlive()) { return; }
	ActionState = EActionState::EAS_Dodging;
	PlayMontageSection(DashDodgeMontage, FName("DodgeFront"));
}

void AWindWalkerCharacter::DashDodgeBack()
{
	if (!IsAlive()) { return; }
	ActionState = EActionState::EAS_Dodging;
	PlayMontageSection(DashDodgeMontage, FName("DodgeBack"));
}

void AWindWalkerCharacter::DashDodgeLeft()
{
	if (!IsAlive()) { return; }
	ActionState = EActionState::EAS_Dodging;
	PlayMontageSection(DashDodgeMontage, FName("DodgeLeft"));
}

void AWindWalkerCharacter::DashDodgeRight()
{
	if (!IsAlive()) { return; }
	ActionState = EActionState::EAS_Dodging;
	PlayMontageSection(DashDodgeMontage, FName("DodgeRight"));
}

void AWindWalkerCharacter::Die()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;
	DisAbleMeshCollision();
}

void AWindWalkerCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage) {

		AnimInstance->Montage_Play(EquipMontage);

		AnimInstance->Montage_JumpToSection(SectionName,EquipMontage);
	}
}

void AWindWalkerCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(this->GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void AWindWalkerCharacter::EquipWeaponLeft(AWeapon* Weapon)
{

	Weapon->Equip(this->GetMesh(), FName("LeftHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeaponLeft = Weapon;
}



bool AWindWalkerCharacter::CanOneHandedSwordAttack()
{
	return (CharacterState == ECharacterState::ECS_EquippedOneHandedWeapon || CharacterState == ECharacterState::ECS_EquippedTwoHandedWeapon) &&
			ActionState == EActionState::EAS_UnOccupied;
}

bool AWindWalkerCharacter::CanTwoHandedSwordAttack()
{
	return CharacterState == ECharacterState::ECS_EquippedTwoHandedWeapon &&
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
		DisArm();
	}
	
	else if (CanArm()) {
		Arm();
	}
}

void AWindWalkerCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	if (EquippedWeaponLeft != nullptr) {
		CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
	}
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AWindWalkerCharacter::DisArm()
{
	PlayEquipMontage(FName("UnEquip"));

	CharacterState = ECharacterState::ECS_UnEquipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AWindWalkerCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_UnOccupied;
}

void AWindWalkerCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_UnOccupied;
}

void AWindWalkerCharacter::DashDodgeEnd()
{
	ActionState = EActionState::EAS_UnOccupied;
}





void AWindWalkerCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(),FName("RightHandSword"));
	}
	if (EquippedWeaponLeft) {
		EquippedWeaponLeft->AttachMeshToSocket(GetMesh(),FName("LeftHandSword"));
	}
}

void AWindWalkerCharacter::AttachWeaponToHand()
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

void AWindWalkerCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_UnOccupied;
}

void AWindWalkerCharacter::CustomCameraSetting(float SpringArmLength, float Pitch)
{
	CameraBoom->TargetArmLength = SpringArmLength;
	AddControllerPitchInput(Pitch);
}

void AWindWalkerCharacter::DefaultCameraSetting()
{
	CameraBoom->TargetArmLength = 400.f;
	AddControllerPitchInput(0.f);
}

void AWindWalkerCharacter::CustomCameraSettingForAnimations()
{
	CustomCameraSetting(400.f,0.f);
}

void AWindWalkerCharacter::CustomCameraSettingForDualBladeAnimations()
{
	
	CustomCameraSetting(600.f,1.f);
}










void AWindWalkerCharacter::EKeyPressed()
{
	//now cast the item in to the weapon
	AWeapon* Weapon = Cast<AWeapon>(OverlappingItem);
	if (Weapon && EquippedWeapon==nullptr) {

		EquipWeapon(Weapon);


	}
	else if (EquippedWeapon) {
		AWeapon* Weapon1 = Cast<AWeapon>(OverlappingItem);
		if (Weapon1) {
			EquipWeaponLeft(Weapon1);
		}
	}
	
}



// Called every frame
void AWindWalkerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FHitResult OutHitResult;
	SphereTrace(OutHitResult);
	AActor* HitActor = OutHitResult.GetActor();
	if (HitActor && (HitActor->ActorHasTag("Enemy") || HitActor->ActorHasTag("Breakables") )) {
		CombatTarget = HitActor;
	
	}



}

// Called to bind functionality to input
void AWindWalkerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::Jump);//for now bind it to charcter class jump function
		EnhancedInputComponent->BindAction(WeaponEquipAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(WeaponUnEquipAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::QKeyPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AWindWalkerCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AWindWalkerCharacter::StopSprinting);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::Attack);
		EnhancedInputComponent->BindAction(DualAttackAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::DualBladeAttack);
		EnhancedInputComponent->BindAction(DashDodgeFrontAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::DashDodgeFront);
		EnhancedInputComponent->BindAction(DashDodgeBackAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::DashDodgeBack);
		EnhancedInputComponent->BindAction(DashDodgeLeftAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::DashDodgeLeft);
		EnhancedInputComponent->BindAction(DashDodgeRightAction, ETriggerEvent::Triggered, this, &AWindWalkerCharacter::DashDodgeRight);
		EnhancedInputComponent->BindAction(BlockStartAction, ETriggerEvent::Started, this, &AWindWalkerCharacter::StartBlock);
		EnhancedInputComponent->BindAction(BlockStartAction, ETriggerEvent::Completed, this, &AWindWalkerCharacter::EndBlock);
	}


}

void AWindWalkerCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint,Hitter);
	if (ActionState == EActionState::EAS_Blocking) { 
		return ; }
	else {
		SetWeaponcollisionEnabled(ECollisionEnabled::NoCollision);
		if (Attribute && Attribute->GetHealthPercent() > 0.f) {
			ActionState = EActionState::EAS_HitReaction;

		}
	}
}

void AWindWalkerCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;

}

void AWindWalkerCharacter::AddSouls(ASoul* Soul)
{
	if (Attribute && WindWalkerOverlay) {
		Attribute->AddSouls(Soul->GetSouls());
		WindWalkerOverlay->SetSoul(Attribute->GetSoul());
	}
}

void AWindWalkerCharacter::AddGold(ATreasure* Treasure)
{
	if (Attribute && WindWalkerOverlay) {
		Attribute->AddGold(Treasure->GetGold());
		WindWalkerOverlay->SetGold(Attribute->GetGold());
	}
}

void AWindWalkerCharacter::Jump()
{
	if (IsUnOccupied()) {
		Super::Jump();
	}
}

bool AWindWalkerCharacter::IsUnOccupied()
{
	return ActionState == EActionState::EAS_UnOccupied;
}

void AWindWalkerCharacter::SphereTrace(FHitResult& OutHitResult)
{

	FVector Location = GetActorLocation();
	Location += FVector(0.f, 0.f, 45.f);
	
	FVector ControllerForwardVector = ViewCamera->GetForwardVector();

		//GetActorForwardVector();
	ControllerForwardVector *= 1500.f;
	FVector EndTraceLocation = Location + ControllerForwardVector;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	 UKismetSystemLibrary::SphereTraceSingle(
		this,
		Location,
		EndTraceLocation,
		40.f,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		OutHitResult,
		true

	);
	
	
	
}

