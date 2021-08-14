//================================
//          ASCENCIA CLIENT
//          gamestate.cpp
//================================
//        (c) Phragware 2019
//================================

#include <systems.h>

/*

game state:

name string - eg "start", "north_dungeon_boss"
variables - eg "renderRGB", "int boss_health"
actions - eg "set_render_color", "if(player.attacks) boss_health -= player.dmg"
link to dat files - eg: all "start_x" states get data from "start.dat"
so when leaving "start_x" set of states, auto unload dats

- probably need sub states to run under a main state
eg: main-state: east town
sub-state: blacksmith shop
sub-state: in crafting window etc etc

gamestate identifier:
heirarchy like folder structure
grandparent/parent/child

variables, actions etc: read "state.dat/level.dat"
level.dat: binary file containing variables and actions
eg LoadVar{int}[x]0x1f1f1f1f (but something binary, not ascii)
perhaps a dictionary at the top of the file dictating types and their sizes
probably need to create files semi-manually before ascencia engine up and running
eventually have a level editor or something, maybe separate exe

*/

//================================
//    Constructor
//================================
GameState::GameState(std::string _name, std::string _id)
{
    StateName = _name;
    StateID = _id;

    // load variables/dats
    if(StateName == ASC_APPNAME) return;
    if(StateName == "start") return;

    if(StateID == "TOP") Assets.LoadDat(StateName);
}

//================================
//    Destructor
//================================
GameState::~GameState()
{
    // unload variables/dats

    if(StateID == "TOP") Assets.UnloadDat(StateName);

    std::vector<std::string> vStates;

	if(SubStates.size())
    {
        std::map<std::string, GameState*>::iterator it = SubStates.begin();
        while (it != SubStates.end())
        {
            vStates.push_back(it->first);
            it++;
        }
    }

    if(vStates.size())
    {
        for(int i=0; i<vStates.size(); i++)
        {
            delete SubStates[vStates[i]];
            SubStates.erase(vStates[i]);
        }
    }
}

//================================
//    Run
//================================
void GameState::Run()
{
    ASC_SUBSTATE = StateID;

    if(SubStates.size())
    {
        std::map<std::string, GameState*>::iterator it = SubStates.begin();
        while (it != SubStates.end())
        {
            SubStates[it->first]->Run();
            it++;
        }
    }

    //DEBUG: switch renderer on TAB
    if(ASC_APPSTATE == "start" && ASC_SUBSTATE == "TOP")
    {
        if(Input.Keypress(SDLK_TAB, ASC_ONKEYDOWN))
        {
            if(ASC_RENDERENGINE == "SDL") ASC_RENDERENGINE = "OGL";
            else if(ASC_RENDERENGINE == "OGL") ASC_RENDERENGINE = "SDL";
        }
    }
}

//================================
//    AddState
//================================
void GameState::AddState(std::string _id)
{
    if(SubStates.count(_id)) return;

    std::string newID = StateID;
    newID += "/";
    newID += _id;

    SubStates[_id] = new GameState(ASC_APPSTATE, newID);
}

//================================
//    RemoveState
//================================
void GameState::RemoveState(std::string _id)
{
    if(SubStates.count(_id))
    {
        delete SubStates[_id];
        SubStates.erase(_id);
    }
}