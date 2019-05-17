// Fill out your copyright notice in the Description page of Project Settings.

#include "LabyGameModeBase.h"
#include "LabyPlayerController.h"
#include "LabyPawn.h"

ALabyGameModeBase::ALabyGameModeBase() {
	PlayerControllerClass = ALabyPlayerController::StaticClass();
	DefaultPawnClass = ALabyPawn::StaticClass();
}
