export module ViewProxy;

import <sstream>;
import <string>;
import <iostream>;
import <vector>;
import <unordered_map>;

import GameView;
import BoardView;
import Board;
import types;
import Criterion;
import Player;

export class ViewProxy {
    /*
    This class is a wrapper around game view to deal with
    invalid inputs (i.e entering string instead of int)
    Does NOT deal with logic handling (such as ensuring the user entered a criterion that isnt already completed)
    */
    GameView gameView;
    BoardView boardView;
    public:
        /*
        Constructs the game view and board view
        */
        ViewProxy();
        /*
        Prompts the student with the given color to choose an initial assignment to complete
        Returns the input from the player (int)
        */
        int chooseInitialAssignments(PlayerColor studentColor);

        /*
        Notifies the player that it's their turn,
        and calls the GameView's methods.
        Returns an enum of TurnCommand indicating the command the player chose
        */
        TurnCommand playerTurn(Player* player);

        /*
        Prompts the user to enter what number they want to roll when 
        choosing a loaded dice
        */
        int getLoadedRoll();
        /*
        Prints the board
        */
       void printBoard(Board* board);
       /*
       rolled
        Need a map from player, to a map from the resource to the amount the gained
       */
      void printResourcesGained( std::unordered_map<Player*, std::unordered_map<ResourceType, int>> gains);

        /*
        Prints the player statuses
        */
        void printPlayerStatus(Player* player);

        /*
        Prints the lost resources after a geese was rolled.
        1. losses: a map from a player to a map from resource type to an int, indicating how
        much of each resource a player lost
        */
       void printGeeseLosses(std::unordered_map<Player*, std::unordered_map<ResourceType, int>> losses);

        /*
        Prompts the student to move the geese to a new location. Return an int
        of the location they chose
        */
       int promptGeeseMovement();
        /*
        Prints the options the active player can steal from
        after rolling a geese
        */
       void printStealableOptions(Player* activePlayer, std::vector<Player*> stealables);
       /*
       Prompts a student to steal from another student. Returns
       the PlayerColor the student chose
       1. Takes in an activePlayer. Ensures the same color is not entered
       */
        PlayerColor promptSteal(Player* activePlayer);
        /*
        Prints the criteria that the player has completed
        */
       void printCriteria(std::vector<Criterion*> completed);
       /*
       Prints an error message
       */
       void printError(std::string error);
      
      /*
      Prints the output of stealing a resource from a player.
      Requires a victim (stolen from), thief (the stealer), and the stolen resource type
      */
     void printStealOutput(Player* victim, Player* thief, ResourceType stolen);
     /*
     Prompts the user what they want to do at the end of their turn
     */
     EndTurnCommand endTurnPrompt(Player* activePlayer);

     /*
     Prompts the user if they want to trade resources with another player
     */
     bool promptTrade(Player* activePlayer, Player* offeringTo, ResourceType give, ResourceType receive);
     /*
     Reads an integer from stdin. Requires that the input is a valid integer
     */
     int readInt();
     /*
     Reads a player color from stdin. Requires that the input is a valid player color
     */
     PlayerColor readPlayerColor();
     /*
     Reads the command (beginning of the turn) from stdin. Does error checking if invalid input
     */
     TurnCommand readTurnCommand();
     /*
     Reads a string from stdin.
     */
     std::string readString();
     /*
     Reads a resource type from stdin. Requires that the input is a valid resource type
     */
     ResourceType readResourceType();
     /*
     Prints the help command
     */
     void printHelp();
     /*
     Prompts the user to play again. Returns true if they want to play again, false otherwise
     */
     bool promptPlayAgain(Player* winner);
    
    private:
        /*
        Prints an invalid input error message. Throws an error after
        */
       void throwInvalidInputError();

};

ViewProxy::ViewProxy(): gameView{}, boardView{} {}

bool ViewProxy::promptPlayAgain(Player* winner) {
    // keep reprompting until valid input
    while (true) {
        std::string inp = gameView.promptPlayAgain(winner);
        if (inp == "y" || inp == "yes") return true;
        if (inp == "n" || inp == "no") return false;
        // invalid, print error message
        this->printError("Invalid command.");
    }
}

void ViewProxy::throwInvalidInputError() {
    throw std::invalid_argument("Invalid command.");
}

void ViewProxy::printHelp() {
    gameView.printHelp();
}

TurnCommand ViewProxy::readTurnCommand() {
    gameView.printWaitForResponse();
    std::string input = gameView.readInput();
    TurnCommand cmd = stringToTurnCommand(input);
    while (cmd == TurnCommand::INVALID_CHOICE) {
        // read input again
        gameView.printWaitForResponse();
        input = gameView.readInput();
        cmd = stringToTurnCommand(input);
    }
    return cmd;
}

bool ViewProxy::promptTrade(Player* activePlayer, Player* offeringTo, ResourceType give, ResourceType receive) {
    // try reading input
    std::string response = gameView.promptTrade(activePlayer, offeringTo, give, receive);
    if (response == "yes") return true;
    if (response == "no") return false;
    // invalid, so throw error
    throwInvalidInputError();
}
void ViewProxy::printCriteria(std::vector<Criterion*> completed) {
    gameView.printCriteria(completed);
}

