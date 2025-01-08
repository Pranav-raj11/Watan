export module BoardView;
import <iostream>;
import <string>;
import <vector>;
import <unordered_map>;

import GenericView;
import Board;
import HexTile;
import types;
import Player;
import Goal;
import Criterion;

// TODO: inherit from GenericVIew
export class BoardView {

    public:
        void printBoard(Board* board);
        BoardView();

    private:
        void printTile(int & goalCount, int & critCount, int & tileCount, int rowAtColumn, int & col, int absoluteColumn, HexTile* tile, Board* board);

        void printSpaces(int numSpaces);

        void printResourceRow(HexTile* tile);

        int getNumInitialSpaces(std::vector<int> rowByIndex);

        void printCriterion(Board* board, int & critCount);

        void printGoal(Board* board, int & goalCount);

        void printGeese();
};

 
