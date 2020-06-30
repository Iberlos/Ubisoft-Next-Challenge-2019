#include "stdafx.h"

#include "SimpleControlledGameObject.h"
#include "SimpleScene.h"
#include "..\SimpleTileMap.h"
#include "..\app\app.h"

SimpleControlledGameObject::SimpleControlledGameObject(SimpleScene* a_scene):SimpleGameObject(a_scene)
{
    m_speed = m_scene->GetMap()->GetTileWidth()*2;
    m_reach = m_scene->GetMap()->GetTileHeight()*0.5f;
    m_targetTileCoord[0] = -1; m_targetTileCoord[1] = -1;
    m_currentForwardDirection[0] = 0; m_currentForwardDirection[1] = 0;
    m_nextForwardDirection[0] = 0; m_nextForwardDirection[1] = 0;
}

SimpleControlledGameObject::~SimpleControlledGameObject()
{
}

void SimpleControlledGameObject::Update(float a_deltaTime)
{
    //Movement handling
//if m_targetTileCoord is valid
    if (m_targetTileCoord[0] != -1 && m_targetTileCoord[1] != -1)
    {
        //Get target tiles centered world pos
        float m_targetWorldPos[2];
        m_scene->GetMap()->TileCoordToWorld(m_targetTileCoord[0], m_targetTileCoord[1], &m_targetWorldPos[0], &m_targetWorldPos[1]);
        //Get the currently ocupied tile
        int m_currentTileCoord[2];
        m_scene->GetMap()->WorldToTileCoord(m_pos[0], m_pos[1], &m_currentTileCoord[0], &m_currentTileCoord[1]);
        //if the distance to the target is smaller then movement this frame 
        if (((m_targetWorldPos[0] - m_pos[0])*(m_targetWorldPos[0] - m_pos[0]) + (m_targetWorldPos[1] - m_pos[1])*(m_targetWorldPos[1] - m_pos[1])) < (m_speed*a_deltaTime)*(m_speed*a_deltaTime))
        {
            //snap to center to avoid the possibility of drifting
            SetPosition(m_targetWorldPos[0], m_targetWorldPos[1]);
            //Do whatever you need to do when you reach your target, the function is overriden in base classes
            TargetTileReached();
            //The tile in the next direction is open
            if (m_scene->GetMap()->GetTileMapValue(m_currentTileCoord[0] + m_nextForwardDirection[0], m_currentTileCoord[1] + m_nextForwardDirection[1]) == EMapValue::FLOOR && (m_nextForwardDirection[0] != 0 || m_nextForwardDirection[1] != 0))
            {
                m_currentForwardDirection[0] = m_nextForwardDirection[0];
                m_currentForwardDirection[1] = m_nextForwardDirection[1];
            }
            //if the tile in current forward direction is open
            if (m_scene->GetMap()->GetTileMapValue(m_currentTileCoord[0] + m_currentForwardDirection[0], m_currentTileCoord[1] + m_currentForwardDirection[1]) == EMapValue::FLOOR)
            {
                //Set the target tile as the tile in the current forward direction
                m_targetTileCoord[0] = m_currentTileCoord[0] + m_currentForwardDirection[0];
                m_targetTileCoord[1] = m_currentTileCoord[1] + m_currentForwardDirection[1];
            }
            else
            {
                StopMovement();
            }
        }
        else
        {
            //move in the m_targetWorldPos direction
            float dir[2];
            dir[0] = m_targetWorldPos[0] - m_pos[0];
            dir[1] = m_targetWorldPos[1] - m_pos[1];
            float length = sqrt(dir[0] * dir[0] + dir[1] * dir[1]);
            dir[0] /= length;
            dir[1] /= length;
            Translate(dir[0] * m_speed*a_deltaTime, dir[1] * m_speed*a_deltaTime);
        }
    }
    else
    {
        //Get the currently ocupied tile
        int m_currentTileCoord[2];
        m_scene->GetMap()->WorldToTileCoord(m_pos[0], m_pos[1], &m_currentTileCoord[0], &m_currentTileCoord[1]);
        //The tile in the next direction is open
        if (m_scene->GetMap()->GetTileMapValue(m_currentTileCoord[0] + m_nextForwardDirection[0], m_currentTileCoord[1] + m_nextForwardDirection[1]) == EMapValue::FLOOR && (m_nextForwardDirection[0] != 0 || m_nextForwardDirection[1] != 0))
        {
            //make m_targetTileCoord the valid coord so we can move next frame
            m_currentForwardDirection[0] = m_nextForwardDirection[0];
            m_currentForwardDirection[1] = m_nextForwardDirection[1];
            m_targetTileCoord[0] = m_currentTileCoord[0] + m_nextForwardDirection[0];
            m_targetTileCoord[1] = m_currentTileCoord[1] + m_nextForwardDirection[1];
        }
        else
        {
            StopMovement();
        }
    }
}

