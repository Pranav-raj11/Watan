module BoardManager;

BoardManager::BoardManager(std::shared_ptr<ModelFacade> gameData, std::shared_ptr<RandomGenerator> randomizer): GenericManager{gameData, randomizer} {}

void BoardManager::moveGeese(int newTileNum) {
    gameData->getBoard()->moveGeese(newTileNum);
}

bool BoardManager::hasCriterionInTile(int tileNum, Player* activePlayer) {
    Board* board = gameData->getBoard();
    // loop through player criterions
    for (int critNum : activePlayer->getCompletedCriterions()) {
        Criterion* crit = board->getCriterionByNumber(critNum);
        // loop through the parents of the criterion
        for (HexTile* parent : crit->getHexTileParents()) {
            // has a tile, return true
            if (parent->getTileNumber() == tileNum) return true;
        }
    }
    return false;
}

std::vector<int> BoardManager::getAdjacentCriterionsToCriterion(int criterionNumber) {
    Criterion* crit = gameData->getBoard()->getCriterionByNumber(criterionNumber);
    // get adjacent tiles
    std::vector<HexTile*> adjacentTiles = crit->getHexTileParents();
    // now for each adjacet tile, get adjacent criterion numbers
    std::vector<int> adjacentCriterions{};
    for (HexTile* tile : adjacentTiles) {
        std::vector<int> criterionNumbers = tile->getCriterionNumbers();
        // get the index of the criterionNumber
        int index = this->getIndex(criterionNumbers, criterionNumber);
        // look once to the left and once to the right
        int leftIndex = (index == 0) ? criterionNumbers.size() - 1 : index - 1;
        int rightIndex = (index + 1) % criterionNumbers.size();
        adjacentCriterions.push_back(criterionNumbers[leftIndex]);
        adjacentCriterions.push_back(criterionNumbers[rightIndex]);
    }
    return this->removeDuplicates(adjacentCriterions);
}

std::vector<int> BoardManager::getAdjacentGoalsToGoal(int goalNumber) {
    // get adjacent tiles
    Board* board = gameData->getBoard();
    Goal* goal = board->getGoalByNumber(goalNumber);
    // loop through the first parent tile.
    HexTile* tile = goal->getHexTileParents()[0];
    std::vector<int> goalNumbers = tile->getGoalNumbers();
    std::vector<int> criterionNumbers = tile->getCriterionNumbers();

    int index = this->getIndex(goalNumbers, goalNumber);

    Criterion* leftNeighbor = board->getCriterionByNumber(criterionNumbers[index]);
    Criterion* rightNeighbor = board->getCriterionByNumber(criterionNumbers[(index + 1) % 6]);
    // get neighboring goals of adjacent criterions
    std::vector<int> leftGoals = this->getAdjacentGoalsToCriterion(leftNeighbor->getObjectiveNumber());
    std::vector<int> rightGoals = this->getAdjacentGoalsToCriterion(rightNeighbor->getObjectiveNumber());
    // add rightGoals to adjacentGoals
    for (int goal : rightGoals) {
        leftGoals.push_back(goal);
    }

    // remove goalNumber from adjacentGoals
    std::vector<int> filteredGoals;
    for (int goal : leftGoals) {
        if (goal != goalNumber) {
            filteredGoals.push_back(goal);
        }
    }
    leftGoals = filteredGoals;

    return this->removeDuplicates(leftGoals);
}

std::vector<int> BoardManager::getAdjacentCriterionsToGoal(int goalNumber) {
    Goal* goal = gameData->getBoard()->getGoalByNumber(goalNumber);
    // get adjacent tiles
    std::vector<HexTile*> adjacentTiles = goal->getHexTileParents();
    // now for each adjacet tile, get adjacent criterion numbers
    std::vector<int> adjacentCriterions{};
    for (HexTile* tile : adjacentTiles) {
        std::vector<int> criterionNumbers = tile->getCriterionNumbers();
        std::vector<int> goalNumbers = tile->getGoalNumbers();
        // get the index of the criterionNumber
        int index = this->getIndex(goalNumbers, goalNumber);
        // look at the same index or the next index
        int leftIndex = index;
        int rightIndex = (index + 1) % criterionNumbers.size();
        adjacentCriterions.push_back(criterionNumbers[leftIndex]);
        adjacentCriterions.push_back(criterionNumbers[rightIndex]);
    }
    return this->removeDuplicates(adjacentCriterions);
}

std::vector<int> BoardManager::getAdjacentGoalsToCriterion(int criterionNumber) {
    Criterion* crit = gameData->getBoard()->getCriterionByNumber(criterionNumber);
    // get adjacent tiles
    std::vector<HexTile*> adjacentTiles = crit->getHexTileParents();
    // now for each adjacet tile, get adjacent criterion numbers
    std::vector<int> adjacentGoals{};
    for (HexTile* tile : adjacentTiles) {
        std::vector<int> goalNumbers = tile->getGoalNumbers();
        std::vector<int> criterionNumbers = tile->getCriterionNumbers();
        // get the index of the criterionNumber
        int index = this->getIndex(criterionNumbers, criterionNumber);
        // look at the same index or the one to the left
        int leftIndex = (index == 0) ? goalNumbers.size() - 1 : index - 1;
        int rightIndex = index;
        adjacentGoals.push_back(goalNumbers[leftIndex]);
        adjacentGoals.push_back(goalNumbers[rightIndex]);
    }
    return this->removeDuplicates(adjacentGoals);
}

template<typename T>
std::vector<T> BoardManager::removeDuplicates(std::vector<T> & vec) {
    std::vector<T> uniqueVec;
    std::unordered_map<T, bool> seen;
    for (const T& item : vec) {
        if (!seen[item]) {
            uniqueVec.push_back(item);
            seen[item] = true;
        }
    }
    return uniqueVec;
}

std::vector<HexTile*> BoardManager::getAdjacentTilesForGoal(int goalNumber) {
    Board* board = gameData->getBoard();
    Goal* goal = board->getGoalByNumber(goalNumber);
    // loop through the first parent tile.
    HexTile* tile = goal->getHexTileParents()[0];
    std::vector<int> goalNumbers = tile->getGoalNumbers();
    int index = this->getIndex(goalNumbers, goalNumber);
    Criterion* leftNeighbor = board->getCriterionByNumber(index);
    Criterion* rightNeighbor = board->getCriterionByNumber((index + 1) % 6);
    
    // now loop through parents of left and right neighbors and add them to the list
    std::vector<HexTile*> adjacentTiles;
    for (HexTile* tile : leftNeighbor->getHexTileParents()) {
        adjacentTiles.push_back(tile);
    }
    for (HexTile* tile : rightNeighbor->getHexTileParents()) {
        adjacentTiles.push_back(tile);
    }
    return this->removeDuplicates(adjacentTiles);
}

int BoardManager::getIndex(std::vector<int> & vec, int element) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == element) {
            return i;
        }
    }
    return -1;
}