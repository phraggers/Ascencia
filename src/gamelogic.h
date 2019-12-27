//================================
//          ASCENCIA CLIENT
//          gamelogic.h
//================================
//        (c) Phragware 2019
//================================

#ifndef ASC_CLIENT_GAMELOGIC_H_
#define ASC_CLIENT_GAMELOGIC_H_

#include <headers.h>

struct GameLogic
{
    GameLogic();
    ~GameLogic();

    void Run();
    void SetState(std::string _state);
    void FrameRate();
    void HandleEvents();
    void SetWindowTitle();
    void RenderPreFrame();
    void RenderFrame();
    void RenderPostFrame();
    void CheckNewState();

    bool ChangeState;
    std::string NewState;
    std::map<std::string, GameState*> States;
};

#endif // ASC_CLIENT_GAMELOGIC_H_