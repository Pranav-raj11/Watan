import <memory>;
import <iostream>;
import <vector>;
import <string>;
import <sstream>;

import GameStateManager;
import ModelFacade;
import Randomness;
import Board;
import HexTile;

std::vector<int> readVector() {
    int inp;
    std::string s;
    getline(std::cin, s);
    std::istringstream iss{s};
    std::vector<int> criterions{};
    while (iss >> inp) {
        criterions.push_back(inp);        
    }
    return criterions;
}

void printVector(std::vector<HexTile*> vec) {
    for (auto i : vec) {
        std::cout << i->getTileNumber() << ", ";
    }
}

int main() {
    int SEED = 1000;
    std::string gameFilename = "board.txt";
    std::shared_ptr<ModelFacade> data{new ModelFacade{}};
    std::shared_ptr<RandomGenerator> randomizer{new RandomGenerator{SEED}};
    GameStateManager manager{data, randomizer};

    manager.loadGame(gameFilename);
    // now check that the objectives have correct parents
    // first read criterions
    std::vector<int> crits = readVector();
    std::vector<int> goals = readVector();
    // loop through goals and print its parents
    for (int critNum : crits) {
        std::cout << "Criteria number " << critNum << " has the following parents:" << std::endl;
        std::vector<HexTile*> parents = data->getBoard()->getCriterionByNumber(critNum)->getHexTileParents();
        printVector(parents);
        std::cout << std::endl;
    }
    std::cout << "---------------------" << std::endl;
    // loop through goals
    for (int goalNum : goals) {
        std::cout << "Goal number " << goalNum << " has the following parents:" << std::endl;
        std::vector<HexTile*> parents = data->getBoard()->getGoalByNumber(goalNum)->getHexTileParents();
        printVector(parents);
        std::cout << std::endl;
    }
}