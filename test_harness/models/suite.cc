import <iostream>;
import <vector>;
import <string>;
import <sstream>;

import WeightedDice;
import Player;
import Dice;
import Board;
import Randomness;
import ModelFacade;
import types;
import HexTile;
import Goal;
import Criterion;
import Student;
import FairDice;

/*
Converts string to PlayerColor enum
Arguments:
    color: "BLUE", "RED", "ORANGE", or "YELLOW"
Returns:
    Corresponding PlayerColor enum value
*/
PlayerColor stringToColor(const std::string& color) {
    if (color == "BLUE") return PlayerColor::BLUE;
    if (color == "RED") return PlayerColor::RED;
    if (color == "ORANGE") return PlayerColor::ORANGE;
    if (color == "YELLOW") return PlayerColor::YELLOW;
    return PlayerColor::BLUE; // default
}

/*
Converts string to ResourceType enum
Arguments:
    resource: "CAFFEINE", "LAB", "LECTURE", "STUDY", or "TUTORIAL"
Returns:
    Corresponding ResourceType enum value
*/
ResourceType stringToResource(const std::string& resource) {
    if (resource == "CAFFEINE") return ResourceType::CAFFEINE;
    if (resource == "LAB") return ResourceType::LAB;
    if (resource == "LECTURE") return ResourceType::LECTURE;
    if (resource == "STUDY") return ResourceType::STUDY;
    if (resource == "TUTORIAL") return ResourceType::TUTORIAL;
    return ResourceType::CAFFEINE; // default
}

/*
Possible commands with the test harness:
    1. create_player <color>: Creates new player with specified color (BLUE/RED/ORANGE/YELLOW)
    2. print_player_status <color>: Displays resources and VP for player of given color
    3. add_resource <color> <resource_type> <amount>: Adds resources to player
        resource_type can be: CAFFEINE/LAB/LECTURE/STUDY/TUTORIAL
    4. roll_dice <color>: Rolls dice for specified player
    5. create_board <num_tiles>: Creates new board with specified number of tiles
    6. add_goal <number>: Creates new goal with given number
    7. add_criterion <number>: Creates new criterion with given number
    8. complete_objective <type> <number> <color>: Completes objective for player
        type can be: goal/criterion
    9. help: Shows this help message
    10. exit: Exits program
*/
void printCommands() {
    std::cout << "\nAvailable Commands:\n";
    std::cout << "1. create_player <color>\n";
    std::cout << "2. print_player_status <color>\n";
    std::cout << "3. add_resource <color> <resource_type> <amount>\n";
    std::cout << "4. roll_dice <color>\n";
    std::cout << "5. create_board <num_tiles>\n";
    std::cout << "6. add_goal <number>\n";
    std::cout << "7. add_criterion <number>\n";
    std::cout << "8. complete_objective <goal/criterion> <number> <player_color>\n";
    std::cout << "9. move_geese <tile_number>\n";
    std::cout << "10. help\n";
    std::cout << "11. exit\n";
}

