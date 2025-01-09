export module Student;

import <vector>;

import types;
import Player;
import Dice;

export class Student: public Player {
    public:
        /*
        Simple constructor for student with just the student color
        */
       Student(PlayerColor color, Dice* equipped);
        /*
        Used when starting a new game. Sets resource values to 0.
        completedGoals and completedCriterions should be the initial assignments
        the student chose
        */
        Student(PlayerColor color, std::vector<int> completedGoals, std::vector<int> completedCriterions, Dice* fairDice);
        /*
        This method is called when it is the player's turn. Does nothing, since
        we expect the student to enter input into stdin when making a move.
        */
        void onPlayerTurn() override;

        /*
        Use to load an existing game with previously loaded resource values and more
        */
        Student(int caf, int lec, int tut, int stud, int lab, int vp, PlayerColor color, std::vector<int> completedGoals, std::vector<int> completedCriterions, Dice* fairDice);
        
        PlayerType getType() override;
};

Student::Student(PlayerColor col, Dice* equipped): Player{col, equipped} {}

Student::Student(PlayerColor color, std::vector<int> completedGoals, std::vector<int> completedCriterions, Dice* fairDice) 
: Player{color, completedGoals, completedCriterions, fairDice} {

}

Student::Student(int caf, int lec, int tut, int stud, int lab, int vp, PlayerColor color, std::vector<int> completedGoals, std::vector<int> completedCriterions, Dice* fairDice): Player{caf, lec, tut, stud, lab, vp, color, completedGoals, completedCriterions, fairDice} {

}

PlayerType Student::getType() {
    return PlayerType::REAL;
}

void Student::onPlayerTurn() {
    // do nothing
}