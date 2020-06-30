#ifndef _SIMPLEGAMEOBJECT_H_
#define _SIMPLEGAMEOBJECT_H_

#include "SimplePool.h"

class SimpleMesh;
class SimpleScene;

class SimpleGameObject
{
public:
    SimpleGameObject(SimpleScene* a_scene);
    virtual ~SimpleGameObject();
    virtual void Update(float a_deltaTime);
    virtual void Render();

    virtual void Init();
    virtual void Reset();

    void SetActive(bool a_state) { m_active = a_state; }

    void CreateMesh(float a_r, float a_g, float a_b, float a_scale, float* a_posLink = nullptr);

    void Translate(float a_dx, float a_dy) { m_pos[0] += a_dx; m_pos[1] += a_dy; }
    void SetPosition(float a_x, float a_y) { m_pos[0] = a_x; m_pos[1] = a_y; }
    float* GetPosition() { return m_linkedPos; }
    void LinkPosition(float* a_pos) { m_linkedPos = a_pos; }

    void SetName(std::string a_name);

    SimpleScene* GetScene() { return m_scene; }

    bool IsActive() { return m_active; }

    void AddTag(std::string a_tag);
    bool GameObjectHasTag(std::string a_tag);

    void SetPoolOfOrigin(SimplePool<SimpleGameObject*>* a_poolOfOrigin) { m_poolOfOrigin = a_poolOfOrigin; }
    SimplePool<SimpleGameObject*>* GetPoolOfOrigin() { return m_poolOfOrigin; }

protected:
    float m_pos[2];
    float* m_linkedPos;
    SimpleMesh* m_mesh;
    SimpleScene* m_scene;
    std::vector<std::string> m_tags;
    std::string m_name;
    bool m_active;
    SimplePool<SimpleGameObject*>* m_poolOfOrigin;
};

#endif