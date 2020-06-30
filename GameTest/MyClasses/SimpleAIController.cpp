#include "stdafx.h"

#include "SimpleAIController.h"
#include "SimpleASPathfinder.h"
#include "SimpleAIControlledGameObject.h"
#include "SimpleScene.h"
#include "..\SimpleTileMap.h"

SimpleAIController::SimpleAIController(SimpleAIControlledGameObject* a_controlledGO)
{
    m_controlledGO = a_controlledGO;
    m_ASPathFinder = m_controlledGO->GetScene()->GetPathFinder();

    m_scolded = false;
    m_bully = (int)(rand()%500/100.0f); //20% chance of being a bully
    m_bullied = false;
    m_waitDuration = 1.0f;
    m_waitTimer = 0.0f;
    m_sightRange = m_controlledGO->GetScene()->GetMap()->GetTileWidth() * 3;

    m_currentState = ESimpleAIStates::State_Idle;

    m_EnterStates[ESimpleAIStates::State_Idle   ] = &SimpleAIController::Enter_Idle;
    m_EnterStates[ESimpleAIStates::State_Follow ] = &SimpleAIController::Enter_Follow;
    m_EnterStates[ESimpleAIStates::State_Wait   ] = &SimpleAIController::Enter_Wait;
    m_EnterStates[ESimpleAIStates::State_Home   ] = &SimpleAIController::Enter_Home;
    m_EnterStates[ESimpleAIStates::State_Bully  ] = &SimpleAIController::Enter_Bully;
    m_EnterStates[ESimpleAIStates::State_Bullied] = &SimpleAIController::Enter_Bullied;

    m_UpdateStates[ESimpleAIStates::State_Idle   ] = &SimpleAIController::Update_Idle;
    m_UpdateStates[ESimpleAIStates::State_Follow ] = &SimpleAIController::Update_Follow;
    m_UpdateStates[ESimpleAIStates::State_Wait   ] = &SimpleAIController::Update_Wait;
    m_UpdateStates[ESimpleAIStates::State_Home   ] = &SimpleAIController::Update_Home;
    m_UpdateStates[ESimpleAIStates::State_Bully  ] = &SimpleAIController::Update_Bully;
    m_UpdateStates[ESimpleAIStates::State_Bullied] = &SimpleAIController::Update_Bullied;

    m_StepStates[ESimpleAIStates::State_Idle   ] = &SimpleAIController::Step_Idle;
    m_StepStates[ESimpleAIStates::State_Follow ] = &SimpleAIController::Step_Follow;
    m_StepStates[ESimpleAIStates::State_Wait   ] = &SimpleAIController::Step_Wait;
    m_StepStates[ESimpleAIStates::State_Home   ] = &SimpleAIController::Step_Home;
    m_StepStates[ESimpleAIStates::State_Bully  ] = &SimpleAIController::Step_Bully;
    m_StepStates[ESimpleAIStates::State_Bullied] = &SimpleAIController::Step_Bullied;

    m_TileReachedStates[ESimpleAIStates::State_Idle   ] = &SimpleAIController::TileReached_Idle;
    m_TileReachedStates[ESimpleAIStates::State_Follow ] = &SimpleAIController::TileReached_Follow;
    m_TileReachedStates[ESimpleAIStates::State_Wait   ] = &SimpleAIController::TileReached_Wait;
    m_TileReachedStates[ESimpleAIStates::State_Home   ] = &SimpleAIController::TileReached_Home;
    m_TileReachedStates[ESimpleAIStates::State_Bully  ] = &SimpleAIController::TileReached_Bully;
    m_TileReachedStates[ESimpleAIStates::State_Bullied] = &SimpleAIController::TileReached_Bullied;

    m_ExitStates[ESimpleAIStates::State_Idle   ] = &SimpleAIController::Exit_Idle;
    m_ExitStates[ESimpleAIStates::State_Follow ] = &SimpleAIController::Exit_Follow;
    m_ExitStates[ESimpleAIStates::State_Wait   ] = &SimpleAIController::Exit_Wait;
    m_ExitStates[ESimpleAIStates::State_Home   ] = &SimpleAIController::Exit_Home;
    m_ExitStates[ESimpleAIStates::State_Bully  ] = &SimpleAIController::Exit_Bully;
    m_ExitStates[ESimpleAIStates::State_Bullied] = &SimpleAIController::Exit_Bullied;

    ////Initialize the movement
    //SwitchState(ESimpleAIStates::State_Idle);
}

SimpleAIController::~SimpleAIController()
{
}

void SimpleAIController::Update(float a_deltaTime)
{
    (this->* (m_UpdateStates[m_currentState]))(a_deltaTime);
}

void SimpleAIController::Step()
{
    (this->* (m_StepStates[m_currentState]))();
}

