module BoardView;

const int HEX_TILE_EDGE_SIZE = 14; // edge size of the hex tile (the top edge)
const int TOTAL_HEX_COLS = 5;
const int SPACE_TILL_RESOURCE = 5;
const int TILE_HEIGHT = 9;
// const int TOTAL_WIDTH = 17 * 5; // eyeball this
const int TOTAL_WIDTH = 90; // manually counted :()
const int TOTAL_BOARD_HEIGHT = 41;

const std::unordered_map<int, int> spaceByRow{
    {0, HEX_TILE_EDGE_SIZE},
    {1, HEX_TILE_EDGE_SIZE},
    {2, HEX_TILE_EDGE_SIZE + 2},
    {3, HEX_TILE_EDGE_SIZE + 4},
    {4, HEX_TILE_EDGE_SIZE + 4}, // center
    {5, HEX_TILE_EDGE_SIZE + 4},
    {6, HEX_TILE_EDGE_SIZE + 2},
    {7, HEX_TILE_EDGE_SIZE}
    // the bottom edge is considered part of the new tile
};

void BoardView::printBoard(Board* board) {
    // this keeps track on the current row (relative to hex tile)
    // based on the colum (there are 5 columns for us!)
    std::vector<int> rowIndexByColumn{-8, -4, 0, -4, -8};
    std::vector<HexTile*> activeTiles{nullptr, nullptr, board->getTileAt(0), nullptr, nullptr};

    int goalCount = 0; 
    int criterionCount = 0;
    int tileCount = 0;
    int row = 0;
    // go through each row, add spaces to left and right, and print each row
    for (int i = 0; i < TOTAL_BOARD_HEIGHT; i++) { //iterate through rows
        int numInitSpace = this->getNumInitialSpaces(rowIndexByColumn);
        this->printSpaces(numInitSpace);
        // now loop trhrough and print tiles
        int col = 0;
        int count = -1;
        for (auto tile : activeTiles) {
            count++;
            if (tile == nullptr) continue;
            this->printTile(goalCount, criterionCount, tileCount, rowIndexByColumn[count], col, count, tile, board);
        }
        // trailing spaces
        this->printSpaces(numInitSpace);
        std::cout << std::endl;
        // increment and such
        row++;
        // if approaching last row, do NOT update the tiles. Just reindex center column
        if (i == TOTAL_BOARD_HEIGHT - 2) {rowIndexByColumn[2] = 0; continue;}
        for (int i = 0; i < TOTAL_HEX_COLS; i++) {
            rowIndexByColumn[i] = (rowIndexByColumn[i] >= 0) 
    ? std::min((rowIndexByColumn[i] + 1) % 8, rowIndexByColumn[i] + 1)
    : rowIndexByColumn[i] + 1;

            // update hex tiles as needed
            // new tile reached and not the center column
            if (rowIndexByColumn[i] == 0 && i != 2) {
                if ((i == 0 || i == TOTAL_HEX_COLS - 1) && row >= 32) continue;
                else if ((i == 1 || i == TOTAL_HEX_COLS - 2) && row >= 36) continue;
                // set initial active tiles
                if (activeTiles[i] == nullptr) {
                    if (i == 0) activeTiles[i] = board->getTileAt(3);
                    else if (i == 1) activeTiles[i] = board->getTileAt(1);
                    else if (i == 3) activeTiles[i] = board->getTileAt(2);
                    else activeTiles[i] = board->getTileAt(5);
                } else {
                    // if not null, increment!
                    activeTiles[i] = board->getTileAt(activeTiles[i]->getTileNumber()+5);
                } 
            } else if (rowIndexByColumn[i] == 0) {
                    // since i = 2, check parity and act
                    if (activeTiles[i]->getTileNumber() == 4 || activeTiles[i]->getTileNumber() == 9) {
                        activeTiles[i] = board->getTileAt(activeTiles[i]->getTileNumber() + 5);
                    } else {
                        activeTiles[i] = board->getTileAt(activeTiles[i]->getTileNumber() + 4);
                    }
            }
        }
        // check if reached the "tail" to remove the first column
        if (row >= 33) {
            activeTiles[0] = nullptr;
            activeTiles[TOTAL_HEX_COLS - 1] = nullptr;
            rowIndexByColumn[0] = -8;
            rowIndexByColumn[TOTAL_HEX_COLS - 1] = -8;
        }
        if (row >= 37) {
            activeTiles[1] = nullptr;
            activeTiles[TOTAL_HEX_COLS - 2] = nullptr;
            rowIndexByColumn[1] = -8;
            rowIndexByColumn[TOTAL_HEX_COLS - 2] = -8;
        }
    }
}


