export module ManagerFacade;

import <memory>;
import <unordered_map>;
import <vector>;

import ModelFacade;
import GameStateManager;
import ResourceManager;
import Randomness;
import BoardManager;
import types;
import Goal;
import Criterion;
import Player;
import Board;

export class ManagerFacade {
    // aggregated
    std::shared_ptr<ModelFacade> gameData;
    std::shared_ptr<RandomGenerator> randomizer;
    // composed
    std::unique_ptr<GameStateManager> gameStateManager;
    std::unique_ptr<ResourceManager> resourceManager;
    std::unique_ptr<BoardManager> boardManager;

    public:
        /*
        Uses the provided model (ModelFacade) and randomizer to initialize private fields
        Also initializes the managers (composed)
        */
        ManagerFacade(std::shared_ptr<ModelFacade> data, std::shared_ptr<RandomGenerator> randomizer);
        /*
        Initializes a new game from scratch.
        1. initialAssignments: an unordered map, where the keys are the player colors
        and the values are a list of integer indicating the assignments the players
        want to initially complete
        */
        void initializeNewGame(std::unordered_map<PlayerColor, std::vector<int>> initialAssignments);
        /*
        Attempts to complete the given criterion. Throws an error if not successful
        */
        void completeCriterion(Criterion* criterion, Player* activePlayer);
        /*
        Attempts to achieve the given goal. Throws an error if not successful
        */
        void achieveGoal(Goal* goal, Player* activePlayer);
        /*
        Attempts to improve the given criterion for the active player. Throws an error
        if not successful, with the message indicating what went wrong
        */
        void improveCriterion(Criterion* criterion, Player* activePlayer);
        /*
        Loads a game from the given filename
        */
       void loadGame(std::string filename);
       /*
       Moves the geese tile to the provided geese tile
       */
      void moveGeese(int newTile);
      /*
      Attempts to trade resources with another player. Throws an error if not successful
      */
      void trade(Player* activePlayer, Player* offeringTo, ResourceType give, ResourceType receive);
      /*
      Checks if a player has won the game. Returns the pointer to the player that won.
      Returns nullptr if no player has won yet
      */
      Player* checkWin();
      /*
      Saves the game to the given filename
      */
      void saveGame(std::string filename);
        /*
        Loads a given board from the filename and sets the initial player assignments
        */
       void loadGameWithBoard(std::string filename, std::unordered_map<PlayerColor, std::vector<int>> initAssignments);
       /*
       Awards the provide player with resource based on the given roll,
       and the criterions the player has completed. Returns a map from
       ResourceType to an int indicating the amount they earnt
       */
      std::unordered_map<ResourceType, int> awardResources(int rolled, Player* player);    

      /*
       Removes resources from players with more than 10 total resources (removes half of it),
       floored. Returns a map from players to a map from resource type to an integer
       of how much they lost.
       */
        std::unordered_map<Player*, std::unordered_map<ResourceType, int>> removeResourcesOnGeese(); 
      /*
      Returns a list of players (apart from the activePlayer) who have completed a
      criterion on the provided tile and have non-zero resources
      */
       std::vector<Player*> getStealablePlayers(int tileNum, Player* activePlayer);
       /*
       Steals a resource from the stealingFrom player to the thief player.
       Randomly picks a resource. 
       Requires that stealingFrom has a non-zero number of resources
       Returns the resource that was stolen
       */
       ResourceType stealFromPlayer(Player* stealingFrom, Player* thief);
       /*
       Returns a list of adjacent criterions to the given objective number.
       1. objectiveNumber: the number of the objective
       2. isCriterion: a boolean indicating if the given objective number is a criterion (false if goal)
       */
       std::vector<int> getAdjacentCriterions(int objectiveNumber, bool isCriterion);
      /*
      Returns a list of adjacent goals to the given goal number.
      1. objectiveNumber: the number of the objective (either Criterion or Goal)
      2. isCriterion: a boolean indicating if the given objective number is a criterion (false if goal)
      */
      std::vector<int> getAdjacentGoals(int objectiveNumber, bool isCriterion);
};