int main(int argc, char* argv[]) {
    int seedNum;
    RandomGenerator randomizer;
    if (argc > 1) {
        std::istringstream iss{argv[1]}; //seed num
        iss >> seedNum;
        randomizer.setSeed(seedNum);
    }

    // Initialize test harness state
    std::string command;
    std::vector<Player*> players;
    std::vector<Goal*> goals;
    std::vector<Criterion*> criterions;
    Board* board = nullptr;
    
    std::cout << "Test Harness Started. Type 'help' for commands.\n";

    // Main command loop
    while (std::getline(std::cin, command)) {
        std::stringstream ss(command);
        std::string cmd;
        ss >> cmd;

        if (cmd == "exit") {
            break;
        }
        else if (cmd == "help") {
            printCommands();
        }
        else if (cmd == "create_player") {
            std::string color;
            ss >> color;
            auto* dice = new FairDice(&randomizer);
            players.push_back(new Student(stringToColor(color), std::vector<int>{}, std::vector<int>{}, dice));
            std::cout << "Created player with color " << color << "\n";
        }
        else if (cmd == "print_player_status") {
            std::string color;
            ss >> color;
            for (auto p : players) {
                if (p->getColor() == stringToColor(color)) {
                    std::cout << "Player status:\n";
                    std::cout << "VP: " << p->getNumVictoryPoints() << "\n";
                    std::cout << "Resources:\n";
                    std::cout << "  Caffeine: " << p->getNumResources(ResourceType::CAFFEINE) << "\n";
                    std::cout << "  Lecture: " << p->getNumResources(ResourceType::LECTURE) << "\n";
                    std::cout << "  Lab: " << p->getNumResources(ResourceType::LAB) << "\n";
                    std::cout << "  Study: " << p->getNumResources(ResourceType::STUDY) << "\n";
                    std::cout << "  Tutorial: " << p->getNumResources(ResourceType::TUTORIAL) << "\n";
                    break;
                }
            }
        }
        else if (cmd == "add_resource") {
            std::string color, resource_type;
            int amount;
            ss >> color >> resource_type >> amount;
            
            for (auto p : players) {
                if (p->getColor() == stringToColor(color)) {
                    p->incrementResource(stringToResource(resource_type), amount);
                    std::cout << "Added " << amount << " " << resource_type << " to player " << color << "\n";
                    break;
                }
            }
        }
        else if (cmd == "roll_dice") {
            std::string color;
            ss >> color;
            
            for (auto p : players) {
                if (p->getColor() == stringToColor(color)) {
                    int result = p->getCurrentDice()->roll();
                    std::cout << "Player " << color << " rolled: " << result << "\n";
                    break;
                }
            }
        }
        else if (cmd == "create_board") {
            int num_tiles;
            ss >> num_tiles;
            
            // Clean up old board if it exists
            delete board;
            
            std::vector<HexTile*> tiles;
            // Create simple tiles for testing
            for (int i = 0; i < num_tiles; i++) {
                std::vector<Goal*> tileGoals;
                std::vector<Criterion*> tileCrits;
                ResourceType resources[] = {ResourceType::CAFFEINE, ResourceType::LECTURE, ResourceType::LAB, ResourceType::STUDY, ResourceType::TUTORIAL};
                int randomResource = randomizer.getRandom(0, 4);
                int randomRollNumber = randomizer.getRandom(1, 6) + randomizer.getRandom(1, 6);
                tiles.push_back(new HexTile(resources[randomResource], i, randomRollNumber, tileGoals, tileCrits));
            }
            
            board = new Board(-1, tiles); // -1 means no geese initially
            std::cout << "Created board with " << num_tiles << " tiles\n";
            
            // Print status of each tile
            for (int i = 0; i < num_tiles; i++) {
                HexTile* tile = board->getTileAt(i);
                std::cout << "\nTile " << i << " status:\n";
                std::cout << "Resource type: " << tile->getTileResource() << "\n";
                std::cout << "Roll number: " << tile->getTileValue() << "\n";
                std::cout << "Tile Number: " << tile->getTileNumber() << "\n";
                
                std::cout << "Goals: ";
                for (auto goal : tile->getGoals()) {
                    std::cout << goal->getObjectiveNumber() << " ";
                }
                std::cout << "\n";
                
                std::cout << "Criteria: ";
                for (auto criterion : tile->getCriterions()) {
                    std::cout << criterion->getObjectiveNumber() << " ";
                }
                std::cout << "\n";
            }
            
            // Print geese location
            int geeseLocation = board->getGeeseTile();
            std::cout << "\nGeese location: " << (geeseLocation == -1 ? "No geese" : std::to_string(geeseLocation)) << "\n";
        }
        else if (cmd == "add_goal") {
            int number;
            ss >> number;
            
            goals.push_back(new Goal(number));
            std::cout << "Added goal with number " << number << "\n";
        }
        else if (cmd == "add_criterion") {
            int number;
            ss >> number;
            
            criterions.push_back(new Criterion(number));
            std::cout << "Added criterion with number " << number << "\n";
        }
        else if (cmd == "complete_objective") {
            std::string type, color;
            int number;
            ss >> type >> number >> color;
            
            Player* targetPlayer = nullptr;
            for (auto p : players) {
                if (p->getColor() == stringToColor(color)) {
                    targetPlayer = p;
                    break;
                }
            }
            
            if (targetPlayer) {
                if (type == "goal") {
                    for (auto g : goals) {
                        if (g->getObjectiveNumber() == number) {
                            g->complete(targetPlayer);
                            std::cout << "Completed goal " << number << " for player " << color << "\n";
                            break;
                        }
                    }
                }
                else if (type == "criterion") {
                    for (auto c : criterions) {
                        if (c->getObjectiveNumber() == number) {
                            c->complete(targetPlayer);
                            std::cout << "Completed criterion " << number << " for player " << color << "\n";
                            break;
                        }
                    }
                }
            }
        }
        else if (cmd == "move_geese") {
            int tile_number;
            ss >> tile_number;
            board->moveGeese(tile_number);
            std::cout << "Moved geese to tile " << board->getGeeseTile() << "\n";
        }
    }

    // Cleanup allocated memory
    delete board;
    for (auto p : players) delete p;
    for (auto g : goals) delete g;
    for (auto c : criterions) delete c;

    return 0;
}