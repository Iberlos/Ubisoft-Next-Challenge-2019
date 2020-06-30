#include "stdafx.h"

#include "BlackController.h"
#include "SimpleASPathfinder.h"
#include "SimpleAIControlledGameObject.h"
#include "SimpleScene.h"
#include "..\SimpleTileMap.h"

BlackController::BlackController(SimpleAIControlledGameObject * a_controlledGO):SimpleAIController(a_controlledGO)
{
}

BlackController::~BlackController()
{
}

void BlackController::Step_Idle()
{
    //IF candy or candyman in sight
    SimpleGameObject* go = m_controlledGO->GetClosestPickupWithTagInRange("Candy", m_sightRange);
    if (go)
    {
        m_targetGO = go;
        SwitchState(ESimpleAIStates::State_Follow);
        return;
    }
    go = m_controlledGO->GetClosestPickupWithTagInRange("Candyman", m_sightRange);
    if (go)
    {
        m_targetGO = go;
        SwitchState(ESimpleAIStates::State_Follow);
        return;
    }
    //IF m_bully
    if (m_bully)
    {
        go = nullptr;
        std::vector<SimpleGameObject*> childrenInRange;
        m_controlledGO->GetClosestPickupWithTagInRange("Child", m_sightRange, &childrenInRange);
        for (int i = 0; i< childrenInRange.size(); i++)
        {
            if (childrenInRange[i])
            {
                //Ask for controller, if it is not Black Set go as childrenInRange[i]
            }
        }
        if (go)
        {
            //cast to child and ask the state it is in.
            //m_targetGO = go;
            //SwitchState(ESimpleAIStates::State_Bully);
            //return;
        }
    }

    //If nothing in sight be bored for a bit
    SwitchState(ESimpleAIStates::State_Wait);
}

void BlackController::Step_Follow()
{
    //IF candy or candyman in sight
    SimpleGameObject* go = m_controlledGO->GetClosestPickupWithTagInRange("Candy", m_sightRange);
    if (go)
    {
        m_targetGO = go;
        SimpleAIController::Step_Follow();
        return;
    }
    go = m_controlledGO->GetClosestPickupWithTagInRange("Candyman", m_sightRange);
    if (go)
    {
        m_targetGO = go;
        SimpleAIController::Step_Follow();
        return;
    }

    //The target escaped...
    SwitchState(ESimpleAIStates::State_Idle);
}

void BlackController::Step_Bully()
{
    //IF candy, candyman or children in sight
    //candy
    SimpleGameObject* go = m_controlledGO->GetClosestPickupWithTagInRange("Candy", m_sightRange);
    if (go)
    {
        m_targetGO = go;
        SwitchState(ESimpleAIStates::State_Follow);
        return;
    }
    //candyman
    go = m_controlledGO->GetClosestPickupWithTagInRange("Candyman", m_sightRange);
    if (go)
    {
        m_targetGO = go;
        SwitchState(ESimpleAIStates::State_Follow);
        return;
    }
    //children with candy
    go = nullptr;
    std::vector<SimpleGameObject*> childrenInRange;
    m_controlledGO->GetClosestPickupWithTagInRange("Child", m_sightRange, &childrenInRange);
    for (int i = 0; i < childrenInRange.size(); i++)
    {
        if (childrenInRange[i])
        {
            //Ask for controller, if it is not Black Set go as childrenInRange[i]
        }
    }
    if (go)
    {
        //cast to child and ask the state it is in.
        //m_targetGO = go;
        //SwitchState(ESimpleAIStates::State_Bully);
        //return;
    }

    //The target escaped... calm down
    SwitchState(ESimpleAIStates::State_Idle);
}
