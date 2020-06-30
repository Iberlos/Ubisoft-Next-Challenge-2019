#ifndef _SIMPLECONTROLLEDGAMEOBJECT_H_
#define _SIMPLECONTROLLEDGAMEOBJECT_H_

#include "SimpleGameObject.h"

class SimpleControlledGameObject: public SimpleGameObject
{
public:
    SimpleControlledGameObject(SimpleScene* a_scene);
    virtual ~SimpleControlledGameObject();

    virtual void Update(float a_deltaTime);

    void StopMovement();

    void GetAllPickupsWithTagInRange(std::string a_tag, float a_range, std::vector<SimpleGameObject*>* a_vectorToFill);
    SimpleGameObject* GetClosestPickupWithTagInRange(std::string a_tag, float a_range, std::vector<SimpleGameObject*>* a_vectorToFill = nullptr);
    virtual void TargetTileReached();
protected:
    float m_speed;
    float m_reach;
    int m_targetTileCoord[2];
    int m_currentForwardDirection[2];
    int m_nextForwardDirection[2];
};

#endif