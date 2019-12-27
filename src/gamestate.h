//================================
//          ASCENCIA CLIENT
//          gamestate.h
//================================
//        (c) Phragware 2019
//================================

#ifndef ASC_CLIENT_GAMESTATE_H_
#define ASC_CLIENT_GAMESTATE_H_

#include <headers.h>

// a GameState is a container holding a set of variables and
// a list of instructions for the current AppState
// Loads StateName.dat on construction
// Unloads StateName.dat on destruction

struct GameState
{
    GameState(std::string _name, std::string _id);
    ~GameState();
    void Run();
    void AddState(std::string _id);
    void RemoveState(std::string _id);

    std::string StateName;
    std::string StateID;
    std::map<std::string, GameState*> SubStates;
};

#endif // ASC_CLIENT_GAMESTATE_H_