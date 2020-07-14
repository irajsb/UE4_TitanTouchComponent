// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchHandlerObject.h"

#include "Kismet/KismetMathLibrary.h"

UTouchHandlerObject::UTouchHandlerObject()
{
    bWantsInitializeComponent=true;
    
}

void UTouchHandlerObject::PassInput(FVector Location, TEnumAsByte<ETouchInputBranch> Branches,ETouchIndex::Type FingerIndex )
{
    
const FVector2D TouchLocation=FVector2D(Location.X,Location.Y);

for (uint8 Index=0;Index!=Components.Num();++Index)
{//IF input is press 
    if(Branches==ETouchInputBranch::Press)
    {//check if we need to pass input to what components
        if(IsVector2DInRange(TouchLocation,Components[Index]->CanvasLocation,Components[Index]->Data.FunctionalRadius*Components[Index]->HUD->ResRatio/2))
        {
            Components[Index]->HandlePress(TouchLocation);
            Components[Index]->ReservedIndex=FingerIndex;
            //if input consumed we break the loop
            if(Components[Index]->Data.bConsumeInput)
                break;
        }else if(Components[Index]->Data.Type==ETouchComponentType::Joystick&&IsVector2DInRange(TouchLocation,Components[Index]->SquareCenter*Components[Index]->HUD->ResRatio,Components[Index]->Data.SquareSize/2))
        {//if touch was outside joytstick interaction radius but was inside joystick interaction square we move joystick to touch location 

            Components[Index]->ReCenter(TouchLocation);
            Components[Index]->HandlePress(TouchLocation);
            Components[Index]->ReservedIndex=FingerIndex;
            if(Components[Index]->Data.bConsumeInput)
                break;
        } else   if(Components[Index]->Data.Type==ETouchComponentType::Swipe&&IsVector2DInRange(TouchLocation,Components[Index]->SquareCenter*Components[Index]->HUD->ResRatio,Components[Index]->Data.SquareSize/2))
        {//if swipe component we need to just check if its inside interaction square 
            Components[Index]->HandlePress(TouchLocation);
            Components[Index]->ReservedIndex=FingerIndex;
            if(Components[Index]->Data.bConsumeInput)
                break;
            
        }
        
    }
    //release and move just check if index was reserved by that finger index if it was pass it to that component
    else if(Branches==ETouchInputBranch::Release)
    {
       if( Components[Index]->ReservedIndex==FingerIndex)
       {
           Components[Index]->HandleRelease(TouchLocation);
       }
    }
    else
    {//on touch move  
        if( Components[Index]->ReservedIndex==FingerIndex)
        {
            Components[Index]->HandleMove(TouchLocation);
        }
    }
    
}



    
}

bool UTouchHandlerObject::IsVector2DInRange(FVector2D in, FVector2D Center, float Size)
{//for squares
if(Center.X-Size<in.X&&in.X<Center.X+Size)
{
    if(Center.Y-Size<in.Y&&in.Y<Center.Y+Size)
    {
        return true;
    }
}
    return false;
}
bool UTouchHandlerObject::IsVector2DInRange(FVector2D in, FVector2D Center, FVector2D Size)
{//for rectangles
    if(Center.X-Size.X<in.X&&in.X<Center.X+Size.X)
    {
        if(Center.Y-Size.Y<in.Y&&in.Y<Center.Y+Size.Y)
        {
            return true;
        }

       
    }
    return false;
}

void UTouchHandlerObject::Timer()
{//parent tick that calls each components tick function if registered   
    for (uint8 Index=0;Index!=Components.Num();++Index)
{
        if(Components[Index]->Data.bRegisterTick)
        Components[Index]->Tick();

}
    
}

void UTouchHandlerObject::InitializeComponent()
{//feed data from setup struct to Components 
ATouchHUD* OwnerHUD=Cast<ATouchHUD>(GetOwner());
    for(uint8 Index=0;Index!=ComponentsSetup.Num();++Index)
    {
       
        UTouchObject* TempObj=    NewObject<UTouchObject>(this);
        Components.Add(TempObj);
        TempObj->Data=ComponentsSetup[Index];
      TempObj->SquareCenter=TempObj->Data.Center;
        TempObj->HUD=OwnerHUD;
    }
    //Start Timer
    GetWorld()->GetTimerManager().SetTimer(JoystickTimer,this ,&UTouchHandlerObject::Timer,TickRate,true);
}
