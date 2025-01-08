module GameBuilder;


const int TOTAL_BOARD_HEIGHT = 21; // to iterate through the board's numbers
const int TOTAL_HEX_COLS = 5;

GameBuilder::GameBuilder(std::shared_ptr<ModelFacade> db, std::shared_ptr<RandomGenerator> randomizer): database{db}, randomizer{randomizer}, goals{}, crit{} {
    // initializes goals and crit

    for (int i = 0; i < NUM_CRITERIONS; i++) {
        crit.push_back(new Criterion{i});
    }

    for (int i = 0; i < NUM_GOALS; i++) {
        goals.push_back(new Goal{i});
    }
}

// TODO: add random implementation
void GameBuilder::buildBoard() {
    if (goals.size() == 0 || crit.size() == 0) {
        throw std::runtime_error("Goals and Criterions not initialized yet!");
    }

    std::vector<HexTile*> tiles;

    // Make copies of the initial counts that we can modify
    std::unordered_map<ResourceType, int> remainingResources = initialResourceCount;
    std::unordered_map<int, int> remainingTileValues = initialNumTileValues;

    for (int i = 0; i < NUM_TILES; i++) {
        // Randomly assign a resource type and value
        ResourceType resource = randomizer->getRandomKey(remainingResources, NUM_TILES - i);
        
        // Decrement the count for this resource. So probabilities of getting next resources
        // are adjusted accordingly
        remainingResources[resource]--;

        if (resource == ResourceType::NETFLIX) {
            // netflix does not display a number. So we set it to 7
            tiles.push_back(new HexTile{resource, i, 7});
            remainingTileValues[7]--;
            continue;
        }

        int chosenValue = randomizer->getRandomKey(remainingTileValues, NUM_TILES - i);
    
        // Decrement the count for this tile value
        remainingTileValues[chosenValue]--;

        // Create and add the tile
        tiles.push_back(new HexTile{resource, i, chosenValue});
    }


    int geeseTile = randomizer->getRandom(0, NUM_TILES - 1);
    // this also sorts the hex tiles in desired order
    this->setObjectiveParents(tiles);
    // Create and store board
    Board* board = new Board(geeseTile, tiles, crit, goals);
    database->setBoard(board);
}

void GameBuilder::buildBoard(std::ifstream & ifs) {
    // make sure these are initialized first
    if (goals.size() == 0 || crit.size() == 0) {
        throw std::runtime_error("Goals and Criterions not initialized yet!");
    }

    std::vector<HexTile*> tiles{};
    int resource;
    int value;
    int tileNumber = 0;
    while (ifs >> resource >> value) {
        HexTile* tile = new HexTile{static_cast<ResourceType> (resource), tileNumber, value};
        tileNumber++;
        tiles.push_back(tile);
    }
    this->setObjectiveParents(tiles);

    // now read geese number
    int geeseNum = resource; // last resource read is geese number!

    Board* myBoard = new Board{geeseNum, tiles, crit, goals};
    
    database->setBoard(myBoard);
}

void GameBuilder::sortHexTiles(std::vector<HexTile*> tiles) {
    for (HexTile* tile : tiles) {
        tile->sortCriterionAndGoalNumbers();
    }
}

void GameBuilder::buildPlayers(std::ifstream & ifs) {
    // reads each player data
    std::vector<Player*> players{};
    for (int i = 0; i < NUM_PLAYERS; i++) {
        // default dice
        Dice* fairDice = new FairDice{randomizer};
        // NOTE: passing integer i as player color. Be careful
        players.push_back(new Student{static_cast<PlayerColor>(i), fairDice});
        int numCaf, numLab, numLec, numStud, numTut;
        ifs >> numCaf >> numLab >> numLec >> numStud >> numTut;
        // set player resources
        players[i]->incrementCaffiene(numCaf);
        players[i]->incrementLab(numLab);
        players[i]->incrementLectures(numLec);
        players[i]->incrementStudy(numStud);
        players[i]->incrementTut(numTut);
        // read the line
        std::string s;
        getline(ifs, s);
        std::istringstream iss{s};
        // read the goals
        char temp;
        iss >> temp; // reads the char 'g'
        this->loadGoals(goals, iss, players[i]);
        iss >> temp; // read the char 'c'
        this->loadCriterions(crit, iss, players[i]);
    }
    // set the players in the db
    database->setPlayers(players);
}