void BoardView::printTile(int & goalCount, int & critCount, int & tileCount, 
    int rowIndexAtColumn, int & columnNumber, int absoluteColumn, HexTile* tile, Board* board
) {
    /*
    The only column that prints the left '\' or '/' is the first column.
    Every other column only prints the right slashes.
    */
    if (rowIndexAtColumn < 0) return ;
    // odd tile- i.e there is no need to print any numbers here
    if (rowIndexAtColumn % 2 == 1) {
        // column number is 0 indexed. print this only if first column
        if (columnNumber == 0 && rowIndexAtColumn < 4) std::cout << "/";
        // if below half way through the tile, start with back slash
        else if (columnNumber == 0) std::cout << "\\";
        // print spaces in between
        // if third tile, make space for showing the resource
        if (rowIndexAtColumn == 3) {
            // DOES NOT PRINT THE ENDING '/' or '\'
            this->printResourceRow(tile);
        } 
        // if tile contains geese, print it
        else if (rowIndexAtColumn == 5 && board->getGeeseTile() == tile->getTileNumber()) {
            this->printGeese();
        } 
        else {
            // get num spaces to print excluding to '/' or '\'
            int numSpaces = HEX_TILE_EDGE_SIZE - (std::abs(TILE_HEIGHT / 2 - rowIndexAtColumn) - 2) * 2;
            this->printSpaces(numSpaces);
        }
        // now print the last backslash (the rightside of the hextile)
        if (absoluteColumn % 2 == 0 && rowIndexAtColumn < 4) std::cout << "\\"; // finish with back, since even and above half way throug htile
        // fnish with '/' since even column and halfway below tile
        else if (absoluteColumn % 2 == 0) std::cout << "/";
        else if (absoluteColumn % 2 == 1 && rowIndexAtColumn < 4) std::cout << "\\";
        else std::cout << "/"; //odd column, and halfway below the tile. End with '\'
    }
    // top edge of the tile. Print these
    else if (rowIndexAtColumn == 0) {
        std::cout << "|";
        this->printCriterion(board, critCount);
        std::cout << "|--";
        this->printGoal(board, goalCount);
        std::cout << "--|";
        this->printCriterion(board, critCount);
        std::cout << "|";
    }
    // now if 4. Need to print spaces and the number in between
    else if (rowIndexAtColumn == 4) {
        // since first column, also add the thing on the left
        if (columnNumber == 0) {
            std::cout << "|";
            this->printCriterion(board, critCount);
            std::cout << "|";
        }
        // if netflix resource, dont print tile value
        if (tile->getTileResource() == ResourceType::NETFLIX) {
            // add 2 since we are not printing the digits
            this->printSpaces(HEX_TILE_EDGE_SIZE + 2);
            return ;
        }
        // initial spaces between the tile value
        this->printSpaces(HEX_TILE_EDGE_SIZE / 2);
        if (tile->getTileValue() < 10) std::cout << " "; // extra space if single digit
        std::cout << tile->getTileValue();
        this->printSpaces(HEX_TILE_EDGE_SIZE / 2);
        // if the last column, also print the last criterion
        if (absoluteColumn == TOTAL_HEX_COLS - 1) {
            std::cout << "|";
            this->printCriterion(board, critCount);
            std::cout << "|";
        }
    }

    // if 2 or 6. Need to print goal numbers as well
    else if (rowIndexAtColumn % 2 == 0) {
        if (columnNumber == 0) {
            // print the left goal number only if the first column
            this->printGoal(board, goalCount);
        }
        // if 2, then need to print tile number as well
        if (rowIndexAtColumn == 2) {
            int numSpacesToAdd = HEX_TILE_EDGE_SIZE / 2 - 1;
            this->printSpaces(numSpacesToAdd);
            if (tileCount < 10) std::cout << " ";
            std::cout << tileCount;
            tileCount++;
            // now print the rest of the space before the next goal number
            this->printSpaces(numSpacesToAdd - 1);
        } else {
            // row is 6. So just print spaces
            this->printSpaces(HEX_TILE_EDGE_SIZE - 1);
        }

        // print the next goal number
        this->printGoal(board, goalCount);
    }
    columnNumber++;
}

