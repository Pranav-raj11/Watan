module ResourceManager;

ResourceManager::ResourceManager(std::shared_ptr<ModelFacade> data, std::shared_ptr<RandomGenerator> random): GenericManager{data, random} {}

std::unordered_map<ResourceType, int> ResourceManager::awardResourceOnRoll(int rolledNumber, Player* player) {
    // initialize
    std::unordered_map<ResourceType, int> resourceCount = {
        {ResourceType::CAFFEINE, 0},
        {ResourceType::STUDY, 0},
        {ResourceType::TUTORIAL, 0},
        {ResourceType::LAB, 0},
        {ResourceType::LECTURE, 0}
    };
    Board* board = gameData->getBoard();
    int geeseTile = board->getGeeseTile();
    // loop through all hex tiles the player has completed and check if the tile value matches the roll and geese is NOT on the tile
    for (int critNum : player->getCompletedCriterions()) {
        // get the criterion
        Criterion* obj = board->getCriterionByNumber(critNum);
        // check all tiles this criterian is adjacent to now
        for (HexTile* parentTile : obj->getHexTileParents()) {
            int tileNum = parentTile->getTileNumber();
            if (tileNum == geeseTile) continue;
            int tileValue = parentTile->getTileValue();
            if (tileValue != rolledNumber) continue;
            // now reward resources based on resource in the tile
            ResourceType tileResource = parentTile->getTileResource();
            if (tileResource == ResourceType::NETFLIX) continue;
            // increment the number of resources
            int award = getResourceAward(tileResource, obj->getCompletionLevel());
            resourceCount[tileResource] += award;
            player->incrementResource(tileResource, award);
        }
    }

    return resourceCount;
}

void ResourceManager::improveCriterion(Criterion* criterion, Player* activePlayer) {
    // first get current criterion level
    CriterionLevel currentLevel = criterion->getCompletionLevel();
    // now check that this player is the owner of the criterion
    if (criterion->getOwner() != activePlayer) {
        throw std::runtime_error("You cannot build here.");
    }
    // now check that the next level is not the maximum level
    if (currentLevel == CriterionLevel::EXAM) {
        throw std::runtime_error("You cannot build here.");
    }
    // now get the resource requirement for the next level
    CriterionLevel nextLevel = nextCompletionLevel(currentLevel);
    ResourceRequirement req = getCriterionResourceRequirement(nextLevel);
    // check if the player has enough resources
    if (!hasEnoughResources(activePlayer, req)) {
        throw std::runtime_error("You do not have enough resources.");
    }
    // now delete the resources
    ResourceRequirement negReq = -req;
    incrementResources(activePlayer, negReq);
    // upgrade criterion level
    // this also increments the owner's VP by 1
    criterion->setCompletion(nextLevel);
}

void ResourceManager::trade(Player* activePlayer, Player* offeringTo, ResourceType give, ResourceType receive) {
    // check that the offering player has enough resources to give
    if (activePlayer->getNumResources(give) < 1 || offeringTo->getNumResources(receive) < 1) {
        throw std::runtime_error("You do not have enough resources.");
    }
    // now delete the resources
    activePlayer->incrementResource(give, -1);
    offeringTo->incrementResource(receive, -1);
    // now add the resources
    activePlayer->incrementResource(receive, 1);
    offeringTo->incrementResource(give, 1);
}

void ResourceManager::achieveGoal(Goal* goal, Player* activePlayer, std::vector<Criterion*> adjacentCriterions, std::vector<Goal*> adjacentGoals) {
    ResourceRequirement req = getGoalResourceRequirement();
    // now ensure that the player has completed either an adjacent criterion or an adjacent goal
    bool criterionCompleted = false;
    bool goalCompleted = false;
    for (Criterion* crit : adjacentCriterions) {
        if (crit->getOwner() == activePlayer) {
            criterionCompleted = true;
            break;
        }
    }
    for (Goal* goal : adjacentGoals) {
        if (goal->getOwner() == activePlayer) {
            goalCompleted = true;
            break;
        }
    }
    if (!criterionCompleted && !goalCompleted) {
        throw std::runtime_error("You cannot build here.");
    }
    // check if the player has enough resources
    if (!hasEnoughResources(activePlayer, req)) {
        throw std::runtime_error("You do not have enough resources.");
    }
    // now delete the resources
    ResourceRequirement negReq = -req;
    incrementResources(activePlayer, negReq);
    // now complete the goal
    goal->complete(activePlayer);
}

