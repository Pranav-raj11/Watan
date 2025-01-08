export module BoardManager;

import <memory>;
import <unordered_map>;
import <vector>;

import types;
import GenericManager;
import ModelFacade;
import Randomness;
import Criterion;
import HexTile;
import Player;
import Goal;
import Board;

export class BoardManager : public GenericManager {
    public:
        BoardManager(std::shared_ptr<ModelFacade> gameData, std::shared_ptr<RandomGenerator> randomizer);
        /*
        Moves the geese to the new tile number
        */
        void moveGeese(int newTileNum);
        /*
        Returns a bool indicating whether a student has completed a criterion in the
        provided tile
        */
        bool hasCriterionInTile(int tile, Player* activePlayer);
        /*
        Returns a list of adjacent criterions to the given criterion number (the 
        criterion numbers are returned, which are adjacent to the given criterion number)
        */
        std::vector<int> getAdjacentCriterionsToCriterion(int criterionNumber);
        /*
        Returns a list of adjacent goals to the given goal number (the goal numbers are returned,
        which are adjacent to the given goal number)
        */
        std::vector<int> getAdjacentGoalsToGoal(int goalNumber);
        /*
        Returns a list of adjacent criterions to the given goal number (the criterion numbers are returned,
        which are adjacent to the given goal number)
        */
        std::vector<int> getAdjacentCriterionsToGoal(int goalNumber);
        /*
        Returns a list of adjacent goals to the given criterion number (the goal numbers are returned,
        which are adjacent to the given criterion number)
        */
        std::vector<int> getAdjacentGoalsToCriterion(int criterionNumber);
    private:
        /*
        Removes duplicates from a vector of ints
        */
        template<typename T>
        std::vector<T> removeDuplicates(std::vector<T> & vec);
        /*
        Gets the index of an element in a vector of ints
        */
        int getIndex(std::vector<int> & vec, int element);
        /*
        Gets relevant adjacent tiles for a given goal number (i.e 
        return a list of tiles that contain adjacent goals)
        */
        std::vector<HexTile*> getAdjacentTilesForGoal(int goalNumber);
};