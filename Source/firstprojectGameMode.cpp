// Copyright Epic Games, Inc. All Rights Reserved.

#include "firstprojectGameMode.h"
#include "firstprojectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AfirstprojectGameMode::AfirstprojectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
