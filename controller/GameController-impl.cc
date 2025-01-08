module GameController;

GameController::GameController(): gameData{new ModelFacade{}}, randomizer{new RandomGenerator{}}, gameManager{new ManagerFacade{gameData, randomizer}}, display{new ViewProxy{}}, boardFile{""}, gameFile{""} {

}

GameController::GameController(int seed): gameData{new ModelFacade{}}, randomizer{new RandomGenerator{seed}}, gameManager{new ManagerFacade{gameData, randomizer}}, display{new ViewProxy{}}, boardFile{""}, gameFile{""} {

}

void GameController::setBoardFile(std::string filename) {
    boardFile = filename;
}

void GameController::setGameFile(std::string filename) {
    gameFile = filename;
}

void GameController::beginGameSeries() {
    while (true) {
        Player* winner = this->startGame();
        // no winner, so break (perhaps because of EOF)
        if (winner == nullptr) {
            break;
        }
        bool playAgain = display->promptPlayAgain(winner);
        // if no, then break
        if (!playAgain) {
            break;
        }
        // otherwise reset the game and continue
        gameData->resetGame();
    }
}

Player* GameController::startGame() {
    this->initializeGame();
    // just print the board now at the start
    display->printBoard(gameData->getBoard());
    Player* winner = nullptr;

    // begin the game loop
    while (true) {
        PlayerColor color = gameData->getTurn();
        Player* player = gameData->getPlayer(color);
        // prompts user for dice, roll, or loaded dice.
        // also prints statuses and such. runs till user rolls
        this->onPlayerTurn(player);
        // now user rolled. so roll their dice
        int rolled = player->getCurrentDice()->roll();
        if (rolled != GEESE_NUMBER) {
            this->onPlayerRoll(rolled);
        } else {
            this->onGeeseRolled(player);
        }
        this->endOfTurnSequence(player);
        // check if any player has won
        winner = gameManager->checkWin();
        if (winner != nullptr) {
            break; // exit the loop if someone has won
        }
        // update the turn
        PlayerColor nextTurn = static_cast<PlayerColor>((gameData->getTurn() + 1) % NUM_PLAYERS);
        gameData->setTurn(nextTurn);
    }
    return winner;
}

void GameController::onPlayerRoll(int rolled) {
    // a map from players, to a map from resouce type to the amount the player gained
    std::unordered_map<Player*, std::unordered_map<ResourceType, int>> gains{};
    // loop through all players
    for (int i = 0; i < NUM_PLAYERS; i++) {
        Player* player = gameData->getPlayer(static_cast<PlayerColor>(i));
        std::unordered_map<ResourceType, int> gained = gameManager->awardResources(rolled, player);
        // adds it in the list
        gains[player] = gained;
    }
    // now print everything the players gained
    display->printResourcesGained(gains);
}

void GameController::printAllPlayerStatuses() {
    for (PlayerColor col : allPlayerColors()) {
        Player* player = gameData->getPlayer(col);
        display->printPlayerStatus(player);
        std::cout << std::endl;
    }
}

void GameController::onPlayerTurn(Player* player) {
    TurnCommand cmd = display->playerTurn(player);
    while (cmd != TurnCommand::ROLL) {
        // keep looping until decide to roll
        if (cmd == TurnCommand::FAIR) {
            player->changeDice(new FairDice{randomizer});
        } else if (cmd == TurnCommand::LOAD) {
            int roll = display->getLoadedRoll();
            // valid roll, so change the dice
            if (roll > 0) player->changeDice(new WeightedDice{roll, randomizer});
        } else {
            // if user decided to roll, break
            break;
        }
        cmd = display->readTurnCommand();
    }
}

void GameController::onGeeseRolled(Player* activePlayer) {
    std::unordered_map<Player*, std::unordered_map<ResourceType, int>> losses = gameManager->removeResourcesOnGeese();

    display->printGeeseLosses(losses);
    // now prompt the movement of the geese. 
    int newTile = -10;
    // keep asking until newTile is in valid range and NOT the current geese tile
    while (newTile < 0 || newTile >= NUM_TILES || newTile == gameData->getBoard()->getGeeseTile()) {
        newTile = display->promptGeeseMovement();
    }
    // now move the geese to the new tile
    gameManager->moveGeese(newTile);
    // get players that can be stolen from
    std::vector<Player*> stealableFrom = gameManager->getStealablePlayers(newTile, activePlayer);
    // prints stealable options, if they exist
    display->printStealableOptions(activePlayer, stealableFrom);
    // if there are no stealable options, exit
    if (stealableFrom.size() <= 0) return ;
    // now prompt student to choose a person to steal from
    PlayerColor col = display->promptSteal(activePlayer);
    Player* victim = gameData->getPlayer(col);
    ResourceType stolen = gameManager->stealFromPlayer(victim, activePlayer);
    // print the stolen resource to output
    display->printStealOutput(victim, activePlayer, stolen);
}

