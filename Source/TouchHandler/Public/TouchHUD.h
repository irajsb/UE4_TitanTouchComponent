// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/Canvas.h"
#include "GameFramework/HUD.h"
#include "GameFramework/SaveGame.h"

#include "TouchHUD.generated.h"

/**
* 
*/


UCLASS()
class TOUCHHANDLER_API ATouchHUD : public AHUD
{
    GENERATED_BODY()
public:
    /* Parent of Components*/
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    class UTouchHandlerObject* TouchHandler;
 
    ATouchHUD();
    virtual void DrawHUD()override;
    void DrawTextureCentered(UTexture2D* Texture,float x ,float y,float Width,float Height,FLinearColor Color);
    void DrawTextureCentered(UTexture2D* Texture,float x ,float y,float Width,float Height,FLinearColor Color,float Rotation);
    void  DrawDebugRectCentered(FLinearColor col,float x ,float y ,float w ,float h);
    void DrawTextScaled(FString Text,float x ,float y ,UFont* Font, float TextPositionCorrection,FLinearColor Color);
    UCanvas* GetCanvas()const;
    /*Res relation relative to 1280*720*/
  
    float ResRatioX;
    float ResRatioY;
    FVector2D ResRatio2D;
    /* Call this in Blueprint implementable function on recive draw HUD*/
    UFUNCTION(BlueprintCallable)
    void DrawDebugs();
    UFUNCTION(BlueprintImplementableEvent)
    void CreateDefaultTouchHud();
    UFUNCTION(BlueprintImplementableEvent)
   void LoadTouchHudFromSave(USaveGame* in);
};
