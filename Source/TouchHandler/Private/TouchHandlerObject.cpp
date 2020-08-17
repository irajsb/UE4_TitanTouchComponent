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
{//--Check if We are in range of square from center
  const bool IsInsideCenterSquare=IsVector2DInRange(TouchLocation,Components[Index]->SquareCenter*Components[Index]->HUD->ResRatio2D,Components[Index]->Data.SquareSize/2);
  const bool IsInsideThumbSquare=IsVector2DInRange(TouchLocation,Components[Index]->Data.Center*Components[Index]->HUD->ResRatio2D,Components[Index]->Data.SquareSize/2);
    //IF input is press
    if(Branches==ETouchInputBranch::Press)
    {//check if we need to pass input to what components
        if(Components[Index]->Data.Type==ETouchComponentType::Joystick&&(IsInsideThumbSquare||IsInsideCenterSquare))
        {//if touch was outside joystick interaction radius but was inside joystick interaction square we move joystick to touch location
            Components[Index]->ReservedIndex=FingerIndex;
if( !Components[Index]->Data.FixedJoystick && IsInsideCenterSquare)
            Components[Index]->ReCenter(TouchLocation);
            Components[Index]->HandlePress(TouchLocation);
           
            if(Components[Index]->Data.bConsumeInput)
                break;
        } else   if(Components[Index]->Data.Type!=ETouchComponentType::Joystick&&IsInsideThumbSquare)
        {//if swipe component we need to just check if its inside interaction square
            Components[Index]->ReservedIndex=FingerIndex;
            Components[Index]->HandlePress(TouchLocation);
           
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
{
    //feed data from setup struct to Components 
    AddTouchObjectsFromArray(ComponentsSetup,false);
    //Start Timer
    GetWorld()->GetTimerManager().SetTimer(JoystickTimer,this ,&UTouchHandlerObject::Timer,TickRate,true);
}
void UTouchHandlerObject::AddTouchObject(FTouchSetup input,ATouchHUD* HUD,TArray<UTouchObject*>& Array)
{   UTouchObject* TempObj=    NewObject<UTouchObject>(this);
    Array.Add(TempObj);
   
    TempObj->Data=input;
    TempObj->SquareCenter=TempObj->Data.Center;
    TempObj->HUD=HUD;
}

void UTouchHandlerObject::AddTouchObjectsFromArray(TArray<FTouchSetup>in,bool bInsertAtBegining)
{  TArray<UTouchObject*>TempObjects;
    ATouchHUD* OwnerHUD=Cast<ATouchHUD>(GetOwner());
    for(uint8 Index=0;Index!=in.Num();++Index)
    {
        AddTouchObject(in[Index],OwnerHUD,TempObjects);
     
    }
    if(bInsertAtBegining)
    {
        TempObjects.Append(Components);
        Components=TempObjects;
    }else
    {
        Components.Append(TempObjects);
    }
}
