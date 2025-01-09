export module Criterion;
import <stdexcept>;
import <string>;

import types;
import Objective;
import Player;

export class Criterion: public Objective {
    CriterionLevel completionLevel;
    public:
        /*
        Creates a new instance wthout owner set. Completion level is set to NONE
        */
        Criterion(int objectiveNumber);
        /*
        Creates a new instance with all properties below set. Used for loading an existing
        game
        */
        Criterion(int objectiveNumber, CriterionLevel completionLevel, Player* owner);
        /*
        Returns the type of the objective ("Criterion" in this case)
        */
        std::string getType() override;
        CriterionLevel getCompletionLevel();

        /*
        Sets completion level of the criterion
        */
        void setCompletion(CriterionLevel newLevel);
    protected:
        /*
        Update the player's criterions list
        */
        void updatePlayer(int objNum, Player* player) override;
        /*
        Loads the criterions into the provided reference vector.
        1. ref: th reference to the vector that is to be modified
        2. ifstream: the stream object to read data from
        */
};

Criterion::Criterion(int objectiveNumber): Objective{objectiveNumber}, completionLevel{CriterionLevel::NONE} {

}

Criterion::Criterion(int objectiveNumber, CriterionLevel completionLevel, Player* owner): Objective{objectiveNumber, owner}, completionLevel{completionLevel} {

}

std::string Criterion::getType() {
    return "Criterion";
}

CriterionLevel Criterion::getCompletionLevel() {
    return completionLevel;
}

void Criterion::updatePlayer(int objNum, Player* player) {
    player->addCompletedCriterion(objNum);
    player->incrementVP(1);
    // updates completion level to assignment (lowest level)
    completionLevel = CriterionLevel::ASSIGNMENT;
}

void Criterion::setCompletion(CriterionLevel newLevel) {
    // get the difference between the new level and the current level
    int diff = static_cast<int>(newLevel) - static_cast<int>(completionLevel);
    completionLevel = newLevel;
    // increments the owner's VP by the difference
    owner->incrementVP(diff);
}
