#pragma once


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_UnEquipped UMETA(DisplayName = "UnEquipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")

};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_UnOccupied UMETA(DisplayName = "UnOccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking")
};

