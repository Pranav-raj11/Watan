export module Goal;

import <string>;

import Objective;
import Player;

export class Goal: public Objective {
    public:
        Goal(int objectiveNumber);

        Goal(int objectiveNumber, Player* owner);
        std::string getType() override;
    protected:
        /*
        Update the player's criterions list
        */
        void updatePlayer(int objNum, Player* player) override;
};

Goal::Goal(int objectiveNumber): Objective{objectiveNumber} {

}

Goal::Goal(int objectiveNumber, Player* owner): Objective{objectiveNumber, owner} {
    
}

std::string Goal::getType() {
    return "Goal";
}

void Goal::updatePlayer(int objNum, Player* player) {
    player->addCompletedGoal(objNum);
    player->incrementVP(1); // increment victory points
}
