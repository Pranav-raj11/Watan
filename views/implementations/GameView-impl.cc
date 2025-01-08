module GameView;

std::string GameView::chooseInitialAssignments(PlayerColor studentColor) {
    std::cout << "Student " << playerColorToString(studentColor) << ", where do you want to complete an Assignment?" << std::endl;
    this->printWaitForResponse();
    return this->readInput();
}

void GameView::printHelp() {
    for (EndTurnCommand cmd : allEndTurnCommands()) {
        // dont print this
        if (cmd == EndTurnCommand::INVALID_END_TURN) continue;
        // print everything else
        std::cout << endTurnCommandToString(cmd) << std::endl;
    }
}

std::string GameView::promptTrade(Player* activePlayer, Player* offeringTo, ResourceType give, ResourceType receive) {
    std::cout << playerColorToString(activePlayer->getColor()) << " offers " << playerColorToString(offeringTo->getColor()) << " one " << resourceToString(give) << " for one " << resourceToString(receive) << ". Do you accept?" << std::endl;
    this->printWaitForResponse();
    return this->readInput();
}


void GameView::printError(std::string error) {
    std::cout << error << std::endl;
}

void GameView::printCriteria(std::vector<Criterion*> completed) {
    // print the criteria
    for (Criterion* c : completed) {
        std::cout << c->getObjectiveNumber() << " ";
    }
    std::cout << std::endl;
}


std::string GameView::promptSteal() {
    std::cout << "Choose a student to steal from." << std::endl;
    this->printWaitForResponse();
    return this->readInput();
}

void GameView::printStealOutput(Player* victim, Player* thief, ResourceType stolen) {
    std::cout << "Student " << playerColorToString(thief->getColor()) << " steals " << resourceToString(stolen) << " from student " << playerColorToString(victim->getColor()) << std::endl;
}

void GameView::printStealableOptions(Player* activePlayer, std::vector<Player*> stealables) {
    // if no stealable options, prints this
    if (stealables.size() == 0) {
        std::cout << "Student " << playerColorToString(activePlayer->getColor()) << " has no students to steal from" << std::endl;
        return ;
    }
    // if stealable options
    std::cout << "Student " << playerColorToString(activePlayer->getColor()) << " can choose to steal from ";
    // loop through and print
    for (int i = 0; i < stealables.size(); i++) {
        std::cout << playerColorToString(stealables[i]->getColor());
        // if not last in the list, print commas
        if (i != stealables.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

std::string GameView::promptGeeseMovement() {
    std::cout << "Choose where to place the GEESE." << std::endl;
    this->printWaitForResponse();
    return this->readInput();
}

void GameView::printGeeseLosses(std::unordered_map<Player*, std::unordered_map<ResourceType, int>> losses) {

    // First, calculate the total losses for each player 
    std::unordered_map<Player*, int> totalLosses; 
    for (const auto& playerLosses : losses) { 
        int total = 0; 
        for (const auto& resourceLoss : playerLosses.second) { 
            total += resourceLoss.second; 
        } 
        totalLosses[playerLosses.first] = total; 
    }
    // loop through each player that lost resources
    for (auto it : losses) {
        std::cout << "Student " << playerColorToString(it.first->getColor()) << " loses " << totalLosses[it.first] << " resources to the geese. They lose:";

        std::cout << std::endl;
        // loop through lost resources and print
        for (auto resIt : it.second) {
            // num resource with resource name
            std::cout << resIt.second << " " << resourceToString(resIt.first) << std::endl;
        }
    }
}

std::string GameView::promptPlayAgain(Player* winner) {
    std::cout << "Student " << playerColorToString(winner->getColor()) << " has won!" << std::endl;

    std::cout << "Would you like to play again?" << std::endl;

    this->printWaitForResponse();
    return this->readInput();
}

std::string GameView::readInput() {
    std::string inp;
    std::cin >> inp;
    if (std::cin.eof()) {
        throw EndOfFileException();
    }
    return inp;
}

std::string GameView::playerTurn(Player* player) {
    std::cout << "Student " << playerColorToString(player->getColor()) << "'s turn." << std::endl;
    this->printPlayerStatus(player);
    std::cout << std::endl;
    this->printWaitForResponse();
    return this->readInput();
}

void GameView::printResourcesGained(std::unordered_map<Player*, std::unordered_map<ResourceType, int>> gains) {
    bool someoneGained = false;
    // first check if anyone gained resources
    for (auto it : gains) {
        // if someone gained resources
        if (this->playerGainedResources(it.second)) {
            someoneGained = true;
            break;
        }
    }
    // if noone gained resources, print this and exit 
    if (!someoneGained) {
        std::cout << "No students gained resources" << std::endl;
        return ;
    }
    // now go through and print
    for (auto it : gains) {
        if (this->playerGainedResources(it.second)) {
            std::cout << "Student " << playerColorToString(it.first->getColor()) << " gained:" << std::endl;
            // iterate through the resources and print
            for (auto secondIt : it.second) {
                // dont print if 0
                if (secondIt.second <= 0) continue;
                std::cout << secondIt.second << " " << resourceToString(secondIt.first) << std::endl;
            }
        }
    }
}

std::string GameView::promptLoadedRoll() {
    std::cout << "Input a roll between 2 and 12:" << std::endl;
    this->printWaitForResponse();
    return this->readInput();
}

void GameView::printWaitForResponse() {
    std::cout <<  ">";
}

void GameView::printPlayerStatus(Player* player) {
    std::cout << playerColorToString(player->getColor()) << " has " << player->getNumVictoryPoints() << " victory points, ";

    // loop through all resources and print
    auto resources = allResources();
    for (size_t i = 0; i < resources.size(); i++) {
        std::cout << player->getNumResources(resources[i]) << " " << resourceToString(resources[i]);
        if (i < resources.size() - 1) {
            std::cout << ", ";
        }
    }
}


bool GameView::playerGainedResources(std::unordered_map<ResourceType, int> gains) {
    for (auto it : gains) {
        if (it.second > 0) {
            return true;
        }
    }
    return false;
}