void SimpleAIController::TileReached()
{
    (this->* (m_TileReachedStates[m_currentState]))();
}

void SimpleAIController::Enter_Idle()
{
    //SaySomething("Sigh...")

//Move once when you enter this state. I hope it is the only one that needs this.
    int foundlength = 0;
    int pathLength = 30;
    int* path = new int[pathLength];

    //------------------------------------------------------------------------------------------------------------------------------------------
    int ctc[2];
    m_controlledGO->GetScene()->GetMap()->WorldToTileCoord(m_controlledGO->GetPosition()[0], m_controlledGO->GetPosition()[1], &ctc[0], &ctc[1]);
    int ttc[2];
    m_controlledGO->GetScene()->GetMap()->GetRandomTileCoordOfValueInRange(EMapValue::FLOOR, ctc[0], ctc[1], 3, &ttc[0], &ttc[1]);
    if (m_ASPathFinder->FindPath(m_controlledGO->GetScene(), ctc[0], ctc[1], ttc[0], ttc[1]))
    {
        foundlength = m_ASPathFinder->GetPath(path, pathLength, ttc[0], ttc[1]);
    }
    //------------------------------------------------------------------------------------------------------------------------------------------

    int stepDistance = 3;
    if (foundlength > pathLength)
    {
        foundlength = pathLength;
    }
    if (stepDistance > foundlength)
    {
        stepDistance = foundlength;
    }

    m_controlledGO->SetPath(path, foundlength, stepDistance);

    delete path;
}

void SimpleAIController::Enter_Follow()
{
    if (m_targetGO)
    {
        if (m_targetGO->GameObjectHasTag("Candy"))
        {
            //SaySomething("Candy!")
        }
        else if (m_targetGO->GameObjectHasTag("Candyman"))
        {
            //SaySomething("It is Mr. Candyman!!")
        }
    }
}

void SimpleAIController::Enter_Wait()
{
    //SaySomething("...")

    m_waitTimer = m_waitDuration;
}

void SimpleAIController::Enter_Home()
{
    //SaySomething("But I cleaned my room last week!");
}

void SimpleAIController::Enter_Bully()
{
    //SaySomething("That is my Candy!", 1.0f)
}

void SimpleAIController::Enter_Bullied()
{
    //SaySomething("Noooooooooo!!")
}

void SimpleAIController::Update_Idle(float a_deltaTime)
{
    //SaySomethingRandomOnIntervals()?
}

void SimpleAIController::Update_Follow(float a_deltaTime)
{
    //SaySomethingRandomOnIntervals()?
}

void SimpleAIController::Update_Wait(float a_deltaTime)
{
    if (m_waitTimer>0)
    {
        m_waitTimer -= a_deltaTime;
    }
    else
    {
        m_waitTimer = 0;
        SwitchState(ESimpleAIStates::State_Idle);
    }
}

void SimpleAIController::Update_Home(float a_deltaTime)
{
    //SaySomethingRandomOnIntervals()?
}

void SimpleAIController::Update_Bully(float a_deltaTime)
{
    //SaySomethingRandomOnIntervals()?
}

void SimpleAIController::Update_Bullied(float a_deltaTime)
{
    //SaySomethingRandomOnIntervals()?
}

void SimpleAIController::Step_Idle()
{
    int foundlength = 0;
    int pathLength = 30;
    int* path = new int[pathLength];
    if (m_targetGO)
    {
        int ctc[2];
        m_controlledGO->GetScene()->GetMap()->WorldToTileCoord(m_controlledGO->GetPosition()[0], m_controlledGO->GetPosition()[1], &ctc[0], &ctc[1]);
        int ttc[2];
        m_controlledGO->GetScene()->GetMap()->WorldToTileCoord(m_targetGO->GetPosition()[0], m_targetGO->GetPosition()[1], &ttc[0], &ttc[1]);
        if (m_ASPathFinder->FindPath(m_controlledGO->GetScene(), ctc[0], ctc[1], ttc[0], ttc[1]))
        {
            foundlength = m_ASPathFinder->GetPath(path, pathLength, ttc[0], ttc[1]);
        }
    }
    else
    {
        int ctc[2];
        m_controlledGO->GetScene()->GetMap()->WorldToTileCoord(m_controlledGO->GetPosition()[0], m_controlledGO->GetPosition()[1], &ctc[0], &ctc[1]);
        int ttc[2];
        m_controlledGO->GetScene()->GetMap()->GetRandomTileCoordOfValueInRange(EMapValue::FLOOR, ctc[0], ctc[1], 3, &ttc[0], &ttc[1]);
        if (m_ASPathFinder->FindPath(m_controlledGO->GetScene(), ctc[0], ctc[1], ttc[0], ttc[1]))
        {
            foundlength = m_ASPathFinder->GetPath(path, pathLength, ttc[0], ttc[1]);
        }
    }

    int stepDistance = 3;
    if (foundlength > pathLength)
    {
        foundlength = pathLength;
    }
    if (stepDistance > foundlength)
    {
        stepDistance = foundlength;
    }


    m_controlledGO->SetPath(path, foundlength, stepDistance);

    delete path;
}

