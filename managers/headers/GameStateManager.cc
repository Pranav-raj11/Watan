export module GameStateManager;

import <memory>;
import <fstream>;
import <unordered_map>;
import <vector>;

import GenericManager;
import GameBuilder;
import ModelFacade;
import Randomness;
import Board;
import Criterion;
import Objective;
import Goal;
import Player;
import types;
import HexTile;

export class GameStateManager: public GenericManager {
    public:

        GameStateManager(std::shared_ptr<ModelFacade> data, std::shared_ptr<RandomGenerator> random);

        void saveGame(std::string filename);

        void loadGame(std::string filename);

        /*
        Loads the specified board from the given file, along with the player's 
        initial chosen assignments
        1. filename: the filename to get the board
        2. initialAssignments: a map from player color to the corresponding list of 
        assignments the player chose to complete at the start of the game
        */
        void loadBoard(std::string filename, std::unordered_map<PlayerColor, std::vector<int>> initAssignments);

        /*
        Return a player* if a player has won the game. Returns nullptr if no player has won yet
        */
        Player* checkWin();
        /*
        Initializes a new game with the given initial assignments
        1. initAssignments: a map from player color to the corresponding list of 
        assignments the player chose to complete at the start of the game
        */  
        void initializeNewGame(std::unordered_map<PlayerColor, std::vector<int>> initAssignments);

};
