#include "stdafx.h"

#include "SimplePlayer.h"
#include "SimpleScene.h"
#include "..\app\app.h"
#include "..\SimpleTileMap.h"

SimplePlayer::SimplePlayer(int a_controllerIndex, SimpleScene* a_scene):SimpleControlledGameObject(a_scene)
{
    m_ControllerIndex = a_controllerIndex;
    m_name = "Player_" + std::to_string(a_controllerIndex);
    m_tags.push_back("Player");
    m_tags.push_back("Candyman");
    m_candyStash_RGB[ECandyColors::Red] = 0;
    m_candyStash_RGB[ECandyColors::Green] = 0;
    m_candyStash_RGB[ECandyColors::Blue] = 0;
    m_dropingCandy = ECandyColors::LAST_Candy;

    CreateMesh(0.7f,1.0f,0.7f,0.4f,m_pos);
    m_redCandyPool = new SimplePool<SimpleGameObject*>();
    m_greenCandyPool = new SimplePool<SimpleGameObject*>();
    m_blueCandyPool = new SimplePool<SimpleGameObject*>();
}

SimplePlayer::~SimplePlayer()
{
    delete m_redCandyPool;
    delete m_greenCandyPool;
    delete m_blueCandyPool;
}

void SimplePlayer::Update(float a_deltaTime)
{
    //This player is meant for a pacman style game, movement is continuous and changes when the oportunity arrises.

    //Input handling
    ////Movement
    if(App::GetController(m_ControllerIndex).CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, true))
    {
        m_nextForwardDirection[0] = -1;
        m_nextForwardDirection[1] = 0;
    }
    if (App::GetController(m_ControllerIndex).CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, true))
    {
        m_nextForwardDirection[0] = 1;
        m_nextForwardDirection[1] = 0;
    }
    if (App::GetController(m_ControllerIndex).CheckButton(XINPUT_GAMEPAD_DPAD_UP, true))
    {
        m_nextForwardDirection[0] = 0;
        m_nextForwardDirection[1] = 1;
    }
    if (App::GetController(m_ControllerIndex).CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, true))
    {
        m_nextForwardDirection[0] = 0;
        m_nextForwardDirection[1] = -1;
    }
    //Candy drops
    if (App::GetController(m_ControllerIndex).CheckButton(XINPUT_GAMEPAD_B, true))
    {
        m_dropingCandy = ECandyColors::Red;
    }
    if (App::GetController(m_ControllerIndex).CheckButton(XINPUT_GAMEPAD_A, true))
    {
        m_dropingCandy = ECandyColors::Green;
    }
    if (App::GetController(m_ControllerIndex).CheckButton(XINPUT_GAMEPAD_X, true))
    {
        m_dropingCandy = ECandyColors::Blue;
    }

    //Call super
    SimpleControlledGameObject::Update(a_deltaTime);
}

void SimplePlayer::Init()
{
    //Stop Movement
    m_targetTileCoord[0] = -1;
    m_targetTileCoord[0] = -1;
    m_currentForwardDirection[0] = 0;
    m_currentForwardDirection[1] = 0;
    m_nextForwardDirection[0] = 0;
    m_nextForwardDirection[1] = 0;
    //position the player randomly on the tile map
    SetPosition(m_playerStart->GetPosition()[0], m_playerStart->GetPosition()[1]);
    //Reset candy stash
    m_candyStash_RGB[ECandyColors::Red] = 5;
    m_candyStash_RGB[ECandyColors::Green] = 5;
    m_candyStash_RGB[ECandyColors::Blue] = 5;
    m_dropingCandy = ECandyColors::LAST_Candy;

    //Populate pools
    for (int i = 0; i < 10; i++)
    {
        //red candy
        {
            SimpleGameObject* candy = new SimpleGameObject(m_scene);
            candy->CreateMesh(1.0f, 0.0f, 0.0f, 0.1f, candy->GetPosition());
            candy->SetName("RedCandyPickup_" + std::to_string(i));
            candy->AddTag("Pickup");
            candy->AddTag("Candy");
            candy->AddTag("RedCandy");
            m_redCandyPool->AddNewToPool(candy);
            candy->SetPoolOfOrigin(m_redCandyPool);
        }
        //green candy
        {
            SimpleGameObject* candy = new SimpleGameObject(m_scene);
            candy->CreateMesh(0.0f, 1.0f, 0.0f, 0.1f, candy->GetPosition());
            candy->SetName("GreenCandyPickup_" + std::to_string(i));
            candy->AddTag("Pickup");
            candy->AddTag("Candy");
            candy->AddTag("GreenCandy");
            m_greenCandyPool->AddNewToPool(candy);
            candy->SetPoolOfOrigin(m_greenCandyPool);
        }
        //blue candy
        {
            SimpleGameObject* candy = new SimpleGameObject(m_scene);
            candy->CreateMesh(0.0f, 0.0f, 1.0f, 0.1f, candy->GetPosition());
            candy->SetName("BlueCandyPickup_" + std::to_string(i));
            candy->AddTag("Pickup");
            candy->AddTag("Candy");
            candy->AddTag("BlueCandy");
            m_blueCandyPool->AddNewToPool(candy);
            candy->SetPoolOfOrigin(m_blueCandyPool);
        }
    }
}

