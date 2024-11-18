#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>

enum class Direction { NORTH, SOUTH, EAST, WEST };
enum class ItemType { WEAPON, TOOL, KEY };

struct Item {
    std::string name;
    std::string description;
    ItemType type;

    Item(std::string itemName, std::string itemDesc, ItemType itemType);
};

struct Room {
    std::string name;
    std::string description;
    bool isLocked = false;
    std::string keyItemName;
    Room* connections[4] = { nullptr };
    std::vector<Item> items;

    Room(std::string roomName, std::string roomDesc, bool locked = false, std::string keyItem = "");
};

struct Player {
    Room* currentRoom = nullptr;
    std::vector<Item> inventory;
    bool hasTreasure = false;

    void move(Direction dir);
    void pickUpItem(const std::string& itemName);
    void viewInventory() const;
};

// Function declarations
void connectRooms(Room& room1, Room& room2, Direction dir);
void setupGame(Player& player, std::vector<Room>& rooms);

#endif // GAME_HPP
