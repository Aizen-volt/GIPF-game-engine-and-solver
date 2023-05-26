#ifndef GIPF_COMMANDMANAGER_H
#define GIPF_COMMANDMANAGER_H


#include "Game.h"

class CommandManager {
private:
    Game* game;

    void CreateGame();
public:
    void DispatchCommand();
};


#endif //GIPF_COMMANDMANAGER_H
