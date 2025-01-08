export module GameView;

import <unordered_map>;
import <string>;
import <vector>;
import <iostream>;

import GenericView;
import Player;
import types;
import Criterion;
import EndOfFile;

export class GameView: public GenericView {
    public:
        /*
        Prompts the student with the given color to choose an initial assignment to complete
        Returns the input from the player (string)
        */
        std::string chooseInitialAssignments(PlayerColor studentColor);
        /*
        Prints the status of the provided player
        */
        void printPlayerStatus(Player* player);
        /*
        Notifies the player that it's their turn, 
        prints the player's status,
        and waits for the user to enter an action 
        */
        std::string playerTurn(Player* player);
        /*
        Prompts the user to enter a roll between 2 and 12 for a loaded dice.
        Returns the string of whatever they enter
        */
        std::string promptLoadedRoll();
        /*
        Prints the list of resources each player gained after a non-7 number was rolled
        Need a map from player, to a map from the resource to the amount the gained
        */
        void printResourcesGained( std::unordered_map<Player*, std::unordered_map<ResourceType, int>> gains);

        /*
        Prints the lost resources after a geese was rolled.
        1. losses: a map from a player to a map from resource type to an int, indicating how
        much of each resource a player lost
        */

        void printGeeseLosses(std::unordered_map<Player*, std::unordered_map<ResourceType, int>> losses);

        /*
        Displays the winner.
        Prompts the users to play again. Returns the string of whatever they input
        */
        std::string promptPlayAgain(Player* winner);

        /*
        Prompts the student to move the geese to a new location. Return an string
        of the input they entered
        */
       std::string promptGeeseMovement();

       /*
        Prints the options the active player can steal from
        after rolling a geese
        */
       void printStealableOptions(Player* activePlayer, std::vector<Player*> stealables);
       /*
       Prompts a student to steal from another student. Returns
       the string of whatever the user input
       */
        std::string promptSteal();

    /*
      Prints the output of stealing a resource from a player.
      Requires a victim (stolen from), thief (the stealer), and the stolen resource type
      */
     void printStealOutput(Player* victim, Player* thief, ResourceType stolen);

    /*
    Prints the criteria that the player has completed
    */
    void printCriteria(std::vector<Criterion*> completed);
    /*
    Prints an error message
    */
    void printError(std::string error);

       /*
       Prompts the user if they want to trade resources with another player
       */
       std::string promptTrade(Player* activePlayer, Player* offeringTo, ResourceType give, ResourceType receive);

       /*
       Reads an input from the user and returns it as a string
       */
       std::string readInput();
       // prints the '>' to indicate waiting for a response
       void printWaitForResponse();
       /*
       Prints the help command
       */
       void printHelp();
    private:
        /*
        Returns a boolean indicating if all the entries are 0s.
        True if all entries are 0 (the values), False otherwise
        */
       bool playerGainedResources(std::unordered_map<ResourceType, int> gains); 
};

