#ifndef _SIMPLEPLAYER_H_
#define _SIMPLEPLAYER_H_

#include "SimpleControlledGameObject.h"

class SimpleMesh;
class SimpleScene;

enum ECandyColors
{
    Red = 0,
    Green,
    Blue,
    LAST_Candy
};

class SimplePlayer: public SimpleControlledGameObject
{
public:
    SimplePlayer(int a_controllerIndex, SimpleScene* a_scene);
    virtual ~SimplePlayer();

    void Update(float a_deltaTime) override;

    void Init() override;
    void Reset() override;

    void SetPlayerStart(SimpleGameObject* a_playerStart) { m_playerStart = a_playerStart; }

    void ReactToPickups();
    void ReactToExit();
    void ReplenishCandy();
    void DropCandy(ECandyColors a_candyColor);

    virtual void TargetTileReached() override;
protected:
    int m_ControllerIndex;
    int m_candyStash_RGB[ECandyColors::LAST_Candy];
    ECandyColors m_dropingCandy;
    SimpleGameObject* m_playerStart;
    SimplePool<SimpleGameObject*>* m_redCandyPool;
    SimplePool<SimpleGameObject*>* m_greenCandyPool;
    SimplePool<SimpleGameObject*>* m_blueCandyPool;
};

#endif