// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Canvas.h"
#include "GameFramework/HUD.h"
#include "UObject/ObjectMacros.h"

#include "TouchHUD.generated.h"

/**
* 
*/
UCLASS()
class TOUCHHANDLER_API ATouchHUD : public AHUD
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    class UTouchHandlerObject* TouchHandler;
    UPROPERTY(EditAnywhere)
    bool bJoystickDebugEnabled;
    ATouchHUD();
    virtual void DrawHUD()override;
    void DrawTextureCentered(UTexture2D* Texture,float x ,float y,float Width,float Height,FLinearColor Color);
    void  DrawDebugRectCentered(FLinearColor col,float x ,float y ,float w ,float h);
    UCanvas* GetCanvas()const;
    /*Res relation with 1280*720*/
    UPROPERTY()
	float ResRatio;
    UFUNCTION(BlueprintCallable)
    void DrawDebugs();
};
