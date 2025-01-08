export module GenericManager;

import <memory>;

import Randomness;
import ModelFacade;

export class GenericManager {
    protected:
        std::shared_ptr<ModelFacade> gameData;
        std::shared_ptr<RandomGenerator> randomizer;

    public:
        GenericManager(std::shared_ptr<ModelFacade> gameData, std::shared_ptr<RandomGenerator> randomizer);
        virtual ~GenericManager() = default;

};
