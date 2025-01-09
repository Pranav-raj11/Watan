export module Board;
import <vector>;
import <memory>;

import GenericModel;
import HexTile;
import Objective;
import Criterion;
import Goal;



export class Board: public GenericModel {
    int geeseTileNumber; // negative number for no geese
    /*
    Goals and Criterions will be stored in order (i.e index 0 represents)
    goal number 0, or criterion number 0, respectively.
    */
    std::vector<HexTile*> hexTiles;
    std::vector<Criterion*> criterions;
    std::vector<Goal*> goals;

    public:
        /*
        Sets up the given HexTiles and geese number. 
        Can be used for either loading a previous game or new game.
        Board is responsible for deleting hexTiles
        - May sort the goals and criterions if not sorted already
        */
        Board(int prevGeeseTileNum, std::vector<HexTile*> & hexTiles,
            std::vector<Criterion*> criterions, std::vector<Goal*> goals
        );

        virtual ~Board();
        /*
        Get the tile number where the geese is located
        */
        int getGeeseTile();
        /*
        Moves the geese to the new tile number
        */
        void moveGeese(int newGeeseTileNumber);
        /*
        Gets the tile at the given tile number
        */
        HexTile* getTileAt(int tileNumber);
        /*
        Gets the criterion with objective number i
        */
        Criterion* getCriterionByNumber(int i);

        /*
        Gets the Goal with objective number i
        */
        Goal* getGoalByNumber(int i);
};


Board::Board(int initialGeeseTileNumber, std::vector<HexTile*> & hexTiles,  std::vector<Criterion*> criterions, std::vector<Goal*> goals): geeseTileNumber{initialGeeseTileNumber}, hexTiles{hexTiles},  criterions{criterions}, goals{goals} {}

Board::~Board() {
    // delete all hexTiles
    for (auto item : hexTiles) {
        delete item;
    }
    // delete all criterions
    for (auto item : criterions) {
        delete item;
    }
    // delete all goals
    for (auto item : goals) {
        delete item;
    }
}


int Board::getGeeseTile() {
    return geeseTileNumber;
}

void Board::moveGeese(int newGeeseTileNumber) {
    geeseTileNumber = newGeeseTileNumber;
}

HexTile* Board::getTileAt(int tileNumber) {
    return hexTiles[tileNumber];
}

Criterion* Board::getCriterionByNumber(int i) {
    return criterions[i];
}

Goal* Board::getGoalByNumber(int i) {
    return goals[i];
}
