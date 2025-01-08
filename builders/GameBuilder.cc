export module GameBuilder;

import <memory>;
import <fstream>;
import <sstream>;
import <unordered_map>;
import <vector>;

import Randomness;
import ModelFacade;
import Goal;
import Criterion;
import Player;
import types;
import Board;
import HexTile;
import Dice;
import FairDice;
import Student;

export class GameBuilder {
    std::shared_ptr<ModelFacade> database;
    std::shared_ptr<RandomGenerator> randomizer;

    std::vector<Goal*> goals;
    std::vector<Criterion*> crit;

    public:
        /*
        Constructs the game builder.
        1. db: A ModelFacade that points to the model data being used by everyone else
        2. randomizer: A RandomGenerator that points to the seeded randomizer 
        */
        GameBuilder(std::shared_ptr<ModelFacade> db, std::shared_ptr<RandomGenerator> randomizer);

        /*
        Builds the board from scratch. Creates HexTiles, goals, and criterions for the board.
        Uses RandomGenerator to add randomness
        */
        void buildBoard();


        /*
        Builds the board by loading the board from the provided std::ifstream. The format for loading the data is provided in Documentation/watan.pdf
        */
        void buildBoard(std::ifstream & ifs);

        /*
        Builds the players using the provided initial assignments
        1. Keys are the player color, and the values are the corresponding
        vector providing a list of integers for all initial assignments
        */
        void buildPlayers(std::unordered_map<PlayerColor, std::vector<int>> initAssignments);

        /*
        Loads player data from the provided std::ifstream, and adds it
        in the game data (model).
        Also initializes the private fields (goals and crit). Adds any objective the player completed into these
        */
        void buildPlayers(std::ifstream & ifs);

        /*
        Sets the current turn to be the i'th player turn. Takes mod 
        if necassary
        */
        void setTurn(int turn);

    private:
        /*
        Loads the criterions from the provided std::istringstream.
        Also adds the completed criterion to the player
        */
        void loadCriterions(std::vector<Criterion*> & ref, std::istringstream & ifs, Player* current);

        void loadGoals(std::vector<Goal*> & ref, std::istringstream &  ifs, Player* current);

        /*
        Sets up HexTile to include criterions and goals located on this tile
        Requires that the tiles are ordered by the tile number
        - Also sorts the criterion and goal numbers in the tiles so they follow the circular order
        */
        void setObjectiveParents(std::vector<HexTile*> tiles);

        void addObjectiveParents(int & goalCount, int & critCount, int absColumnNumber, int absRowNumber, std::vector<HexTile*> activeTiles, std::vector<int> rowIndexByColum, HexTile* currTile, std::vector<HexTile*> allTiles);

        /*
        Sorts the criterion and goal numbers in the given tiles so that they are in circular order
        */
        void sortHexTiles(std::vector<HexTile*> tiles);

};
