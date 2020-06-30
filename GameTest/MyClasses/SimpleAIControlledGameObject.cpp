#include "stdafx.h"

#include "SimpleAIControlledGameObject.h"
#include "Controllers.h"
#include "SimpleScene.h"
#include "..\App\app.h"
#include "..\SimpleTileMap.h"

SimpleAIControlledGameObject::SimpleAIControlledGameObject(SimpleScene* a_scene): SimpleControlledGameObject(a_scene)
{
    m_currentController = EControllers::Black_Controller;
    for (int i = 0; i< EControllers::LAST_Controller; i++)
    {
        m_availableControllers.push_back(nullptr);
    }
    m_availableControllers[EControllers::Black_Controller] = new BlackController(this);
    m_availableControllers[EControllers::Red_Controller] = new RedController(this);
    m_availableControllers[EControllers::Green_Controller] = new GreenController(this);
    m_availableControllers[EControllers::Blue_Controller] = new BlueController(this);
    m_availableControllers[EControllers::Cyan_Controller] = new CyanController(this);
    m_availableControllers[EControllers::Magenta_Controller] = new MagentaController(this);
    m_availableControllers[EControllers::Yellow_Controller] = new YellowController(this);
    m_availableControllers[EControllers::White_Controller] = new WhiteController(this);

    CreateMesh(0.0f, 0.0f, 0.0f, 0.3f, m_pos);
}

SimpleAIControlledGameObject::~SimpleAIControlledGameObject()
{
    for (int i = 0; i< m_availableControllers.size(); i++)
    {
        delete m_availableControllers[i];
    }
}

void SimpleAIControlledGameObject::Update(float a_deltaTime)
{
    m_availableControllers[m_currentController]->Update(a_deltaTime);
    if (m_path.size() == 0)
    {
        m_availableControllers[m_currentController]->Step();
    }
    else
    {
        m_scene->GetMap()->IndexToTileCoord(m_path.front(), &m_targetTileCoord[0], &m_targetTileCoord[1]);
    }
    SimpleControlledGameObject::Update(a_deltaTime);
}

void SimpleAIControlledGameObject::Render()
{
    RenderPath();
    RenderTarget();
    RenderSightAsRectangle();
    SimpleControlledGameObject::Render();
}

void SimpleAIControlledGameObject::Reset()
{
    m_path.clear();
    SimpleControlledGameObject::Reset();
}

void SimpleAIControlledGameObject::TargetTileReached()
{
    if (m_path.size() > 0)
    {
        int ttc[2];
        m_scene->GetMap()->IndexToTileCoord(m_path.front(), &ttc[0], &ttc[1]);
        int ctc[2];
        m_scene->GetMap()->WorldToTileCoord(m_pos[0], m_pos[1], &ctc[0], &ctc[1]);
        if (ttc[0] == ctc[0] && ttc[1] == ctc[1])
        {
            m_path.erase(m_path.begin());
        }
    }
}

void SimpleAIControlledGameObject::RenderPath()
{
    if (m_path.size() > 0)
    {
        //draw the first segment from the players position to the next tile in the path
        float sw[2] = { m_pos[0] , m_pos[1] };

        int etc[2];
        float ew[2];
        m_scene->GetMap()->IndexToTileCoord(m_path[0], &etc[0], &etc[1]);
        m_scene->GetMap()->TileCoordToWorld(etc[0], etc[1], &ew[0], &ew[1]);

        //draw path section in white
        App::DrawLine(sw[0], sw[1], ew[0], ew[1], 1.0f, 1.0f, 1.0f);

        //draw the other segments
        for (int i = 0; i < m_path.size() - 1; i++)
        {
            int stc[2];
            float sw[2];
            m_scene->GetMap()->IndexToTileCoord(m_path[i], &stc[0], &stc[1]);
            m_scene->GetMap()->TileCoordToWorld(stc[0], stc[1], &sw[0], &sw[1]);
            int etc[2];
            float ew[2];
            m_scene->GetMap()->IndexToTileCoord(m_path[i + 1], &etc[0], &etc[1]);
            m_scene->GetMap()->TileCoordToWorld(etc[0], etc[1], &ew[0], &ew[1]);

            //draw path section in white
            App::DrawLine(sw[0], sw[1], ew[0], ew[1], 1.0f, 1.0f, 1.0f);
        }
    }
}

void SimpleAIControlledGameObject::RenderTarget()
{
    if (m_path.size() > 0)
    {
        int stc[2];
        float sw[2];
        m_scene->GetMap()->IndexToTileCoord(m_path.back(), &stc[0], &stc[1]);
        m_scene->GetMap()->TileCoordToWorld(stc[0], stc[1], &sw[0], &sw[1], false);

        float th = m_scene->GetMap()->GetTileHeight();
        float tw = m_scene->GetMap()->GetTileWidth();

        //Draw target marker in red
        App::DrawLine(sw[0]     , sw[1]     , sw[0]     , sw[1] + th, 1.0f, 0.0f, 0.0f);//left wall
        App::DrawLine(sw[0] + tw, sw[1]     , sw[0] + tw, sw[1] + th, 1.0f, 0.0f, 0.0f);//right wall
        App::DrawLine(sw[0]     , sw[1] + th, sw[0] + tw, sw[1] + th, 1.0f, 0.0f, 0.0f);//upper wall
        App::DrawLine(sw[0]     , sw[1]     , sw[0] + tw, sw[1]     , 1.0f, 0.0f, 0.0f);//lower wall
    }
}

void SimpleAIControlledGameObject::RenderSightAsRectangle()
{
    float sightRange = m_availableControllers[m_currentController]->GetSightRange();
    float sw[2] = { m_pos[0] - sightRange, m_pos[1] - sightRange };

    float th = sightRange*2;
    float tw = sightRange*2;

    //Draw target marker in red
    App::DrawLine(sw[0], sw[1], sw[0], sw[1] + th, 1.0f, 1.0f, 0.0f);//left wall
    App::DrawLine(sw[0] + tw, sw[1], sw[0] + tw, sw[1] + th, 1.0f, 1.0f, 0.0f);//right wall
    App::DrawLine(sw[0], sw[1] + th, sw[0] + tw, sw[1] + th, 1.0f, 1.0f, 0.0f);//upper wall
    App::DrawLine(sw[0], sw[1], sw[0] + tw, sw[1], 1.0f, 1.0f, 0.0f);//lower wall
}

void SimpleAIControlledGameObject::SetPath(int * a_pathArray, int a_pathlength, int a_stepsToUse)
{
    m_path.clear();
    for (int i = a_pathlength -1; i> a_pathlength - 1 - a_stepsToUse; i--)
    {
        m_path.push_back(a_pathArray[i]);
    }
}
