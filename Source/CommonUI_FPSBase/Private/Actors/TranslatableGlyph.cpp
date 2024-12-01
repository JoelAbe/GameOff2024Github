// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TranslatableGlyph.h"

ATranslatableGlyph::ATranslatableGlyph()
{
	isTranslated = false;
	Index = -1;
}

void ATranslatableGlyph::SetStateFromSaveData(const FWorldSave& WorldSaveData)
{
	if (Index != -1)
	{
		if (WorldSaveData.GlyphsArray.Num() > 0)
		{
			isTranslated = WorldSaveData.GlyphsArray[Index];
		}
	}
}