void ResourceManager::completeCriterion(Criterion* criterion, Player* activePlayer, std::vector<Criterion*> adjacentCriterions, std::vector<Goal*> adjacentGoals) {

    // now ensure no adjacent criterions are completed AND an adjacent goal is achieved by this player
    for (Criterion* crit : adjacentCriterions) {
        if (crit->getOwner() != nullptr) {
            throw std::runtime_error("You cannot build here.");
        }
    }
    bool goalAchieved = false;
    for (Goal* goal : adjacentGoals) {
        if (goal->getOwner() == activePlayer) {
            goalAchieved = true;
        }
    }
    if (!goalAchieved) {
        throw std::runtime_error("You cannot build here.");
    }
    
    // get the resource requirement
    ResourceRequirement req = getCriterionResourceRequirement(CriterionLevel::ASSIGNMENT);
    // check if the player has enough resources
    if (!hasEnoughResources(activePlayer, req)) {
        throw std::runtime_error("You do not have enough resources.");
    }
    // now delete the resources
    ResourceRequirement negReq = -req;
    incrementResources(activePlayer, negReq);
    // now complete the criterion
    criterion->complete(activePlayer);
}

std::vector<Player*> ResourceManager::getStudentsWithResources(int n) {
    std::vector<Player*> players;
    // loop through all players
    for (PlayerColor col : allPlayerColors()) {
        Player* player = gameData->getPlayer(col);
        // if more than or equal to required resources
        if (this->getTotalResources(player) >= n) {
            players.push_back(player);
        }
    }
    return players;
}

int ResourceManager::getTotalResources(Player* player) {
    int total = 0;
    // loop through all resources and count
    for (ResourceType res: allResources()) {
        total += player->getNumResources(res);
    }
    return total;
}

ResourceType ResourceManager::deleteRandomResource(Player* player) {
    // get the total number of resources the player has
    int totalResources = this->getTotalResources(player);
    // if no resources, return netflix
    if (totalResources == 0) return ResourceType::NETFLIX;
    // get probabilities numerator for each resource
    std::vector<int> numerators{};
    for (ResourceType res : allResources()) {
        // add number of resources the player owns of this kind
        numerators.push_back(player->getNumResources(res));
    }
    int idx = randomizer->getRandomIndex(numerators, totalResources);
    ResourceType res = static_cast<ResourceType>(idx);
    // now delete resource from player
    player->incrementResource(res, -1);
    // cast and return
    return res;

}

bool ResourceManager::hasEnoughResources(Player* player, ResourceRequirement req) {
    return player->getNumResources(ResourceType::CAFFEINE) >= req.numCaffeine &&
        player->getNumResources(ResourceType::STUDY) >= req.numStudy &&
        player->getNumResources(ResourceType::TUTORIAL) >= req.numTutorial &&
        player->getNumResources(ResourceType::LAB) >= req.numLab &&
        player->getNumResources(ResourceType::LECTURE) >= req.numLecture;
}

void ResourceManager::incrementResources(Player* player, ResourceRequirement req) {
    player->incrementResource(ResourceType::CAFFEINE, req.numCaffeine);
    player->incrementResource(ResourceType::STUDY, req.numStudy);
    player->incrementResource(ResourceType::TUTORIAL, req.numTutorial);
    player->incrementResource(ResourceType::LAB, req.numLab);
    player->incrementResource(ResourceType::LECTURE, req.numLecture);
}
