module GameStateManager;

GameStateManager::GameStateManager(std::shared_ptr<ModelFacade> data, std::shared_ptr<RandomGenerator> random): GenericManager{data, random} {

}

Player* GameStateManager::checkWin() {
    // go through all players and check if any have won
    for (PlayerColor col : allPlayerColors()) {
        Player* player = gameData->getPlayer(col);
        int numCompletedCriterions = player->getCompletedCriterions().size();
        if (numCompletedCriterions >= REQUIRED_CRITERIONS_FOR_WIN) {
            return player;
        }
    }
    return nullptr;
}

void GameStateManager::saveGame(std::string filename) {
    std::ofstream fs{filename};
    // get the current turn and cast to integer
    int currentTurn = static_cast<int>(gameData->getTurn()); 
    // first save the turn
    fs << currentTurn << " " << std::endl;
    for (PlayerColor color :  allPlayerColors()) {
        Player* player = gameData->getPlayer(color);
        // loop through all resources and print them
        for (ResourceType resource : allResources()) {
            fs << player->getNumResources(resource) << " ";
        }
        
        std::vector<int> goalsComplete = player->getCompletedGoals();
        fs << "g ";
        for (int goal : goalsComplete) {
            fs << goal << " ";
        }

        std::vector<int> criterionsComplete = player->getCompletedCriterions();
        fs << "c ";
  
        Board* board = gameData->getBoard();
        for (int criterion : criterionsComplete) {
            Criterion* critObj = board->getCriterionByNumber(criterion);
            fs << criterion << " " << critObj->getCompletionLevel() << " ";
        }
        // endl after each player
        fs << "\n";
        
    }

    Board* board = gameData->getBoard();
    for (int i = 0; i < NUM_TILES; i++) {
        HexTile* tile = board->getTileAt(i);
        fs << tile->getTileResource() << " " << tile->getTileValue() << " ";
    }
    // endl after the tiles
    fs << std::endl;
    int geeseTile = board->getGeeseTile();
    fs << geeseTile;
    fs << std::endl;

    fs.close();
}

void GameStateManager::loadGame(std::string filename) {
    std::ifstream fs{filename}; // opens the file

    int currentTurn;
    fs >> currentTurn;

    GameBuilder gameBuilder{gameData, randomizer};
    // start by building the players
    gameBuilder.buildPlayers(fs);
    // now build the board
    gameBuilder.buildBoard(fs);
    // set the turn
    gameBuilder.setTurn(currentTurn);
}

void GameStateManager::loadBoard(std::string filename, std::unordered_map<PlayerColor, std::vector<int>> initAssignments) {
    GameBuilder gameBuilder{gameData, randomizer};
    // builds the board
    std::ifstream fs{filename};
    gameBuilder.buildBoard(fs); // loads board from the file
    gameBuilder.buildPlayers(initAssignments); // sets player assignments
    gameBuilder.setTurn(0); // sets to blue (first player's turn)
}

// TODO: complete below
void GameStateManager::initializeNewGame(std::unordered_map<PlayerColor, std::vector<int>> initAssignments) {

    GameBuilder gameBuilder{gameData, randomizer};
    gameBuilder.buildPlayers(initAssignments);
    gameBuilder.buildBoard();
    gameBuilder.setTurn(0);
}