export module WeightedDice;

import <memory>;
import Randomness;
import types;
import Dice;

export class WeightedDice: public Dice {
    int desiredNumber;
    public:
        WeightedDice(int desiredNumber, std::shared_ptr<RandomGenerator> randomizer);
        DiceType getType() override;
        int roll() override;
};

WeightedDice::WeightedDice(int desiredNumber, std::shared_ptr<RandomGenerator> randomizer): Dice{randomizer}, desiredNumber{desiredNumber} {}

DiceType WeightedDice::getType() {
    return DiceType::WEIGHTED;
}

int WeightedDice::roll() {
    return desiredNumber;
}
