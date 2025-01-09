export module FairDice;
import <memory>;
import types;
import Dice;
import Randomness;

export class FairDice: public Dice {
    public:
        FairDice(std::shared_ptr<RandomGenerator> randomizer);
        DiceType getType() override;
        int roll() override;
};

FairDice::FairDice(std::shared_ptr<RandomGenerator> randomizer) : Dice{randomizer} {}

DiceType FairDice::getType() {
    return DiceType::FAIRDICE;
}

int FairDice::roll() {
    return randomManager->getRandom(1, 6) + randomManager->getRandom(1,6); // roll two dice
}
