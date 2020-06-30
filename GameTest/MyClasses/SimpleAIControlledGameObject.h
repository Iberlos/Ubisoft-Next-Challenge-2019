#ifndef _SIMPLEAICONTROLLEDGAMEOBJECT_H_
#define _SIMPLEAICONTROLLEDGAMEOBJECT_H_

#include "SimpleControlledGameObject.h"

class SimpleAIController;

enum EControllers
{
    Black_Controller = 0,
    Red_Controller,
    Green_Controller,
    Blue_Controller,
    Cyan_Controller,
    Magenta_Controller,
    Yellow_Controller,
    White_Controller,
    Mom_Controller,
    LAST_Controller
};

class SimpleAIControlledGameObject : public SimpleControlledGameObject
{
public:
    SimpleAIControlledGameObject(SimpleScene* a_scene);
    virtual ~SimpleAIControlledGameObject();

    virtual void Update(float a_deltaTime) override;
    virtual void Render() override;
    virtual void Reset() override;
    virtual void TargetTileReached() override;
    void RenderPath();
    void RenderTarget();
    void RenderSightAsRectangle();
    void SetPath(int* a_pathArray, int a_pathlength, int a_stepsToUse);
protected:
    std::vector<SimpleAIController*> m_availableControllers;
    std::vector<int> m_path;
    EControllers m_currentController;
};

#endif