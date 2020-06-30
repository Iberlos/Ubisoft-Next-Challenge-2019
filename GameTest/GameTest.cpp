//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
#include "MyClasses\SimpleScene.h"
#include "MyClasses\SimpleASPathfinder.h"

SimpleASPathFinder g_pathFinder;
SimpleScene g_scene;
//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
    //App::PlaySoundW("D:/Documents/UN_Chalenges/Ubisoft_Toronto_NEXT_Programming_API_2019/243641__nnus__nnus-self-wistle-test1.wav",true);
    g_scene.SetPathFinder(&g_pathFinder);
    g_scene.Init();
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
    static const float MIN_FRAMERATE = (1000 / 20);
    if (deltaTime > MIN_FRAMERATE)
        deltaTime = MIN_FRAMERATE;
    g_scene.Update(deltaTime/1000.0f);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine or Print) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	 
    g_scene.Render();
    //App::DrawLine(0.0f,0.0f,100.0f,100.0f,1.0f,1.0f,0.0f);
    //App::DrawQuad(0.0f,0.0f,13.0f,14.0f,0.0f,1.0f,0.0f);
    //App::Print(0.0f,0.0f,"Hello World!", 1.0f,0.0f,0.0f);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}