void SimpleControlledGameObject::StopMovement()
{
    //Do whatever you need to do when you reach your target, the function is overriden in base classes
    TargetTileReached();
    //stop moving
    m_targetTileCoord[0] = -1;
    m_targetTileCoord[0] = -1;
    m_currentForwardDirection[0] = 0;
    m_currentForwardDirection[1] = 0;
    m_nextForwardDirection[0] = 0;
    m_nextForwardDirection[1] = 0;
}

void SimpleControlledGameObject::GetAllPickupsWithTagInRange(std::string a_tag, float a_range, std::vector<SimpleGameObject*>* a_vectorToFill)
{
    std::vector<SimpleGameObject*> objectsWithTag;
    m_scene->GetAllGOsWithTag(a_tag, &objectsWithTag);

    for (int i = 0; i< objectsWithTag.size(); i++)
    {
        float* GOpos = objectsWithTag[i]->GetPosition();
        if (((GOpos[0]-m_pos[0])*(GOpos[0] - m_pos[0]) + (GOpos[1] - m_pos[1])*(GOpos[1] - m_pos[1])) < a_range*a_range)
        {
            (*a_vectorToFill).push_back(objectsWithTag[i]);
        }
    }
}

SimpleGameObject* SimpleControlledGameObject::GetClosestPickupWithTagInRange(std::string a_tag, float a_range, std::vector<SimpleGameObject*>* a_vectorToFill)
{
    //if no vector was passed in make one for the proceedings
    if (!a_vectorToFill)
    {
        std::vector<SimpleGameObject*> m_vec;
        a_vectorToFill = &m_vec;
    }

    //Get all objects in range
    std::vector<SimpleGameObject*> objectsWithTag;
    m_scene->GetAllGOsWithTag(a_tag, &objectsWithTag);

    for (int i = 0; i < objectsWithTag.size(); i++)
    {
        float* GOpos = objectsWithTag[i]->GetPosition();
        if (((GOpos[0] - m_pos[0])*(GOpos[0] - m_pos[0]) + (GOpos[1] - m_pos[1])*(GOpos[1] - m_pos[1])) < a_range*a_range)
        {
            (*a_vectorToFill).push_back(objectsWithTag[i]);
        }
    }

    //Buble sort to find the closest and organize the vector
    if ((*a_vectorToFill).size() > 1)
    {
        bool finished = true;
        do
        {
            for (int i = 0; i < (*a_vectorToFill).size() - 1; i++)
            {
                if ((*a_vectorToFill)[i + 1] > (*a_vectorToFill)[i])
                {
                    SimpleGameObject* temp = (*a_vectorToFill)[i];
                    (*a_vectorToFill)[i] = (*a_vectorToFill)[i + 1];
                    (*a_vectorToFill)[i + 1] = temp;
                    finished = false;
                }
            }
        } while (!finished);
    }
    else
    {
        (*a_vectorToFill).push_back(nullptr);
    }


    //Return the front of the vector
    return (*a_vectorToFill).front();
}

void SimpleControlledGameObject::TargetTileReached()
{
}
