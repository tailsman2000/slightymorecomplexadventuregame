#include "game.hpp"
#include <iostream>
#include <string>

int main() {
    Player player;
    std::vector<Room> rooms;
    setupGame(player, rooms);

    std::cout << "Welcome to the adventure game!\n";
    std::cout << "Your goal is to collect the treasure and escape through the Exit.\n";
    std::cout << "Type commands like 'move north', 'pick [item]', or 'inventory'.\n";

    while (true) {
        std::cout << "\n> ";
        std::string command;
        std::getline(std::cin, command);

        if (command == "quit") {
            break;
        } else if (command == "inventory") {
            player.viewInventory();
        } else if (command.substr(0, 5) == "move ") {  // Fixed here
            std::string dir = command.substr(5);
            if (dir == "north") player.move(Direction::NORTH);
            else if (dir == "south") player.move(Direction::SOUTH);
            else if (dir == "east") player.move(Direction::EAST);
            else if (dir == "west") player.move(Direction::WEST);
            else std::cout << "Invalid direction.\n";
        } else if (command.substr(0, 5) == "pick ") {  // Fixed here
            player.pickUpItem(command.substr(5));
        } else {
            std::cout << "Unknown command.\n";
        }
    }

    return 0;
}
