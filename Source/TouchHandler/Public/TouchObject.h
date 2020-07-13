// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TouchHUD.h"
#include "Engine/Canvas.h"
#include "UObject/NoExportTypes.h"


#include "TouchObject.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPress,  FVector2D,Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRelease,  FVector2D,Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMove,  FVector2D,Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJoyStickAxis,  float,X,float,Y);


 UENUM(BlueprintType)
 enum ETouchComponentType {
 	Joystick     UMETA(DisplayName = "Joystick"),
 	Swipe      UMETA(DisplayName = "Swipe"),
 	Button   UMETA(DisplayName = "Button"),
 
 };


USTRUCT(BlueprintType)
struct FTouchSetup
{
	GENERATED_USTRUCT_BODY()

	/* lower array number higher input priority*/
	UPROPERTY(EditAnywhere)
        TEnumAsByte<ETouchComponentType> Type;
	UPROPERTY(EditAnywhere)
	bool bRegisterTick;
	/*Center Location in screen for this component Center Will Be screen size *Center */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D Center;
	/* Radial Offset from center which Component should handle as input(Joystick Background size) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float FunctionalRadius;
	/*Size Will Be screen size *Visual size*/ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D VisualSize;
	
	/*ForButtons and some joysticks that need a square */
	
	/*ForButtons and some joysticks that need a square */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D SquareSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UTexture2D* BackGround;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UTexture2D* InActiveBackGround;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
        UTexture2D* JoystickThumb;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
        UTexture2D* InActiveJoystickThumb;
	/*Max Distance Thumb Can Travel (percentage of screenSize)*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
	float ThumbClamp;
	/*Size of Distance between touch and joystick that will make joystick follow touch 0 for disable*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
    float  FollowTouchSize;
	/* should broadcast only on move or broadcast constantly p.s:constant requires tick enabled*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
    bool  BroadCastConstant;
	
	/*Pass input to lower priority components under this component or not*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bConsumeInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor ActiveColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor InActiveColor;
	
	FTouchSetup()
	{
InActiveColor=	ActiveColor=FLinearColor::White;
		bConsumeInput=true;
		ThumbClamp=65;
		FunctionalRadius=50;
		Center.X=Center.Y=150;
		VisualSize.X=VisualSize.Y=50;
	}
	
	
};
UCLASS()
class TOUCHHANDLER_API UTouchObject : public UObject
{
	GENERATED_BODY()
	public:
		/* Type of this Component*/
    		UPROPERTY(EditAnywhere)
    		TEnumAsByte<ETouchComponentType> Type;
	UPROPERTY(EditAnywhere)
    bool bRegisterTick;
	/*Will Be screen size /Center*/ 
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
    FVector2D VisualSize;
    	/*Center Location in screen for this component*/
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    	FVector2D Center;
    	/* Radial Offset from center which Component should handle as input(Joystick Background size) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    		float Radius;
	
	/*ForButtons and some joysticks that need a square */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
    FVector2D SquareSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* BackGround;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UTexture2D* InActiveBackGround;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
        UTexture2D* JoystickThumb;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
        UTexture2D* InActiveJoystickThumb;
	
	/*Max Distance Thumb Can Travel*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
    float ThumbClamp;
	/*Size of Distance between touch and joystick that will make joystick follow touch 0 for disable*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
	float  FollowTouchSize;
	/* should broadcast only on move or broadcast constantly p.s:constant requires tick enabled*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
    bool  BroadCastConstant;
    
	
	/*Pass input to lower priority components under this component or not*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bConsumeInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor ActiveColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor InActiveColor;
	
	//
	
	/*skip rendering this component*/
	UPROPERTY(BlueprintReadWrite)
	bool bHidden;
	/*xinput */
	UPROPERTY(BlueprintReadOnly)
	float XInput;
	/*Yinput*/
	UPROPERTY(BlueprintReadOnly)
	float YInput;
	UPROPERTY(BlueprintReadOnly)
	FVector2D CanvasLocation;
	UPROPERTY(BlueprintReadOnly)
	bool bIsPressed=false;
	//null finger  index is 255 
uint8 ReservedIndex=255;
	UFUNCTION()
	void HandlePress(FVector2D Location);
	void HandleRelease(FVector2D Location);
	void HandleMove(FVector2D Location);

	/*Events*/
	UPROPERTY(BlueprintAssignable)
	FOnPress OnPress;
	UPROPERTY(BlueprintAssignable)
	FOnMove OnMove;
	UPROPERTY(BlueprintAssignable)
	FOnRelease OnRelease;
	UPROPERTY(BlueprintAssignable)
	FJoyStickAxis JoyStickAxis;
	void ReCenter(FVector2D Location);
	void DrawTouchObject();
	void DrawJoystick();
	void DrawButton();
	void DrawSwipe();
	void Tick();
	UPROPERTY()
	ATouchHUD* HUD;
	UPROPERTY()
	UCanvas*Canvas;
	FVector2D SquareCenter;
	FVector2D TouchLocation;
};
