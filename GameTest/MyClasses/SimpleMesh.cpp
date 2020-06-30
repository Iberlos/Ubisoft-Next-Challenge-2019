#include "stdafx.h"
#include "SimpleMesh.h"
#include "..\app\app.h"
#include "..\SimpleTileMap.h"

SimpleMesh::SimpleMesh(float r, float g, float b, float scale)
{
    m_pos[0] = 0.0f; m_pos[1] = 0.0f;
    m_anchor[0] = 0.5f; m_anchor[1] = 0.5f;
    m_linkedPos = m_pos;
    SetMeshDef(r,g,b,scale);
}

void SimpleMesh::GetRectDim(CSimpleTileMap* a_linkedTileMap, float * a_llvx, float * a_llvy, float * a_urvx, float * a_urvy)
{
    float tileWidth = a_linkedTileMap->GetTileWidth();
    float tileHeight = a_linkedTileMap->GetTileHeight();

    float w = tileWidth * m_MeshDef[3];
    float h = tileHeight * m_MeshDef[3];

    *a_llvx = -w * (1 - m_anchor[0]); 
    *a_llvy = -h * (1 - m_anchor[1]);
    *a_urvx = +w * m_anchor[0];
    *a_urvy = +h * m_anchor[1];
}

void SimpleMesh::Render(CSimpleTileMap* a_linkedTileMap)
{
    //Draw this object in the world based on the scale of the tile map and centered using the anchorpoint
    float tileWidth = a_linkedTileMap->GetTileWidth();
    float tileHeight = a_linkedTileMap->GetTileHeight();

    float w = tileWidth * m_MeshDef[3];
    float h = tileHeight * m_MeshDef[3];

    float llv[2];
    float urv[2];
    GetRectDim(a_linkedTileMap, &llv[0], &llv[1], &urv[0], &urv[1]);
    llv[0] += GetPosition()[0];
    llv[1] += GetPosition()[1];
    urv[0] += GetPosition()[0];
    urv[1] += GetPosition()[1];

    App::DrawQuad(llv[0], llv[1], urv[0], urv[1], m_MeshDef[0], m_MeshDef[1], m_MeshDef[2]);
}