ResourceType ViewProxy::readResourceType() {
    std::string input = gameView.readInput();
    ResourceType res = stringToResourceType(input);
    if (res == ResourceType::NETFLIX) throwInvalidInputError();
    return res;
}

void ViewProxy::printError(std::string error) {
    gameView.printError(error);
}

PlayerColor ViewProxy::readPlayerColor() {
    std::string input = gameView.readInput();
    PlayerColor col = stringToPlayerColor(input);
    if (col == PlayerColor::INVALID) throwInvalidInputError();
    return col;
}

std::string ViewProxy::readString() {
    std::string input;
    std::cin >> input;
    return input;
}

int ViewProxy::readInt() {
    std::string input;
    std::cin >> input;
    std::istringstream iss{input};
    int num;
    iss >> num;
    return num;
}

EndTurnCommand ViewProxy::endTurnPrompt(Player* activePlayer) {
    // loop through until valid input
    while (true) {
        gameView.printWaitForResponse();
        std::istringstream iss{gameView.readInput()};
        std::string inp;
        iss >> inp;
        EndTurnCommand cmd = stringToEndTurnCommand(inp);
        if (cmd != EndTurnCommand::INVALID_END_TURN) return cmd;
        // invalid, print error message
        this->printError("Invalid command.");
    }
}

void ViewProxy::printStealOutput(Player* victim, Player* thief, ResourceType stolen) {
    gameView.printStealOutput(victim, thief, stolen);
}

PlayerColor ViewProxy::promptSteal(Player* activePlayer) {
    std::string inp;
    bool prompted = false;
    // loop until valid input is entered
    while (true) {
        if (!prompted) {
            inp = gameView.promptSteal();
            prompted = true;
        } else {
            gameView.printWaitForResponse();
            inp = gameView.readInput();
        }
        PlayerColor col = stringToPlayerColor(inp);
        // if invalid player or active player, repeat
        if (col == PlayerColor::INVALID || col == activePlayer->getColor()) continue;
        // return the color
        return col;
    }
}

void ViewProxy::printStealableOptions(Player* activePlayer, std::vector<Player*> stealables) {
    gameView.printStealableOptions(activePlayer, stealables);
}

int ViewProxy::promptGeeseMovement() {
    std::string input;
    int num;
    bool prompted = false;
    // loop until valid int is entered
    while (true) {
        if (!prompted) {
            input = gameView.promptGeeseMovement();
            prompted = true;
        } else {
            gameView.printWaitForResponse();
            input = gameView.readInput();
        }
        std::istringstream iss{input};
        if (iss >> num && 0 <= num && num < NUM_TILES) {
            return num;
        }
        // invalid, print error message
        this->printError("Invalid Geese Location.");
    }
}

void ViewProxy::printGeeseLosses(std::unordered_map<Player*, std::unordered_map<ResourceType, int>> losses) {
    gameView.printGeeseLosses(losses);
}

TurnCommand ViewProxy::playerTurn(Player* player) {
    std::string input;
    bool prompted = false;
    // loop till user enters a valid input
    while (true) {
        // prompt only once. Apart from the first time, just wait for response
        if (!prompted) {
            input = gameView.playerTurn(player);
            prompted = true;
        } else {
            gameView.printWaitForResponse();
            input = gameView.readInput();
        }
        if (input == "fair") return TurnCommand::FAIR;
        else if (input == "roll") return TurnCommand::ROLL;
        else if (input == "load") return TurnCommand::LOAD;
        // invalid, print error message
        this->printError("Invalid command.");
    }
}

void ViewProxy::printPlayerStatus(Player* player) {
    gameView.printPlayerStatus(player);
}

int ViewProxy::getLoadedRoll() {
    int inp;
    std::string actual;
    bool prompted = false;

    actual = gameView.promptLoadedRoll();
    prompted = true;
    
    std::istringstream iss{actual};
    // check valid int and valid number
    if (iss >> inp && 2 <= inp && inp <= 12) {
        return inp;
    }
    // prompt error
    this->printError("Invalid roll.");
    return -1;
}

int ViewProxy::chooseInitialAssignments(PlayerColor studentColor) {
    std::string input;
    int assignmentChosen;
    bool prompted = false;
    // keep trying until valid input
    while (true) {
        // prompt only once. Apart from the first time, just wait for response
        if (!prompted) {
            input = gameView.chooseInitialAssignments(studentColor);
            prompted = true;
        } else {
            gameView.printWaitForResponse();
            input = gameView.readInput();
        }
        std::istringstream iss{input};
        // valid input, return it
        if (iss >> assignmentChosen) {
            return assignmentChosen;
        }
        // clear error states
        iss.clear();
    }

    return -1;
}

void ViewProxy::printBoard(Board* board) {
    boardView.printBoard(board);
}


void ViewProxy::printResourcesGained( std::unordered_map<Player*, std::unordered_map<ResourceType, int>> gains) {
    gameView.printResourcesGained(gains);
}