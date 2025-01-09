export module ModelFacade;
import <memory>;
import <vector>;
import <stdexcept>;

import types;
import Randomness;


// import certain partitions of the module
import Player;
import Board;
import Student;
import FairDice;


export class ModelFacade {
    std::vector<Player*> players;
    Board* board;
    PlayerColor currentTurn;
    std::shared_ptr<RandomGenerator> randomizer;

    public:

        ModelFacade();
        
        ModelFacade(Board* board, std::shared_ptr<RandomGenerator> randomizer); // initialize everything on its own

        ModelFacade(Board* board, std::vector<Player*> & players, PlayerColor curr, std::shared_ptr<RandomGenerator> randomizer); // to load an existing game

        ~ModelFacade();

        Board* getBoard();
        /*
        Gets the player with the given color
        Returns nullptr if not found
        */
        Player* getPlayer(PlayerColor color);

        void setBoard(Board* board);

        void setPlayers(std::vector<Player*> players);

        /*
        Returns the player color whose turn it is right now
        */
        PlayerColor getTurn();

        /*
        Sets the provided player color to be the curren turn
        */
       void setTurn(PlayerColor col);

       /*
       Resets the game to the initial state. 
       Deletes all data associated with the game, including players, board, dice, etc.
       Does NOT delete randomizer, as it is shared
       */
       void resetGame();
    
    private:
        /*
        Creates a new vector for players, with all colors as provided in the Models 
        interface
        */
        void initializePlayers();

};

ModelFacade::ModelFacade(): players{}, board{nullptr}, currentTurn{PlayerColor::BLUE}, randomizer{nullptr} {}

ModelFacade::ModelFacade(Board* newBoard, std::shared_ptr<RandomGenerator> randomizer): players{}, board{newBoard}, currentTurn{PlayerColor::BLUE}, randomizer{randomizer} {
    // create our set of players by color
    this->initializePlayers();
}

ModelFacade::~ModelFacade() {
    // deletes the resources
    this->resetGame();
}

ModelFacade::ModelFacade(Board* board, std::vector<Player*> & players, PlayerColor curr, std::shared_ptr<RandomGenerator> randomizer): players{std::move(players)}, board{board}, currentTurn{curr}, randomizer{randomizer} {

}

void ModelFacade::resetGame() {
    // delete all players
    for (auto player : players) {
        delete player;
    }
    // delete the board
    delete board;
    // reset the rest of the data
    players.clear();
    board = nullptr;
    currentTurn = allPlayerColors().front();
}

Board* ModelFacade::getBoard() {
    return board;
}

Player* ModelFacade::getPlayer(PlayerColor color) {
    for (auto player : players) {
        if (player->getColor() == color) {
            return player;
        }
    }
    // if none, return nullptr
    return nullptr;
}

void ModelFacade::initializePlayers() {
    for (PlayerColor color : allPlayerColors()) {
        // create a new player
        Player* currPlayer = new Student{color, std::vector<int>{0}, std::vector<int>{1}, new FairDice{randomizer}};
        players.push_back(currPlayer);
    }
}

void ModelFacade::setBoard(Board* board) {
    if (this->board == nullptr) {
        this->board = board;
    } else {
        throw std::runtime_error("Board is already set!");
    }
}

void ModelFacade::setPlayers(std::vector<Player*> gamers) {
    if (players.size() == 0) {
        players = gamers;
    } else {
        throw std::runtime_error("Players are already set!");
    }
}

PlayerColor ModelFacade::getTurn() {
    return currentTurn;
}

void ModelFacade::setTurn(PlayerColor col) {
    currentTurn = col;
}