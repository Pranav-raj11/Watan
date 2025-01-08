export module GameController;

import <memory>;
import <string>;
import <unordered_map>;
import <vector>;
import <algorithm>;
import <iostream>;

import ModelFacade;
import Randomness;
import ManagerFacade;
import ViewProxy;
import types;
import Dice;
import Player;
import FairDice;
import WeightedDice;
import Goal;
import Criterion;


export class GameController {
    std::shared_ptr<ModelFacade> gameData;
    std::shared_ptr<RandomGenerator> randomizer;

    std::unique_ptr<ManagerFacade> gameManager;
    std::unique_ptr<ViewProxy> display;
    // stores board filename and game filename
    std::string boardFile;
    std::string gameFile;

    public:
        friend class TestHarness;
        /*
        ctor without seeding the randomness
        */
        GameController();
        /*
        Ctor while seeding the randomness
        */
        GameController(int seed);

        /*
        Sets the board filename to load a board from the given filename
        */
        void setBoardFile(std::string filename);

        /*
        Sets the game filename to load a game from the given filename
        */
        void setGameFile(std::string filename);

        /*
        Prints the statuses of all players. Primarily used while testing
        */
       void printAllPlayerStatuses();
       /*
       Saves the game to the given filename
       */
       void saveGame(std::string filename);
       /*
       Begins the loop to play a series of games
       Keeps playing game until EOF or players say no on reprompt
       */
       void beginGameSeries();
       /*
       Sets the seed for the randomizer
       */
       void setSeed(int seed);
    
    private:
        /*
        Starts the game loop. Begins by getting player initial assignments (if needed- for example, if we loaded from a file it skips this step).
        Returns a pointer to the player that won the game. Returns nullptr if no player ended up winning
        */
        Player* startGame();
        /*
        Works with GameView to obtain initial assignments
         from the players. Returns a map from player color to the list of chosen initial assignments
        */
        std::unordered_map<PlayerColor, std::vector<int>> setInitialAssignments();
        /*
        Handles the precursor to a player's roll
        Eg: deals with setting players' dice, printing their statuses, prompting them, etc.
        Runs until player decides to roll the dice
        */
       void onPlayerTurn(Player* player);
       /*
       Deals with the case of when a player rolls a non-seven number
       */
       void onPlayerRoll(int rolled);
       /*
       Handles the case where the geese number is rolled. Reuiqres the active player (the player rolling the dice)
       1. Removes resources from players with 10 or more resources
       2. works with game view to prompt the movement of geese
       3. prompts the stealing of resources
       */
      void onGeeseRolled(Player* activePlayer);
    /*
    Iitializes a new game. If gamefile is provided, loads that game.
    If boardfile is provided, loads that board and gets initial assignments from the players.
    Otherwise, starts a new game.
    */
      void initializeGame();
      /*
      Handles the end of a turn
      */
      void endOfTurnSequence(Player* activePlayer);
      /*
      Handles the case when a player attempts to trade resources with another player
      */
      void onTradeCommand(Player* activePlayer);
};


export class TestHarness {
    private:
        GameController controller;
    public:
        TestHarness(): controller{1000} {
            controller.setGameFile("game.txt");
            controller.initializeGame();
        }
        void printAdjacentCriterionsToGoal(int goalNumber) {
            std::vector<int> adjacentCriterions = controller.gameManager->getAdjacentCriterions(goalNumber, false);
            for (int criterion : adjacentCriterions) {
                std::cout << criterion << " ";
            }
            std::cout << std::endl;
        }
        void printAdjacentGoalsToCriterion(int criterionNumber) {
            std::vector<int> adjacentGoals = controller.gameManager->getAdjacentGoals(criterionNumber, true);
            for (int goal : adjacentGoals) {
                std::cout << goal << " ";
            }
            std::cout << std::endl;
        }
        void printAdjacentCriterionsToCriterion(int criterionNumber) {
            std::vector<int> adjacentCriterions = controller.gameManager->getAdjacentCriterions(criterionNumber, true);
            for (int criterion : adjacentCriterions) {
                std::cout << criterion << " ";
            }
            std::cout << std::endl;
        }
        void printAdjacentGoalsToGoal(int goalNumber) {
            std::vector<int> adjacentGoals = controller.gameManager->getAdjacentGoals(goalNumber, false);
            for (int goal : adjacentGoals) {
                std::cout << goal << " ";
            }
            std::cout << std::endl;
        }


};
