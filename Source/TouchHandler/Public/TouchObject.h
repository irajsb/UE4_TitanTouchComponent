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

/* Implement Data in setup Struct */
USTRUCT(BlueprintType)
struct FTouchSetup
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The main input to send from this control (for sticks, this is the horizontal axis)"))
	FKey MainInputKey;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The alternate input to send from this control (for sticks, this is the vertical axis)"))
	FKey AltInputKey;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="For Handling press and release"))
	FKey PressInputKey;
	/* lower array number higher input priority*/
	UPROPERTY(EditAnywhere)
        TEnumAsByte<ETouchComponentType> Type;
	
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
	/*Main texture for each component*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UTexture2D* BackGround;
	/*Main texture for each component*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UTexture2D* InActiveBackGround;
	/*Thumb Texture for joysticks only*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
        UTexture2D* JoystickThumb;
	/*Thumb Texture for joysticks only*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
        UTexture2D* InActiveJoystickThumb;
	/*Max Distance Thumb Can Travel (percentage of screenSize)*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
	float ThumbClamp;
	/*Size of Distance between touch and joystick that will make joystick follow touch 0 for disable*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "Type==ETouchComponentType::Joystick"))
    float  FollowTouchSize;
	/*Should Call BoradCastEvents*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool  BroadCast;
	/* should tick just update visuals or pass input too ! p.s:constant requires tick enabled*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
    bool  UpdateInputInTick;
	
	UPROPERTY(EditAnywhere)
    bool bRegisterTick;
	UPROPERTY(EditAnywhere)
    float DynamicJoystickSpeed;
	
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
		DynamicJoystickSpeed=0.01;
	}
	
	
};
UCLASS()
class TOUCHHANDLER_API UTouchObject : public UObject
{
	GENERATED_BODY()
	public:
	FTouchSetup Data;
	
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
	FVector2D SwipeAmount;
	/*Checks if location of swipe has changed*/
	bool LocationChanged;
void PassInputToKeyAxis(float x,FKey Key);
	FVector2D TouchLocation;
};
