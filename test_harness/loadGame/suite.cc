
import <memory>;
import <iostream>;

import GameBuilder;
import ModelFacade;
import Randomness;
import GameStateManager;
import BoardView;
import Player;
import types;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide a file to read from, and a random seed (though this is not used here)" << std::endl;
        return 2;
    }
    std::cout << "Starting suite!" << std::endl;
    char* filename = argv[1];
    // use the game state manager to build
    std::shared_ptr<ModelFacade> data{new ModelFacade{}};

    std::shared_ptr<RandomGenerator> randomizer{};

    GameStateManager manager{data, randomizer};
    manager.loadGame(filename); // loads the game

    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "printBoard") {
            BoardView boardView{};
            boardView.printBoard(data->getBoard());
        }
        else if (cmd == "playerStatuses") {
            for (auto playerColor : allPlayerColors()) {
                Player* player = data->getPlayer(playerColor);
                // now just do some printing
                std::cout << "Player " << playerColorToChar(playerColor) << " has ";
                for (auto resource : allResources()) {
                    std::cout << player->getNumResources(resource) << " " << resourceToString(resource) << ",";
                }
                std::cout << std::endl;
            }
        }
        else if (cmd == "currentTurn") {
            std::cout << "It is this player's turn: " << playerColorToChar(data->getTurn()) << std::endl;
        }
    }
}