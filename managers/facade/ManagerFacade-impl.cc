module ManagerFacade;

ManagerFacade::ManagerFacade(std::shared_ptr<ModelFacade> data, std::shared_ptr<RandomGenerator> randomizer) {
    gameData = data;
    this->randomizer = randomizer;
    gameStateManager = std::make_unique<GameStateManager>(gameData, randomizer);
    resourceManager = std::make_unique<ResourceManager>(gameData, randomizer);
    boardManager = std::make_unique<BoardManager>(gameData, randomizer);
}

std::vector<int> ManagerFacade::getAdjacentCriterions(int objectiveNumber, bool isCriterion) {
    if (isCriterion) {
        return boardManager->getAdjacentCriterionsToCriterion(objectiveNumber);
    } else {
        return boardManager->getAdjacentCriterionsToGoal(objectiveNumber);
    }
}

std::vector<int> ManagerFacade::getAdjacentGoals(int objectiveNumber, bool isCriterion) {
    if (isCriterion) {
        return boardManager->getAdjacentGoalsToCriterion(objectiveNumber);
    } else {
        return boardManager->getAdjacentGoalsToGoal(objectiveNumber);
    }
}

std::vector<Player*> ManagerFacade::getStealablePlayers(int tileNum, Player* activePlayer) {
    std::vector<Player*> stealables{};
    for (PlayerColor col : allPlayerColors()) {
        Player* player = gameData->getPlayer(col);
        // skip if the active player
        if (player->getColor() == activePlayer->getColor()) continue;
        // if nothing on the given tile, then skip
        if (!boardManager->hasCriterionInTile(tileNum, player)) continue;
        // if zero resources, skip
        if (resourceManager->getTotalResources(player) == 0) continue;

        stealables.push_back(player); // otherwise, add it to the list
    }

    return stealables;
}

void ManagerFacade::trade(Player* activePlayer, Player* offeringTo, ResourceType give, ResourceType receive) {
    resourceManager->trade(activePlayer, offeringTo, give, receive);
}

void ManagerFacade::improveCriterion(Criterion* criterion, Player* activePlayer) {
    resourceManager->improveCriterion(criterion, activePlayer);
}

void ManagerFacade::achieveGoal(Goal* goal, Player* activePlayer) {
    // get adjacent criterions and goals
    std::vector<int> adjacentCriterions = boardManager->getAdjacentCriterionsToGoal(goal->getObjectiveNumber());
    std::vector<int> adjacentGoals = boardManager->getAdjacentGoalsToGoal(goal->getObjectiveNumber());
    // convert from integer of Gal and criterion numbers to Criteiron* and GOal*
    std::vector<Criterion*> adjacentCriterionsPtrs{};
    std::vector<Goal*> adjacentGoalsPtrs{};
    Board* board = gameData->getBoard();
    for (int critNum : adjacentCriterions) {
        adjacentCriterionsPtrs.push_back(board->getCriterionByNumber(critNum));
    }
    for (int goalNum : adjacentGoals) {
        adjacentGoalsPtrs.push_back(board->getGoalByNumber(goalNum));
    }
    // this may throw an error
    resourceManager->achieveGoal(goal, activePlayer, adjacentCriterionsPtrs, adjacentGoalsPtrs);
}

void ManagerFacade::completeCriterion(Criterion* criterion, Player* activePlayer) {
    std::vector<int> adjacentCriterions = boardManager->getAdjacentCriterionsToCriterion(criterion->getObjectiveNumber());
    std::vector<int> adjacentGoals = boardManager->getAdjacentGoalsToCriterion(criterion->getObjectiveNumber());
    // convert from integer of Gal and criterion numbers to Criteiron* and GOal*
    std::vector<Criterion*> adjacentCriterionsPtrs{};
    Board* board = gameData->getBoard();
    std::vector<Goal*> adjacentGoalsPtrs{};
    for (int critNum : adjacentCriterions) {
        adjacentCriterionsPtrs.push_back(board->getCriterionByNumber(critNum));
    }
    for (int goalNum : adjacentGoals) {
        adjacentGoalsPtrs.push_back(board->getGoalByNumber(goalNum));
    }
    // this may throw an error
    resourceManager->completeCriterion(criterion, activePlayer, adjacentCriterionsPtrs, adjacentGoalsPtrs);
}

Player* ManagerFacade::checkWin() {
    return gameStateManager->checkWin();
}

void ManagerFacade::saveGame(std::string filename) {
    gameStateManager->saveGame(filename);
}

void ManagerFacade::initializeNewGame(std::unordered_map<PlayerColor, std::vector<int>> initAssignments) {
    gameStateManager->initializeNewGame(initAssignments);
}

void ManagerFacade::moveGeese(int newTile) {
    boardManager->moveGeese(newTile);
}

void ManagerFacade::loadGame(std::string filename) {
    gameStateManager->loadGame(filename);
}

void ManagerFacade::loadGameWithBoard(std::string filename, std::unordered_map<PlayerColor, std::vector<int>> initAssignments) {
    // loads the board, and sets player initial assignments
    gameStateManager->loadBoard(filename, initAssignments);
}

std::unordered_map<ResourceType, int> ManagerFacade::awardResources(int rolled, Player* player) {
    // adds resource to the player
    return resourceManager->awardResourceOnRoll(rolled, player);
}

std::unordered_map<Player*, std::unordered_map<ResourceType, int>> ManagerFacade::removeResourcesOnGeese() {
    std::unordered_map<Player*, std::unordered_map<ResourceType, int>> res {};

    // iterate through players more than 10 resources
    for (Player* player : resourceManager->getStudentsWithResources(RESOURCE_REQUIRED_ON_GEESE)) {
        // get half (floored)
        int resourcesToDelete = resourceManager->getTotalResources(player) / 2;
        // now iterate and randomly delete
        for (int i = 0; i < resourcesToDelete; i++) {
            ResourceType deleted = resourceManager->deleteRandomResource(player);
            // add it in the map. add (deleted, 0) if deleted does not exist
            res[player].emplace(deleted, 0).first->second += 1;
        }
    }
    return res;
}

ResourceType ManagerFacade::stealFromPlayer(Player* stealingFrom, Player* thief) {
   ResourceType stolen = resourceManager->deleteRandomResource(stealingFrom); // steal from victim
   thief->incrementResource(stolen, 1); // adds to thief 
   return stolen; // return stolen type
}
