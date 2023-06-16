// Fill out your copyright notice in the Description page of Project Settings.


#include "LovelyJson.h"

void FLovelyJsonModule::StartupModule ()
{
	IModuleInterface::StartupModule();
}

void FLovelyJsonModule::ShutdownModule ()
{
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_MODULE(FLovelyJsonModule, LovelyJson)