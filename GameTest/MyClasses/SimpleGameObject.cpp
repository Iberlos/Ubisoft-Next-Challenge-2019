#include "stdafx.h"

#include "SimpleGameObject.h"
#include "SimpleMesh.h"
#include "SimpleScene.h"
#include "..\app\app.h"
#include "..\SimpleTileMap.h"

SimpleGameObject::SimpleGameObject(SimpleScene* a_scene)
{
    m_pos[0] = 0.0f; m_pos[1] = 0.0f;
    m_linkedPos = m_pos;
    m_mesh = nullptr;
    m_scene = a_scene;
    m_active = true;
    m_poolOfOrigin = nullptr;
}

SimpleGameObject::~SimpleGameObject()
{
    delete m_mesh;
}

void SimpleGameObject::CreateMesh(float a_r, float a_g, float a_b, float a_scale, float * a_posLink)
{
    if (!m_mesh)
    {
        m_mesh = new SimpleMesh(a_r, a_g, a_b, a_scale);
    }
    else
    {
        m_mesh->SetMeshDef(a_r, a_g, a_b, a_scale);
    }
    if (a_posLink)
    {
        m_mesh->LinkPosition(a_posLink);
    }
}

void SimpleGameObject::SetName(std::string a_name)
{
    m_name = a_name;
}

void SimpleGameObject::AddTag(std::string a_tag)
{
    for (int i =0; i<m_tags.size(); i++)
    {
        if (m_tags[i] == a_tag)
        {
            return;
        }
    }
    m_tags.push_back(a_tag);
}

bool SimpleGameObject::GameObjectHasTag(std::string a_tag)
{
    for (int i = 0; i<m_tags.size(); i++)
    {
        if (m_tags[i] == a_tag)
        {
            return true;
        }
    }
    return false;
}

void SimpleGameObject::Update(float a_deltaTime)
{
}

void SimpleGameObject::Render()
{
    if (m_active)
    {
        if (m_mesh)
        {
            m_mesh->Render(m_scene->GetMap());
        }
    }
}

void SimpleGameObject::Init()
{
    if (m_poolOfOrigin)
    {
        SetPosition(-10.0f, -10.0f);
        m_active = false;
    }
    else
    {
        int pitcp[2];
        m_scene->GetMap()->GetRandomTileCoordOfValue(EMapValue::FLOOR, &pitcp[0], &pitcp[1]);
        float piwp[2];
        m_scene->GetMap()->TileCoordToWorld(pitcp[0], pitcp[1], &piwp[0], &piwp[1], true);
        SetPosition(piwp[0], piwp[1]);
        m_active = true;
    }
}

void SimpleGameObject::Reset()
{
    if (m_poolOfOrigin)
    {
        SetPosition(-10.0f, -10.0f);
        m_active = false;
        if (m_scene->RemoveGameObject(this))
        {
            m_poolOfOrigin->ReturnToPool(this);
        }
    }
    else
    {
        int pitcp[2];
        m_scene->GetMap()->GetRandomTileCoordOfValue(EMapValue::FLOOR, &pitcp[0], &pitcp[1]);
        float piwp[2];
        m_scene->GetMap()->TileCoordToWorld(pitcp[0], pitcp[1], &piwp[0], &piwp[1], true);
        SetPosition(piwp[0], piwp[1]);
        m_active = true;
    }
}