void GameController::endOfTurnSequence(Player* activePlayer) {
    // prompt the user what they want to do
    EndTurnCommand cmd = EndTurnCommand::INVALID_END_TURN;
    while (cmd != EndTurnCommand::NEXT) {
        cmd = display->endTurnPrompt(activePlayer);
        // print the board
        if (cmd == EndTurnCommand::BOARD) {
            display->printBoard(gameData->getBoard());
        } 
        // print all player statuses
        else if (cmd == EndTurnCommand::STATUS) {
            this->printAllPlayerStatuses();
        } 
        // print all criterions the player has completed
        else if (cmd == EndTurnCommand::CRITERIA) {
            std::vector<int> completed = activePlayer->getCompletedCriterions();
            // convert to criterions
            std::vector<Criterion*> criterions{};
            for (int critNum : completed) {
                Criterion* crit = gameData->getBoard()->getCriterionByNumber(critNum);
                criterions.push_back(crit);
            }
            display->printCriteria(criterions);
        } 
        // Player attempts to complete a criterion
        else if (cmd == EndTurnCommand::COMPLETE) {
            int critNum = display->readInt();
            Criterion* crit = gameData->getBoard()->getCriterionByNumber(critNum);
            try {
                gameManager->completeCriterion(crit, activePlayer);
                // check if any player has won
                Player* winner = gameManager->checkWin();
                if (winner != nullptr) {
                    break; // exit the loop if someone has won
                }
            } catch (std::runtime_error& e) {
                // invalid for whatever reason.
                // invalid location or not enough resources
                display->printError(e.what());
            }
        }
        // Player attempts to achieve a goal
        else if (cmd == EndTurnCommand::ACHIEVE) { 
            int goalNum = display->readInt();
            Goal* goal = gameData->getBoard()->getGoalByNumber(goalNum);
            try {
                gameManager->achieveGoal(goal, activePlayer);
            } catch (std::runtime_error& e) {
                display->printError(e.what());
            }
        } 
        // Player attempts to trade
        else if (cmd == EndTurnCommand::TRADE) {
            this->onTradeCommand(activePlayer);
        }
        // Player attempts to improve a criterion
        else if (cmd == EndTurnCommand::IMPROVE) {
            int critNum = display->readInt();
            Criterion* crit = gameData->getBoard()->getCriterionByNumber(critNum);
            try {
                gameManager->improveCriterion(crit, activePlayer);
            } catch (std::runtime_error& e) {
                display->printError(e.what());
            }
        }
        // print help command
        else if (cmd == EndTurnCommand::HELP) {
            display->printHelp();
        }
        // save the game
        else if (cmd == EndTurnCommand::SAVE) {
            std::string filename = display->readString();
            this->saveGame(filename);
        }
    }
}

void GameController::onTradeCommand(Player* activePlayer) {
    // first read the inputs from the player
    PlayerColor col;
    Player* offeringTo;
    ResourceType give;
    ResourceType receive;
    // read the color and integers
    try {
        col = display->readPlayerColor();
        offeringTo = gameData->getPlayer(col);
        give = display->readResourceType();
        receive = display->readResourceType();
    } catch (std::invalid_argument& e) {
        // if invalid argument, print error and return
        display->printError(e.what());
        return;
    }
    // check that the player has enough resources to give
    if (activePlayer->getNumResources(give) < 1) {
        display->printError("You do not have enough resources.");
        return;
    }
    bool receiverAggreed = false;
    try {
        receiverAggreed = display->promptTrade(activePlayer, offeringTo, give, receive);
    } catch (std::invalid_argument& e) {
        // if invalid argument, print error and return
        display->printError(e.what());
        return;
    }
    if (receiverAggreed) {
        try {
            gameManager->trade(activePlayer, offeringTo, give, receive);
        } catch (std::runtime_error& e) {
            // if the offeringTo player does not have enough resources to give, print error and return
            display->printError(e.what());
            return;
        }
    }
}

std::unordered_map<PlayerColor, std::vector<int>> GameController::setInitialAssignments() {
    std::unordered_map<PlayerColor, std::vector<int>> initAssignments{};
    std::vector<int> chosenCrits{}; // to keep track of chosen criterions

    int direction = 1;
    int i = 0;
    int totalChosen = 0;
    // call game view to prompt the user
    // first go from BLUE to YELLOW
    while (totalChosen < NUM_INITIAL_ASSIGNMENTS) {

        PlayerColor col = static_cast<PlayerColor>(i);
        // keep trying until valid
        int chosen;
        while (true) {
            chosen = display->chooseInitialAssignments(col);
            // check if someone else has already chosen this
            if (std::find(chosenCrits.begin(), chosenCrits.end(), chosen) != chosenCrits.end()) {
                // someone already chose this, try again
                continue;
            }
            break;
        }
        // add the chosen assignments
        initAssignments[col].push_back(chosen);
        chosenCrits.push_back(chosen);
        // increment
        i += direction;
        // if all players chose, go in reverse
        if (i == NUM_PLAYERS) {
            direction *= -1;
            i--;
            totalChosen++;
        } else if (i == -1) {
            direction *= -1;
            i++;
            totalChosen++;
        }
    }
    // return the map;
    return initAssignments;
    
}

void GameController::initializeGame() {
    // checks if we have a gamefile. If so, skip setting initial assignments
    if (gameFile != "") { // have a gamefile. so just load it
        gameManager->loadGame(gameFile);
        gameFile = ""; // do not load the same file if they play again
    } else if (boardFile != "") {
        // have a boardFile. Get initial assignments and load the boardfile
        std::unordered_map<PlayerColor, std::vector<int>> initAssignments = this->setInitialAssignments();
        gameManager->loadGameWithBoard(boardFile, initAssignments);
        boardFile = ""; // do not load the same file if they play again

    } else {
        // neither board nor gamefile. Start a new game
        std::unordered_map<PlayerColor, std::vector<int>> initAssignments = this->setInitialAssignments();
        gameManager->initializeNewGame(initAssignments);
    }
}

void GameController::saveGame(std::string filename) {
    // if the game is not initialized yet, then dont save
    if (gameData->getBoard() == nullptr) return ;
    // save the game
    gameManager->saveGame(filename);
}


void GameController::setSeed(int seed) {
    randomizer->setSeed(seed);
}