void GameBuilder::buildPlayers(std::unordered_map<PlayerColor, std::vector<int>> initAssignments) {
    std::vector<Player*> players{};
    for (int i = 0; i < NUM_PLAYERS; i++) {
        PlayerColor col = static_cast<PlayerColor>(i);
        Dice* fairDice = new FairDice{randomizer};
        players.push_back(new Student{col, fairDice});
        // now complete each criterion to assignment
        //  from the initial assignments the playe rchose
        for (int j : initAssignments[col]) {
            crit[j]->complete(players[i]);
        }
    }
    database->setPlayers(players);
}

void GameBuilder::loadCriterions(std::vector<Criterion*> & ref, std::istringstream & ifs, Player* current) {
    int critNum, achievementLevel;
    // adds players to the criteiron object, and the criterion number to the players
    while (ifs >> critNum >> achievementLevel) {
        // complete the criterion. Also adds to player and increments their vp
        ref[critNum]->complete(current);
        ref[critNum]->setCompletion(static_cast<CriterionLevel> (achievementLevel));
    }
    // after reading non-integer, we hit error. clear this
    ifs.clear();
}

void GameBuilder::loadGoals(std::vector<Goal*> & ref, std::istringstream & ifs, Player* current) {
    int goalNum;
    while (ifs >> goalNum) {
        ref[goalNum]->complete(current);
        current->addCompletedGoal(goalNum);
    }
    ifs.clear();
}

void GameBuilder::setTurn(int turn) {
    database->setTurn(static_cast<PlayerColor>(turn));
}

void GameBuilder::setObjectiveParents(std::vector<HexTile*> tiles) {
    std::vector<int> rowIndexByColumn{-4, -2, 0, -2, -4};
    std::vector<HexTile*> activeTiles{nullptr, nullptr, tiles[0], nullptr, nullptr};

    int goalCount = 0; 
    int criterionCount = 0;
    int tileCount = 0;
    int row = 0;

    for (int i = 0; i < TOTAL_BOARD_HEIGHT; i++) {
        int col = 0;
        int count = -1;
        for (auto tile : activeTiles) {
            count++;
            if (tile == nullptr) continue;
            this->addObjectiveParents(goalCount, criterionCount, count, row, activeTiles, rowIndexByColumn, tile, tiles);
        }
        row++;
        // if approaching last row, do NOT update the tiles. Just reindex center column
        if (i == TOTAL_BOARD_HEIGHT - 2) {rowIndexByColumn[2] = 0; continue;}
        for (int i = 0; i < TOTAL_HEX_COLS; i++) {
            rowIndexByColumn[i] = (rowIndexByColumn[i] >= 0) 
    ? std::min((rowIndexByColumn[i] + 1) % 4, rowIndexByColumn[i] + 1)
    : rowIndexByColumn[i] + 1;

            // update hex tiles as needed
            // new tile reached and not the center column
            if (rowIndexByColumn[i] == 0 && i != 2) {
                if ((i == 0 || i == TOTAL_HEX_COLS - 1) && row >= 16) continue;
                else if ((i == 1 || i == TOTAL_HEX_COLS - 2) && row >= 18) continue;
                // set initial active tiles
                if (activeTiles[i] == nullptr) {
                    if (i == 0) activeTiles[i] = tiles[3];
                    else if (i == 1) activeTiles[i] = tiles[1];
                    else if (i == 3) activeTiles[i] = tiles[2];
                    else activeTiles[i] = tiles[5];
                } else {
                    // if not null, increment!
                    activeTiles[i] = tiles[activeTiles[i]->getTileNumber()+5];
                } 
            } else if (rowIndexByColumn[i] == 0) {
                    // since i = 2, check parity and act
                    if (activeTiles[i]->getTileNumber() == 4 || activeTiles[i]->getTileNumber() == 9) {
                        activeTiles[i] = tiles[activeTiles[i]->getTileNumber() + 5];
                    } else {
                        activeTiles[i] = tiles[activeTiles[i]->getTileNumber() + 4];
                    }
            }
        }
        // check if reached the "tail" to remove the first column
        if (row >= 17) {
            activeTiles[0] = nullptr;
            activeTiles[TOTAL_HEX_COLS - 1] = nullptr;
            rowIndexByColumn[0] = -8;
            rowIndexByColumn[TOTAL_HEX_COLS - 1] = -8;
        }
        if (row >= 19) {
            activeTiles[1] = nullptr;
            activeTiles[TOTAL_HEX_COLS - 2] = nullptr;
            rowIndexByColumn[1] = -8;
            rowIndexByColumn[TOTAL_HEX_COLS - 2] = -8;
        }
    }
    // sort the criterion and goal numbers in the tiles
    this->sortHexTiles(tiles);
}