void SimpleAIController::Step_Follow()
{
    int foundlength = 0;
    int pathLength = 30;
    int* path = new int[pathLength];

    //------------------------------------------------------------------------------------------------------------------------------------------
    if (m_targetGO)
    {
        int ctc[2];
        m_controlledGO->GetScene()->GetMap()->WorldToTileCoord(m_controlledGO->GetPosition()[0], m_controlledGO->GetPosition()[1], &ctc[0], &ctc[1]);
        int ttc[2];
        m_controlledGO->GetScene()->GetMap()->WorldToTileCoord(m_targetGO->GetPosition()[0], m_targetGO->GetPosition()[1], &ttc[0], &ttc[1]);
        if (m_ASPathFinder->FindPath(m_controlledGO->GetScene(), ctc[0], ctc[1], ttc[0], ttc[1]))
        {
            foundlength = m_ASPathFinder->GetPath(path, pathLength, ttc[0], ttc[1]);
        }
    }
    //------------------------------------------------------------------------------------------------------------------------------------------

    int stepDistance = 3;
    if (foundlength > pathLength)
    {
        foundlength = pathLength;
    }
    if (stepDistance > foundlength)
    {
        stepDistance = foundlength;
    }

    m_controlledGO->SetPath(path, foundlength, stepDistance);

    delete path;
}

void SimpleAIController::Step_Wait()
{
}

void SimpleAIController::Step_Home()
{
    if (!m_targetGO->GameObjectHasTag("Mom"))
    {
        //m_targetGO = m_controlledGO->GetMom();
    }

    int foundlength = 0;
    int pathLength = 30;
    int* path = new int[pathLength];

    //------------------------------------------------------------------------------------------------------------------------------------------
    if (m_targetGO)
    {
        int ctc[2];
        m_controlledGO->GetScene()->GetMap()->WorldToTileCoord(m_controlledGO->GetPosition()[0], m_controlledGO->GetPosition()[1], &ctc[0], &ctc[1]);
        int ttc[2];
        m_controlledGO->GetScene()->GetMap()->WorldToTileCoord(m_targetGO->GetPosition()[0], m_targetGO->GetPosition()[1], &ttc[0], &ttc[1]);
        if (m_ASPathFinder->FindPath(m_controlledGO->GetScene(), ctc[0], ctc[1], ttc[0], ttc[1]))
        {
            foundlength = m_ASPathFinder->GetPath(path, pathLength, ttc[0], ttc[1]);
        }
    }
    //------------------------------------------------------------------------------------------------------------------------------------------

    int stepDistance = 3;
    if (foundlength > pathLength)
    {
        foundlength = pathLength;
    }
    if (stepDistance > foundlength)
    {
        stepDistance = foundlength;
    }

    m_controlledGO->SetPath(path, foundlength, stepDistance);

    delete path;
}

void SimpleAIController::Step_Bully()
{
}

void SimpleAIController::Step_Bullied()
{
    //IF Candy in sight
    //Set closest as target
    SwitchState(ESimpleAIStates::State_Follow);
    //END IF
}

void SimpleAIController::TileReached_Idle()
{
    //nothing
}

void SimpleAIController::TileReached_Follow()
{
    //nothing
}

void SimpleAIController::TileReached_Wait()
{
    //nothing
}

void SimpleAIController::TileReached_Home()
{
    //nothing
}

void SimpleAIController::TileReached_Bully()
{
    //nothing
}

void SimpleAIController::TileReached_Bullied()
{
    //nothing
}

void SimpleAIController::Exit_Idle()
{
    //SaySomething("...")
}

void SimpleAIController::Exit_Follow()
{
    //SaySomething("Awwww...")
}

void SimpleAIController::Exit_Wait()
{
    //SaySomething("Hummm?")
}

void SimpleAIController::Exit_Home()
{
    //Never exist
}

void SimpleAIController::Exit_Bully()
{
    //SaySomething("Oh, fuck it...")
}

void SimpleAIController::Exit_Bullied()
{
    //SaySomething("Thanks Mr. Candyman!!")
}

void SimpleAIController::SwitchState(ESimpleAIStates a_stateToSwitch)
{
    (this->* (m_ExitStates[m_currentState]))();
    (this->* (m_EnterStates[a_stateToSwitch]))();
    m_currentState = a_stateToSwitch;
}

