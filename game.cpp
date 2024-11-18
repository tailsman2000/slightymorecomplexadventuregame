#include "game.hpp"
#include <iostream>
#include <algorithm>

// Item Constructor
Item::Item(std::string itemName, std::string itemDesc, ItemType itemType)
    : name(std::move(itemName)), description(std::move(itemDesc)), type(itemType) {}

// Room Constructor
Room::Room(std::string roomName, std::string roomDesc, bool locked, std::string keyItem)
    : name(std::move(roomName)), description(std::move(roomDesc)), isLocked(locked), keyItemName(std::move(keyItem)) {}

// Player Movement
void Player::move(Direction dir) {
    Room* nextRoom = currentRoom->connections[static_cast<int>(dir)];
    if (!nextRoom) {
        std::cout << "You can't move in that direction.\n";
        return;
    }

    if (nextRoom->isLocked) {
        auto it = std::find_if(inventory.begin(), inventory.end(),
                               [&nextRoom](const Item& item) { return item.name == nextRoom->keyItemName; });
        if (it != inventory.end()) {
            std::cout << "You used " << it->name << " to unlock the door.\n";
            nextRoom->isLocked = false;
        } else {
            std::cout << "The door is locked. You need " << nextRoom->keyItemName << " to open it.\n";
            return;
        }
    }

    currentRoom = nextRoom;
    std::cout << "You moved to " << currentRoom->name << ".\n";
    std::cout << currentRoom->description << "\n";

    // Endgame check
    if (currentRoom->name == "Exit" && hasTreasure) {
        std::cout << "Congratulations! You've escaped with the treasure. You win!\n";
        exit(0);
    }
}

// Pick up Item
void Player::pickUpItem(const std::string& itemName) {
    auto it = std::find_if(currentRoom->items.begin(), currentRoom->items.end(),
                           [&itemName](const Item& item) { return item.name == itemName; });

    if (it != currentRoom->items.end()) {
        if (it->name == "Treasure") hasTreasure = true;
        inventory.push_back(*it);
        currentRoom->items.erase(it);
        std::cout << "Picked up: " << itemName << "\n";
    } else {
        std::cout << "Item not found in this room.\n";
    }
}

// View Inventory
void Player::viewInventory() const {
    if (inventory.empty()) {
        std::cout << "Your inventory is empty.\n";
        return;
    }

    std::cout << "Inventory:\n";
    for (const auto& item : inventory) {
        std::cout << "- " << item.name << ": " << item.description << "\n";
    }
}

// Connect Rooms
void connectRooms(Room& room1, Room& room2, Direction dir) {
    room1.connections[static_cast<int>(dir)] = &room2;
    room2.connections[static_cast<int>((static_cast<int>(dir) + 2) % 4)] = &room1;
}

// Setup Game World
void setupGame(Player& player, std::vector<Room>& rooms) {
    // Create rooms
    Room hall("Hall", "A large, empty hall with a grand chandelier.");
    Room library("Library", "A quiet room filled with dusty old books.", true, "Library Key");
    Room vault("Vault", "A secure vault with a shimmering treasure.", true, "Vault Key");
    Room kitchen("Kitchen", "A kitchen filled with rusty cooking tools.");
    Room garden("Garden", "A beautiful garden with blooming flowers.", true, "Garden Key");
    Room basement("Basement", "A dark, damp basement.", true, "Basement Key");
    Room exit("Exit", "The final exit. Escape with the treasure to win!");

    // Connect rooms
    connectRooms(hall, library, Direction::NORTH);
    connectRooms(library, vault, Direction::EAST);
    connectRooms(hall, kitchen, Direction::EAST);
    connectRooms(kitchen, garden, Direction::NORTH);
    connectRooms(hall, basement, Direction::SOUTH);
    connectRooms(garden, exit, Direction::EAST);

    // Add items
    hall.items.push_back(Item("Library Key", "A key to the library door.", ItemType::KEY));
    kitchen.items.push_back(Item("Garden Key", "A key to the garden door.", ItemType::KEY));
    basement.items.push_back(Item("Vault Key", "A key to the vault door.", ItemType::KEY));
    vault.items.push_back(Item("Treasure", "A priceless treasure sparkling with gold and jewels.", ItemType::TOOL));

    // Add rooms to the list
    rooms = { hall, library, vault, kitchen, garden, basement, exit };

    // Place player in starting room
    player.currentRoom = &rooms[0]; // Hall
}