void BoardView::printSpaces(int numSpaces) {
    for (int i = 0; i < numSpaces; i++) {
        std::cout << " ";
    }
}

void BoardView::printResourceRow(HexTile* tile) {
    std::string resource = resourceToString(tile->getTileResource());
    int totalSpace = HEX_TILE_EDGE_SIZE + 2 * 2 - 2; // top edge plus 4 to get third row size, -2 to remove the boundary '/' and '\'
    int leftOverSpaces = totalSpace - resource.size();

    this->printSpaces(SPACE_TILL_RESOURCE);
    // if we need to print the resource, print that alongside the proceeding spaces
    std::cout << resource; 
    this->printSpaces(leftOverSpaces - SPACE_TILL_RESOURCE);
}

int BoardView::getNumInitialSpaces(std::vector<int> rowIndexByColumn) {
    // rowIndexByColumn is a vector of length TOTAL_HEX_COLS,
    // each representing the current index (relative to the closest hextile)
    // can be negative

    // if both negative, then only thing appearing is the central column
    if (rowIndexByColumn[0] < 0 && rowIndexByColumn[1] < 0) {
        int numSpaces = (TOTAL_WIDTH - spaceByRow.at(rowIndexByColumn[2])) / 2; 
        // check if row starts with int, and subtract a space
        if (rowIndexByColumn[2] == 2 || rowIndexByColumn[2] == 6) numSpaces--;
        
        return numSpaces;
    }

    // first column still got no hextiles, while second to fourth do
    else if (rowIndexByColumn[0] < 0) {
        // subtract -4 from last term since there are 4 overlaps
        int numSpacesTotal = TOTAL_WIDTH - 2 * spaceByRow.at(rowIndexByColumn[1]);
        numSpacesTotal -= spaceByRow.at(rowIndexByColumn[2]) - 2;
        int totalSpace = numSpacesTotal / 2;
        // if this row starts with an int, remove one
        if (rowIndexByColumn[1] == 2 || rowIndexByColumn[1] == 6) totalSpace -=1;
        return totalSpace; // since we need only the space for the first half
    }
    // otherwise all three exist. just compute the difference
    // first take away the "triple hex tiles" in the row, then the "double hex tiles"
    // with any overlaps removed 
    int totalSpace = TOTAL_WIDTH - 3 * (spaceByRow.at(rowIndexByColumn[0])) - 2 * (spaceByRow.at(rowIndexByColumn[1]) - 2);

    if (rowIndexByColumn[0] % 4 == 0 && rowIndexByColumn[0] % 8 != 0) totalSpace -= 6; // this is to accomodate the extra digits
    // if starts with number, then subtract 
    if (rowIndexByColumn[0] == 2 || rowIndexByColumn[0] == 6) totalSpace -= 2;
    return totalSpace / 2;
}

void BoardView::printGeese() {
    int GEESE_LEN = 5;
    int TOTAL_LEN = HEX_TILE_EDGE_SIZE + 4 - 2; // sub 2 cuz excluding boundary
    this->printSpaces(5);
    std::cout << "GEESE";
    this->printSpaces(TOTAL_LEN - GEESE_LEN - 5);
}

void BoardView::printCriterion(Board* board, int & critNumber) {
    // check if someone completed the criterion
    Criterion* crit = board->getCriterionByNumber(critNumber);
    Player* completed = crit->getOwner();
    // if no one completed, just print number
    if (completed == nullptr) {
        if (critNumber < 10) std::cout << " ";
        std::cout << critNumber;
        critNumber++;
        return ;
    }
    critNumber++;
    // print student color char follows by completion level
    std::cout << playerColorToChar(completed->getColor()) << completionLevelToChar(crit->getCompletionLevel());
}

void BoardView::printGoal(Board* board, int & goalNumber) {
    Goal* myGoal = board->getGoalByNumber(goalNumber);
    Player* completed = myGoal->getOwner();
    // if no one, just print
    if (completed == nullptr) {
        if (goalNumber < 10) std::cout << " ";
        std::cout << goalNumber;
        goalNumber++;
        return;
    }
    goalNumber++;
    // print initial
    std::cout << " " << playerColorToChar(completed->getColor());
}

BoardView::BoardView() {}
