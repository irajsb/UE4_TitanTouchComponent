// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchHUD.h"
#include "TouchHandlerObject.h"


ATouchHUD::ATouchHUD()
{
    TouchHandler=CreateDefaultSubobject<UTouchHandlerObject>(TEXT("Touch Handler"));
    bShowHUD=true;
    
}

void ATouchHUD::DrawHUD()
{
   
    Super::DrawHUD();
    
    
    const float CanvasSize=Canvas->SizeX;
    ResRatio=CanvasSize/1280;
  
for (int8 index=TouchHandler->Components.Num()-1;index!= -1;--index)
{
    if(TouchHandler->Components[index]->bHidden==false)
    {
        TouchHandler->Components[index]->DrawTouchObject();
    }
}
    
}





void ATouchHUD::DrawTextureCentered(UTexture2D* Texture,float x ,float y,float Width,float Height,FLinearColor Color)
{
  
        DrawTexture(Texture,x*ResRatio-(Width*ResRatio/2),y*ResRatio-(Height*ResRatio/2),Width*ResRatio,Height*ResRatio,0,0,1,1,Color);
   
   
           
}

void ATouchHUD::DrawDebugRectCentered(FLinearColor col, float x, float y, float w, float h)
{
    DrawRect(col,ResRatio*(x-w/2),ResRatio*(y-h/2),ResRatio*w,ResRatio*h);
}

UCanvas* ATouchHUD::GetCanvas()const
{return  Canvas;
}

void ATouchHUD::DrawDebugs()
{

    for (int8 index=0;index!= TouchHandler->Components.Num();index++)
    {
        const UTouchObject* Component=TouchHandler->Components[index];
        if(Component->Type==ETouchComponentType::Joystick)
        {    FLinearColor Thumb;
            Thumb.R=1;
            Thumb.A=0.3;
            FLinearColor Square;
            Square.G=1;
            Square.A=0.05;
           DrawDebugRectCentered(Square,Component->SquareCenter.X,Component->SquareCenter.Y,Component->SquareSize.X,Component->SquareSize.Y);
            DrawDebugRectCentered(Thumb,Component->Center.X-Component->XInput*-Component->ThumbClamp,Component->Center.Y-Component->YInput*Component->ThumbClamp,Component->VisualSize.X*(Component->Radius/Component->VisualSize.X),Component->VisualSize.Y*(Component->Radius/Component->VisualSize.Y));
            DrawDebugRectCentered(FLinearColor::Blue,Component->Center.X,Component->Center.Y,6,6);

        }else if(Component->Type==ETouchComponentType::Swipe)
        {FLinearColor Square;
            Square.G=1;
            Square.A=0.1;
            DrawDebugRectCentered(Square,Component->SquareCenter.X,Component->SquareCenter.Y,Component->SquareSize.X,Component->SquareSize.Y);

        }else
        {
            DrawDebugRectCentered(FLinearColor::Blue,Component->Center.X,Component->Center.Y,6,6);
            
        }
        
    }
}