void GameBuilder::addObjectiveParents(int & goalCount, int & critCount, int absColumnNumber, int absRowNumber, std::vector<HexTile*> activeTiles, std::vector<int> rowIndexByColumn, HexTile* currTile, std::vector<HexTile*> allTiles) {
    // current tile is empty so ontinue
    if (currTile == nullptr) return ;
    // if left and right are empty, just add and continue
    if (absColumnNumber > 0 && absColumnNumber < TOTAL_HEX_COLS - 1 && activeTiles[absColumnNumber - 1] == nullptr && activeTiles[absColumnNumber + 1] == nullptr) {
        // first add the criterions
        for (int i = 0; i < numCriterionsPerRow[absRowNumber]; i++) {
            crit[critCount]->addHexTileParent(currTile);
            currTile->addCriterionNumber(critCount);
            critCount++;
        }
        // now add the goals
        for (int i = 0; i < numGoalsPerRow[absRowNumber]; i++) {
            goals[goalCount]->addHexTileParent(currTile);
            currTile->addGoalNumber(goalCount);
            goalCount++;
        }
    }
    // if first column and either top or bottom of a hextile,
    // add the goal and (2) criterions
    else if (rowIndexByColumn[absColumnNumber] == 0) {
        // there are two tiles to add- the top and the current. get the top if it exists
        HexTile* top = nullptr;
        int tileNum = currTile->getTileNumber();
        // add the rowNumber bounds for each statement, so we don't also
        // add it to the top HexTile if it is the bottom row of the board
        if (absColumnNumber == 0 && tileNum > 3 && absRowNumber < 16) top = allTiles[tileNum - 5];
        if (absColumnNumber == 1 && tileNum > 1 && absRowNumber < 18) top = allTiles[tileNum - 5];
        else if (absColumnNumber == 2 && absRowNumber < 20) {
            if (tileNum == 4) top = allTiles[0];
            else if (tileNum < 18) top = allTiles[tileNum - 5];
            else if (tileNum == 18) top = allTiles[14];
        }
        else if (absColumnNumber == 3 && tileNum > 2 &&  absRowNumber < 18) top = allTiles[tileNum - 5];
        else if (absColumnNumber == 4 && tileNum > 5 && absRowNumber < 16) top = allTiles[tileNum - 5];
        // add the criterion to top as well, if it exists
        if (top != nullptr) {
            // add the two criterions
            for (int i = 0; i < 2; i++) {
                crit[critCount+i]->addHexTileParent(top);
                top->addCriterionNumber(critCount + i);
            }
            // add the one goal
            goals[goalCount]->addHexTileParent(top);
            top->addGoalNumber(goalCount);
        }
        // now add them to the current tile as well
        for (int i = 0; i < 2; i++) {
            crit[critCount + i]->addHexTileParent(currTile);
            currTile->addCriterionNumber(critCount + i);
        }
        critCount++; // increment only once so next tile can add this as well
        // increment another time if there is no tile to the right
        if (absColumnNumber == TOTAL_HEX_COLS - 1 || activeTiles[absColumnNumber + 1] == nullptr) critCount++;
        // add the one goal
        goals[goalCount]->addHexTileParent(currTile);
        currTile->addGoalNumber(goalCount);
        goalCount++;
    }
    // now if we are at the center of the current hextile
    else if (rowIndexByColumn[absColumnNumber] % 2 == 0 && rowIndexByColumn[absColumnNumber] % 4 != 0) {
        crit[critCount]->addHexTileParent(currTile);
        currTile->addCriterionNumber(critCount);
        critCount++;
        crit[critCount]->addHexTileParent(currTile);
        currTile->addCriterionNumber(critCount);
        // increment another time iff the rightside of the tile is empty
        if (absColumnNumber == TOTAL_HEX_COLS - 1 || activeTiles[absColumnNumber + 1] == nullptr) critCount++;
    }
    // otherwise if we are on a row with oly goals
    else if (rowIndexByColumn[absColumnNumber] % 2 == 1) {
        goals[goalCount]->addHexTileParent(currTile);
        currTile->addGoalNumber(goalCount);
        goalCount++;
        goals[goalCount]->addHexTileParent(currTile);
        currTile->addGoalNumber(goalCount);
        // increment another time iff the rightside of the tile is empty
        if (absColumnNumber == TOTAL_HEX_COLS - 1 || activeTiles[absColumnNumber + 1] == nullptr) goalCount++;
    }
}
