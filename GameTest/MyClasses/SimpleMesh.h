#ifndef _SIMPLEMESH_H_
#define _SIMPLEMESH_H_

class CSimpleTileMap;

class SimpleMesh
{
public:
    SimpleMesh(float a_r, float a_g, float a_b, float a_scale);

    void SetMeshDef(float a_r, float a_g, float a_b, float a_scale) { m_MeshDef[0] = a_r; m_MeshDef[1] = a_g; m_MeshDef[2] = a_b; m_MeshDef[3] = a_scale;}
    void SetPosition(float a_x, float a_y) { m_pos[0] = a_x; m_pos[1] = a_y; }
    float* GetPosition() { return m_linkedPos; }
    void LinkPosition(float* a_pos) { m_linkedPos = a_pos; }
    void SetAnchor(float a_ax, float a_ay) { m_anchor[0] = a_ax; m_anchor[1] = a_ay; }
    void GetRectDim(CSimpleTileMap* a_linkedTileMap, float* a_llvx, float* a_llvy, float* a_urvx, float* a_urvy);

    void Render(CSimpleTileMap* a_linkedTileMap);
protected:
    float m_pos[2];
    float* m_linkedPos;
    float m_anchor[2];
    float m_MeshDef[4];
};

#endif