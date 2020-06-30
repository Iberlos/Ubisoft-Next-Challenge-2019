#include "stdafx.h"

#include "SimpleScene.h"
#include "SimpleGameObject.h"
#include "SimpleASPathfinder.h"
#include "SimplePlayer.h"
#include "SimpleAIControlledGameObject.h"
#include "..\SimpleTileMap.h"
#include "..\App\app.h"
#include "..\App\SimpleController.h"


static const int MAP_SIZE = 50;//16 Must be bigger than 2; displays result after 4; maximum map size is (value -2)^2; Camera is centered on level so any size is feasable; tested 100 size and generation is still pretty quick.
static const int TUNNEL_LEN = 12;//12 Must be bigger than 1; display results after 1; tunel leght (number of sqaures in a line before forcebly turning, probably) is value-1; higher values increse drastically the chance of stright lines making tows like maps
static const float TUNNEL_FILL_PERCENT = 75;//80 Can be zero; Probably dictates the probablility of a square being generated, on a map of 100 squares, 40% generated 17 squares in average.; 100% created a bug; between 60 and 80 seems to be a good value for a generic area, between 40 and 60 generates good corridors especially with high TUNNEL_LEN;

SimpleScene::SimpleScene()
{
    m_map = new CSimpleTileMap(MAP_SIZE);
    m_paused = false;
}

SimpleScene::~SimpleScene()
{
    for (int i = 0; i < m_gameObjects.size(); i++)
    {
        delete m_gameObjects[i];
    }
    delete m_map;
}

void SimpleScene::Init()
{
    //srand(time(NULL));
    //Init map
    m_map->RandomMap(TUNNEL_FILL_PERCENT, TUNNEL_LEN);
    //Init game objects
    for (int i = 0; i < MAX_CONTROLLERS; i++)
    {
        if (App::GetController(i).IsConnected())
        {
            //Entrance/exit
            SimpleGameObject* exit = new SimpleGameObject(this);
            exit->CreateMesh(1.0f, 1.0f, 1.0f, 1.0f, exit->GetPosition());
            exit->SetName("Exit_" + std::to_string(i));
            exit->AddTag("Exit");
            m_gameObjects.push_back(exit);
            //Player positioned randomly
            SimplePlayer* player = new SimplePlayer(i, this);
            player->SetPlayerStart(exit);
            m_gameObjects.push_back(player);
        }
    }
    //Random candy boxes placed in the world to replesh the players candy if needed
    for (int i = 0; i<5; i++)
    {
        SimpleGameObject* pickup = new SimpleGameObject(this);
        pickup->CreateMesh(1.0f,1.0f,1.0f,0.2f, pickup->GetPosition());
        pickup->SetName("RedCandyPickup_" + std::to_string(i));
        pickup->AddTag("Pickup");
        pickup->AddTag("CandyBox");
        m_gameObjects.push_back(pickup);
    }
    //Test for the AI controlled Gameobject
    SimpleAIControlledGameObject* ai = new SimpleAIControlledGameObject(this);
    m_gameObjects.push_back(ai);

    for (int i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->Init();
    }
}

void SimpleScene::Update(float a_deltaTime)
{
    //Input handling
    for (int i =0; i< MAX_CONTROLLERS; i++)
    {
        if (App::GetController(i).IsConnected())
        {
            if (App::GetController(i).CheckButton(XINPUT_GAMEPAD_START, true))
            {
                m_paused = !m_paused;
            }
            if (App::GetController(i).CheckButton(XINPUT_GAMEPAD_BACK, true))
            {
                Reset();
            }
        }
    }
    if (!m_paused)
    {
        //Update map if needed
        //Update Game objects
        for (int i = 0; i < m_gameObjects.size(); i++)
        {
            m_gameObjects[i]->Update(a_deltaTime);
        }
    }
}

void SimpleScene::Render()
{
    //Render map
    m_map->Render();
    //Render Game Objects
    for (int i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->Render();
    }
}

void SimpleScene::Reset()
{
    m_map->RandomMap(TUNNEL_FILL_PERCENT, TUNNEL_LEN);
    for (int i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->Reset();
    }

    //Deals with GOs removed from the scene.
    m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), nullptr), m_gameObjects.end());
}

void SimpleScene::AddGameObject(SimpleGameObject * a_gameObject)
{
    m_gameObjects.push_back(a_gameObject);
}

bool SimpleScene::RemoveGameObject(SimpleGameObject * a_gameObject)
{
    auto objIterator = std::find(m_gameObjects.begin(), m_gameObjects.end(), a_gameObject);

    if (objIterator != m_gameObjects.end())
    {
        //Its a little dangerous because with enought inatetion could cause memory leaks, but it works.
        m_gameObjects[objIterator- m_gameObjects.begin()] = nullptr;
        return true;
    }
    return false;
}

void SimpleScene::GetAllGOsWithTag(std::string a_tag, std::vector<SimpleGameObject*>* a_vectorToFill)
{
    for (int i = 0; i < m_gameObjects.size(); i++)
    {
        if (m_gameObjects[i]->GameObjectHasTag(a_tag) && m_gameObjects[i]->IsActive())
        {
            (*a_vectorToFill).push_back(m_gameObjects[i]);
        }
    }
}
