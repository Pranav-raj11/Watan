module GenericManager;

GenericManager::GenericManager(std::shared_ptr<ModelFacade> gameData, std::shared_ptr<RandomGenerator> rando): gameData{gameData}, randomizer{rando} {}

