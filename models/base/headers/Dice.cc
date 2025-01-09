export module Dice;
import <memory>;
import Randomness;

import types;


export class Dice {
    protected:
        std::shared_ptr<RandomGenerator> randomManager;
    public:
        Dice(std::shared_ptr<RandomGenerator> randomerizer);

        virtual ~Dice();

        /*
        Gets the type of the dice
        */
        virtual DiceType getType() = 0;
        virtual int roll() = 0;
};

Dice::Dice(std::shared_ptr<RandomGenerator> randomizer) : randomManager{randomizer} {}

Dice::~Dice() {}