void SimplePlayer::Reset()
{
    //Stop Movement
    m_targetTileCoord[0] = -1;
    m_targetTileCoord[0] = -1;
    m_currentForwardDirection[0] = 0;
    m_currentForwardDirection[1] = 0;
    m_nextForwardDirection[0] = 0;
    m_nextForwardDirection[1] = 0;
    //position the player on his start point
    SetPosition(m_playerStart->GetPosition()[0], m_playerStart->GetPosition()[1]);
    //Reset candy stash
    m_candyStash_RGB[ECandyColors::Red] = 5;
    m_candyStash_RGB[ECandyColors::Green] = 5;
    m_candyStash_RGB[ECandyColors::Blue] = 5;
    m_dropingCandy = ECandyColors::LAST_Candy;
}

void SimplePlayer::ReactToPickups()
{
    std::vector<SimpleGameObject*> pickupsInRange;
    GetAllPickupsWithTagInRange("CandyBox", m_reach, &pickupsInRange);

    for (int i = 0; i < pickupsInRange.size(); i++)
    {
        ReplenishCandy();
        pickupsInRange[i]->SetActive(false);
    }
}

void SimplePlayer::ReactToExit()
{
    std::vector<SimpleGameObject*> exitsInRange;
    GetAllPickupsWithTagInRange("Exit", m_reach, &exitsInRange);

    bool endGame = true;
    for (int i = 0; i < ECandyColors::LAST_Candy; i++)
    {
        if (m_candyStash_RGB[i] != 0)
        {
            endGame = false;
        }
    }

    if (exitsInRange.size()!=0 && endGame)
    {
        DebugBreak();
    }
}

void SimplePlayer::ReplenishCandy()
{
    m_candyStash_RGB[ECandyColors::Red] += 1;
    m_candyStash_RGB[ECandyColors::Green] += 1;
    m_candyStash_RGB[ECandyColors::Blue] += 1;
}

void SimplePlayer::DropCandy(ECandyColors a_candyColor)
{
    if (m_candyStash_RGB[a_candyColor]>0)
    {
        m_candyStash_RGB[a_candyColor] -= 1;
        SimpleGameObject* candy = nullptr;
        switch (a_candyColor)
        {
        case ECandyColors::Red:
        {
            candy = m_redCandyPool->GetFromPool();
            break;
        }
        case ECandyColors::Green:
        {
            candy = m_greenCandyPool->GetFromPool();
            break;
        }
        case ECandyColors::Blue:
        {
            candy = m_blueCandyPool->GetFromPool();
            break;
        }
        }
        if (candy)
        {
            candy->SetActive(true);
            candy->SetPosition(m_pos[0], m_pos[1]);
            m_scene->AddGameObject(candy);
        }
    }
    m_dropingCandy = ECandyColors::LAST_Candy;
}

void SimplePlayer::TargetTileReached()
{
    //CheckForOtherGos
    ReactToPickups();
    ReactToExit();

    //Drop candy
    if (m_dropingCandy != ECandyColors::LAST_Candy)
    {
        DropCandy(m_dropingCandy);
    }
}
