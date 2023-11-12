#include <iostream>
#include "CommandManager.h"

int main() {
    CommandManager commandManager;

    std::string command;
    while (std::cin >> command) {
        commandManager.execute(command);
    }
    return 0;
}