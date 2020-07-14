// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchHUD.h"
#include "TouchHandlerObject.h"


ATouchHUD::ATouchHUD()
{    //init touch handler object
    TouchHandler=CreateDefaultSubobject<UTouchHandlerObject>(TEXT("Touch Handler"));
    bShowHUD=true;
    
}

void ATouchHUD::DrawHUD()
{
   //dont do anything before superclass call
    Super::DrawHUD();
    
    //Resratio keeps widgets relative to screen size
    const float CanvasSize=Canvas->SizeX;
    ResRatio=CanvasSize/1280;
  //loop through  objects and draw them if visible
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
  //draw texture centered 
        DrawTexture(Texture,x*ResRatio-(Width*ResRatio/2),y*ResRatio-(Height*ResRatio/2),Width*ResRatio,Height*ResRatio,0,0,1,1,Color);
   
   
           
}

void ATouchHUD::DrawDebugRectCentered(FLinearColor col, float x, float y, float w, float h)
{//just for debugging
    DrawRect(col,ResRatio*(x-w/2),ResRatio*(y-h/2),ResRatio*w,ResRatio*h);
}

UCanvas* ATouchHUD::GetCanvas()const
{return  Canvas;
}

void ATouchHUD::DrawDebugs()
{//TODO Make this work using draw texture if needed

    for (int8 index=0;index!= TouchHandler->Components.Num();index++)
    {
        const UTouchObject* Component=TouchHandler->Components[index];
        if(Component->Data.Type==ETouchComponentType::Joystick)
        {    FLinearColor Thumb;
            Thumb.R=1;
            Thumb.A=0.3;
            FLinearColor Square;
            Square.G=1;
            Square.A=0.05;
           DrawDebugRectCentered(Square,Component->SquareCenter.X,Component->SquareCenter.Y,Component->Data.SquareSize.X,Component->Data.SquareSize.Y);
            DrawDebugRectCentered(Thumb,Component->Data.Center.X-Component->XInput*-Component->Data.ThumbClamp,Component->Data.Center.Y-Component->YInput*Component->Data.ThumbClamp,Component->Data.VisualSize.X*(Component->Data.FunctionalRadius/Component->Data.VisualSize.X),Component->Data.VisualSize.Y*(Component->Data.FunctionalRadius/Component->Data.VisualSize.Y));
            DrawDebugRectCentered(FLinearColor::Blue,Component->Data.Center.X,Component->Data.Center.Y,6,6);

        }else if(Component->Data.Type==ETouchComponentType::Swipe)
        {FLinearColor Square;
            Square.G=1;
            Square.A=0.1;
            DrawDebugRectCentered(Square,Component->SquareCenter.X,Component->SquareCenter.Y,Component->Data.SquareSize.X,Component->Data.SquareSize.Y);

        }else
        {
            DrawDebugRectCentered(FLinearColor::Blue,Component->Data.Center.X,Component->Data.Center.Y,6,6);
            
        }
        
    }
}
