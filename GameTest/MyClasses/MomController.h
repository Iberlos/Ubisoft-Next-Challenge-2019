#ifndef _MOMCONTROLLER_H_
#define _MOMCONTROLLER_H_

#include "SimpleAIController.h"

class MomController : public SimpleAIController
{
public:
    MomController(SimpleAIControlledGameObject* a_controlledGO);
    ~MomController();
protected:
    //functions called during state change may or may not do mechanic related things, but should mostly be used to signal the begining of a state
    virtual void Enter_Idle();
    virtual void Enter_Follow();
    virtual void Enter_Wait();
    virtual void Enter_Home();
    virtual void Enter_Bully();
    virtual void Enter_Bullied();

    //functions called in update for each state (Not too much will be in these functions. Overlaps are handled only on tile reached.)
    virtual void Update_Idle(float a_deltaTime);
    virtual void Update_Follow(float a_deltaTime);
    virtual void Update_Wait(float a_deltaTime);
    virtual void Update_Home(float a_deltaTime);
    virtual void Update_Bully(float a_deltaTime);
    virtual void Update_Bullied(float a_deltaTime);

    //functions called when the end of the path is reached (called it step because it is like updating the AI behaviour, but not sincronos with update)
    virtual void Step_Idle();
    virtual void Step_Follow();
    virtual void Step_Wait();
    virtual void Step_Home();
    virtual void Step_Bully();
    virtual void Step_Bullied();

    //Functions called when a tile is reached, would do things like checking for pickups or contact with other controlled GOs
    virtual void TileReached_Idle();
    virtual void TileReached_Follow();
    virtual void TileReached_Wait();
    virtual void TileReached_Home();
    virtual void TileReached_Bully();
    virtual void TileReached_Bullied();

    //functions called during state change may or may not do mechanic related things, but should mostly be used to signal the end of a state
    virtual void Exit_Idle();
    virtual void Exit_Follow();
    virtual void Exit_Wait();
    virtual void Exit_Home();
    virtual void Exit_Bully();
    virtual void Exit_Bullied();
};

#endif