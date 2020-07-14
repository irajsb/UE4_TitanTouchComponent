// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchObject.h"
#include "Kismet/KismetMathLibrary.h"

static inline bool IsVector2DInRange(FVector2D in, FVector2D Center, FVector2D Size)
{//sometimes i need to Divide size by two to make it work:S
    if(Center.X-Size.X<in.X&&in.X<Center.X+Size.X)
    {
        if(Center.Y-Size.Y<in.Y&&in.Y<Center.Y+Size.Y)
        {
            return true;
        }

       
    }
    return false;
}
void UTouchObject::HandlePress(FVector2D Location)
{    //Handle Pressing 
    OnPress.Broadcast(Location);
    bIsPressed=true;
    TouchLocation=Location;
}

void UTouchObject::HandleRelease(FVector2D Location)
{//reset touch
    TouchLocation=Data.Center;
    OnRelease.Broadcast(Location);

    //recenter Thumb
    XInput=YInput=0;
    bIsPressed=false;
    ReservedIndex=255;
}

void UTouchObject::HandleMove(FVector2D Location)
{
    if(ReservedIndex!=255&&bIsPressed==true){
        if(Data.Type==ETouchComponentType::Swipe)
        {
            const FVector2D SwipeAmount =TouchLocation-Location;
            JoyStickAxis.Broadcast(SwipeAmount.X,SwipeAmount.Y);


    
        }
        else
            {
            OnMove.Broadcast(Location); 
        }

        TouchLocation=Location;


   
        if(ETouchComponentType::Joystick==Data.Type)
        {
        
                FVector AxisSize=FVector( CanvasLocation-Location,0);
      
 //map from range to -1,1     
                AxisSize.X=UKismetMathLibrary::MapRangeClamped(AxisSize.X,-Data.ThumbClamp,Data.ThumbClamp,1,-1);
                AxisSize.Y=UKismetMathLibrary::MapRangeClamped(AxisSize.Y,-Data.ThumbClamp,Data.ThumbClamp,-1,1);
                XInput= AxisSize.GetClampedToSize2D(-1,1).X;
                YInput=AxisSize.GetClampedToSize2D(-1,1).Y;
//if we need to broadcast in tick we dont need this broadcast 
            if(!Data.BroadCastConstant)
                    JoyStickAxis.Broadcast(XInput,YInput);
         
        }
    }
}

void UTouchObject::ReCenter(FVector2D Location)
{//used to move joystick to touch location if inside square
    Data.Center=Location/ HUD->ResRatio;
}

void UTouchObject::DrawTouchObject()
{//Draw Handler for each type of objects 

    if(Data.Type==ETouchComponentType::Joystick)
    {
        DrawJoystick();
    }
    else if(Data.Type==ETouchComponentType::Button){
DrawButton();
    }
    else if(Data.Type==ETouchComponentType::Swipe)
    {
        DrawSwipe();
    }


}

void UTouchObject::DrawJoystick()
{
 Canvas=HUD->GetCanvas();
    
    //Draw BG
    if(bIsPressed)
    {
        HUD->DrawTextureCentered(Data.BackGround,Data.Center.X,Data.Center.Y,Data.VisualSize.X,Data.VisualSize.Y,Data.ActiveColor);
        HUD->DrawTextureCentered(Data.JoystickThumb,Data.Center.X-XInput*-Data.ThumbClamp,Data.Center.Y-YInput*Data.ThumbClamp,Data.VisualSize.X*(Data.FunctionalRadius/Data.VisualSize.X),Data.VisualSize.Y*(Data.FunctionalRadius/Data.VisualSize.Y),Data.ActiveColor);
    }else
    {
        HUD->DrawTextureCentered(Data.InActiveBackGround,Data.Center.X,Data.Center.Y,Data.VisualSize.X,Data.VisualSize.Y,Data.InActiveColor);
        HUD->DrawTextureCentered(Data.InActiveJoystickThumb,Data.Center.X-XInput*-Data.ThumbClamp,Data.Center.Y-YInput*Data.ThumbClamp,Data.VisualSize.X*(Data.FunctionalRadius/Data.VisualSize.X),Data.VisualSize.Y*(Data.FunctionalRadius/Data.VisualSize.Y),Data.InActiveColor);
    }
    //Draw Thumb
 //Old code method to get object exact location in viewport
    CanvasLocation=FVector2D(HUD->ResRatio* Data.Center.X,HUD->ResRatio*Data.Center.Y);
      
     
    
}

void UTouchObject::DrawButton()
{
    
    if(bIsPressed)
    {
        HUD->DrawTextureCentered(Data.BackGround,Data.Center.X,Data.Center.Y,Data.VisualSize.X,Data.VisualSize.Y,Data.ActiveColor);
    }else
    {
        HUD->DrawTextureCentered(Data.InActiveBackGround,Data.Center.X,Data.Center.Y,Data.VisualSize.X,Data.VisualSize.Y,Data.InActiveColor);
    }
    CanvasLocation=FVector2D(HUD->ResRatio* Data.Center.X,HUD->ResRatio*Data.Center.Y);
}

void UTouchObject::DrawSwipe()
{
    CanvasLocation=FVector2D(HUD->ResRatio* Data.Center.X,HUD->ResRatio*Data.Center.Y);
}

void UTouchObject::Tick()
{
  if(Data.Type==ETouchComponentType::Joystick)
  {
      if(ReservedIndex!=255&&bIsPressed==true)
      {FVector2D Result;
          const FVector AxisSize=FVector( Data.Center-TouchLocation,0);
          if(Data.FollowTouchSize!=0&&AxisSize.Size()>Data.FollowTouchSize)
            Result=UKismetMathLibrary::Vector2DInterpTo(Data.Center,TouchLocation/HUD->ResRatio,1,Data.DynamicJoystickSpeed);
   if((IsVector2DInRange(Result,SquareCenter,Data.SquareSize/2)))
   {
       Data.Center=Result;
   }
      }
      if(Data.BroadCastConstant)
          JoyStickAxis.Broadcast(XInput,YInput);
  }
}
