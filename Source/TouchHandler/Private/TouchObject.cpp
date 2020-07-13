// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchObject.h"
#include "Kismet/KismetMathLibrary.h"

static inline bool IsVector2DInRange(FVector2D in, FVector2D Center, FVector2D Size)
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
void UTouchObject::HandlePress(FVector2D Location)
{
    OnPress.Broadcast(Location);
    bIsPressed=true;
    TouchLocation=Location;
}

void UTouchObject::HandleRelease(FVector2D Location)
{  TouchLocation=Center;
    OnRelease.Broadcast(Location);

    //recenter Thumb
    XInput=YInput=0;
    bIsPressed=false;
    ReservedIndex=255;
}

void UTouchObject::HandleMove(FVector2D Location)
{
    if(ReservedIndex!=255&&bIsPressed==true){
        if(Type==ETouchComponentType::Swipe)
        {
            const FVector2D SwipeAmount =TouchLocation-Location;
            JoyStickAxis.Broadcast(SwipeAmount.X,SwipeAmount.Y);
UE_LOG(LogTemp,Log,TEXT("%f"),SwipeAmount.X);

    
        }else{
            OnMove.Broadcast(Location);
        }

        TouchLocation=Location;


   
        if(ETouchComponentType::Joystick==Type)
        {
        
                FVector AxisSize=FVector( CanvasLocation-Location,0);
      
      
                AxisSize.X=UKismetMathLibrary::MapRangeClamped(AxisSize.X,-ThumbClamp,ThumbClamp,1,-1);
                AxisSize.Y=UKismetMathLibrary::MapRangeClamped(AxisSize.Y,-ThumbClamp,ThumbClamp,-1,1);
                XInput= AxisSize.GetClampedToSize2D(-1,1).X;
                YInput=AxisSize.GetClampedToSize2D(-1,1).Y;
                if(!BroadCastConstant)
                    JoyStickAxis.Broadcast(XInput,YInput);
         
        }
    }
}

void UTouchObject::ReCenter(FVector2D Location)
{Center=Location/ HUD->ResRatio;
}

void UTouchObject::DrawTouchObject()
{

    if(Type==ETouchComponentType::Joystick)
    {
        DrawJoystick();
    }
    else if(Type==ETouchComponentType::Button){
DrawButton();
    }
    else if(Type==ETouchComponentType::Swipe)
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
        HUD->DrawTextureCentered(BackGround,Center.X,Center.Y,VisualSize.X,VisualSize.Y,ActiveColor);
        HUD->DrawTextureCentered(JoystickThumb,Center.X-XInput*-ThumbClamp,Center.Y-YInput*ThumbClamp,VisualSize.X*(Radius/VisualSize.X),VisualSize.Y*(Radius/VisualSize.Y),ActiveColor);
    }else
    {
        HUD->DrawTextureCentered(InActiveBackGround,Center.X,Center.Y,VisualSize.X,VisualSize.Y,InActiveColor);
        HUD->DrawTextureCentered(InActiveJoystickThumb,Center.X-XInput*-ThumbClamp,Center.Y-YInput*ThumbClamp,VisualSize.X*(Radius/VisualSize.X),VisualSize.Y*(Radius/VisualSize.Y),InActiveColor);
    }
    //Draw Thumb
 
    CanvasLocation=FVector2D(HUD->ResRatio* Center.X,HUD->ResRatio*Center.Y);
      
     
    
}

void UTouchObject::DrawButton()
{
    if(bIsPressed)
    {
        HUD->DrawTextureCentered(BackGround,Center.X,Center.Y,VisualSize.X,VisualSize.Y,ActiveColor);
    }else
    {
        HUD->DrawTextureCentered(InActiveBackGround,Center.X,Center.Y,VisualSize.X,VisualSize.Y,InActiveColor);
    }
    CanvasLocation=FVector2D(HUD->ResRatio* Center.X,HUD->ResRatio*Center.Y);
}

void UTouchObject::DrawSwipe()
{
    CanvasLocation=FVector2D(HUD->ResRatio* Center.X,HUD->ResRatio*Center.Y);
}

void UTouchObject::Tick()
{
  if(Type==ETouchComponentType::Joystick)
  {
      if(ReservedIndex!=255&&bIsPressed==true)
      {
          const FVector AxisSize=FVector( CanvasLocation-TouchLocation,0);
          if(FollowTouchSize!=0&&AxisSize.Size()>FollowTouchSize&&(SquareCenter.IsZero()||(IsVector2DInRange(Center,SquareCenter,SquareSize/2)||IsVector2DInRange(TouchLocation,SquareCenter,SquareSize/2))))
              Center=UKismetMathLibrary::Vector2DInterpTo(Center,TouchLocation,1,0.01);
   
      }
      if(BroadCastConstant)
          JoyStickAxis.Broadcast(XInput,YInput);
  }
}
