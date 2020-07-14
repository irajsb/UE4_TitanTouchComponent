// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "TouchObject.h"
#include "InputCoreTypes.h"
#include "UObject/NoExportTypes.h"
#include "UObject/ObjectMacros.h"

#include "TouchHandlerObject.generated.h"
/*Creates Multiple Execs for function*/
UENUM(BlueprintType)
enum  ETouchInputBranch 
{
    Press,
    Moved,
    Release,
};

/**
* 
*/

/*Holds TouchObjects in an array based on ZOrder and passes input to them */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOUCHHANDLER_API UTouchHandlerObject : public UActorComponent
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
TArray<FTouchSetup>ComponentsSetup;
    /* >0 if you need extra functionality set to 0 to disable */
    UPROPERTY(EditAnywhere)
    float TickRate ;
    UPROPERTY(BlueprintReadOnly)
    TArray<UTouchObject*>Components;

    virtual void InitializeComponent()override;
    UTouchHandlerObject();

    UFUNCTION(BlueprintCallable, Category = "Stuff", Meta = (ExpandEnumAsExecs = "Branches"))
 void PassInput(FVector Location , TEnumAsByte<ETouchInputBranch> Branches,ETouchIndex::Type FingerIndex );
    /* is "in" between center.x-size and center.x+size IE: is point in a square with center of center and edge length of size*/
    static bool IsVector2DInRange(FVector2D in, FVector2D Center,float Size);
    static bool IsVector2DInRange(FVector2D in, FVector2D Center,FVector2D Size);

    FTimerHandle JoystickTimer;
    void Timer();
};
