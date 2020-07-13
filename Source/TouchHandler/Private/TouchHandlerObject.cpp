// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchHandlerObject.h"

#include "Kismet/KismetMathLibrary.h"

UTouchHandlerObject::UTouchHandlerObject()
{bWantsInitializeComponent=true;
    
}

void UTouchHandlerObject::PassInput(FVector Location, TEnumAsByte<ETouchInputBranch> Branches,ETouchIndex::Type FingerIndex )
{
    
const FVector2D TouchLocation=FVector2D(Location.X,Location.Y);

for (uint8 Index=0;Index!=Components.Num();++Index)
{
    if(Branches==ETouchInputBranch::Press)
    {
        if(IsVector2DInRange(TouchLocation,Components[Index]->CanvasLocation,Components[Index]->Radius*Components[Index]->HUD->ResRatio/2))
        {
            Components[Index]->HandlePress(TouchLocation);
            Components[Index]->ReservedIndex=FingerIndex;
            if(Components[Index]->bConsumeInput)
                break;
        }else if(Components[Index]->Type==ETouchComponentType::Joystick&&IsVector2DInRange(TouchLocation,Components[Index]->SquareCenter*Components[Index]->HUD->ResRatio,Components[Index]->SquareSize/2))
        {
          Components[Index]->ReCenter(TouchLocation);
            Components[Index]->HandlePress(TouchLocation);
            Components[Index]->ReservedIndex=FingerIndex;
            if(Components[Index]->bConsumeInput)
                break;
        } else   if(Components[Index]->Type==ETouchComponentType::Swipe&&IsVector2DInRange(TouchLocation,Components[Index]->SquareCenter*Components[Index]->HUD->ResRatio,Components[Index]->SquareSize/2))
        {
            Components[Index]->HandlePress(TouchLocation);
            Components[Index]->ReservedIndex=FingerIndex;
            if(Components[Index]->bConsumeInput)
                break;
            
        }
        
    }
    else if(Branches==ETouchInputBranch::Release)
    {
       if( Components[Index]->ReservedIndex==FingerIndex)
       {
           Components[Index]->HandleRelease(TouchLocation);
       }
    }
    else
    {//move 
        if( Components[Index]->ReservedIndex==FingerIndex)
        {
            Components[Index]->HandleMove(TouchLocation);
        }
    }
    
}



    
}

bool UTouchHandlerObject::IsVector2DInRange(FVector2D in, FVector2D Center, float Size)
{
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
{
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
{  
    for (uint8 Index=0;Index!=Components.Num();++Index)
{
        if(Components[Index]->bRegisterTick)
        Components[Index]->Tick();

}
    
}

void UTouchHandlerObject::InitializeComponent()
{
ATouchHUD* OwnerHUD=Cast<ATouchHUD>(GetOwner());
    for(uint8 Index=0;Index!=ComponentsSetup.Num();++Index)
    {
       
        UTouchObject* TempObj=    NewObject<UTouchObject>(this);
      //feedingfrom setup to obj.
        Components.Add(TempObj);
        TempObj->Center=ComponentsSetup[Index].Center;
        TempObj->Radius=ComponentsSetup[Index].FunctionalRadius;
        TempObj->JoystickThumb=ComponentsSetup[Index].JoystickThumb;
        TempObj->BackGround=ComponentsSetup[Index].BackGround;
        TempObj->Type=ComponentsSetup[Index].Type;
        TempObj->VisualSize=ComponentsSetup[Index].VisualSize;
     
        TempObj->bConsumeInput=ComponentsSetup[Index].bConsumeInput;
        
        TempObj->ThumbClamp=ComponentsSetup[Index].ThumbClamp;
        TempObj->SquareCenter=TempObj->Center;
        TempObj->SquareSize=ComponentsSetup[Index].SquareSize;
        TempObj->bRegisterTick=ComponentsSetup[Index].bRegisterTick;
        TempObj->FollowTouchSize=ComponentsSetup[Index].FollowTouchSize;
        TempObj->BroadCastConstant=ComponentsSetup[Index].BroadCastConstant;
        TempObj->InActiveBackGround=ComponentsSetup[Index].InActiveBackGround;
        TempObj->InActiveJoystickThumb=ComponentsSetup[Index].InActiveJoystickThumb;
        TempObj->ActiveColor=ComponentsSetup[Index].ActiveColor;
        TempObj->InActiveColor=ComponentsSetup[Index].InActiveColor;
        TempObj->HUD=OwnerHUD;
    }
    
    GetWorld()->GetTimerManager().SetTimer(JoystickTimer,this ,&UTouchHandlerObject::Timer,TickRate,true);
}
