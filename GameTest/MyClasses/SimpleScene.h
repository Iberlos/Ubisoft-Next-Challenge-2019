#ifndef _SIMPLESCENE_H_
#define _SIMPLESCENE_H_

class SimpleGameObject;
class CSimpleTileMap;
class SimpleASPathFinder;

class SimpleScene
{
public:
    SimpleScene();
    virtual ~SimpleScene();

    void Init();
    void Update(float a_deltaTime);
    void Render();

    void Reset();

    CSimpleTileMap* GetMap() { return m_map; }
    void GetAllGOsWithTag(std::string a_tag, std::vector<SimpleGameObject*>* a_vectorToFill);

    void SetPathFinder(SimpleASPathFinder* a_pathFinder) { m_pathFinder = a_pathFinder; }
    SimpleASPathFinder* GetPathFinder() { return m_pathFinder; };

    void AddGameObject(SimpleGameObject* a_gameObject);
    bool RemoveGameObject(SimpleGameObject* a_gameObject);
protected:
    SimpleASPathFinder* m_pathFinder;
    CSimpleTileMap* m_map;
    std::vector<SimpleGameObject*> m_gameObjects;
    bool m_paused;
};

#endif