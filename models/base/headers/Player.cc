export module Player;
import <vector>;
import types;

import GenericModel;
import Dice;


export class Player: public GenericModel {
    int numCaffiene, numLectures, numTut, numStudy, numLab;
    int victoryPoints;
    PlayerColor color;
    std::vector<int> completedGoals;
    std::vector<int> completedCriterions;

    Dice* equippedDice;

    public:
        /*
        A simplier constructor that just initializes the player with
        their color
        */
       Player(PlayerColor color, Dice* equipped);
        /*
        Use to construct a new player for a new game with the given color, goals, and criterions. The goals and criterions should be the initial assignments the
        player chose to complete at the start of the game
        */
        Player(PlayerColor color, std::vector<int> completedGoals, std::vector<int> completedCriterions, Dice* equipped); // initialize from scratch
        /*
        Construct a new player while filling all values.
        Used when loading an existing game
        */
        Player(int caf, int lec, int tut, int stud, int lab, int vp, PlayerColor color, std::vector<int> completedGoals, std::vector<int> completedCriterions, Dice* equipped);

        virtual ~Player();
        /*
        increments victory points by the given integer
        */
        void incrementVP(int toAdd);
        void incrementCaffiene(int toAdd);

        void incrementLab(int toAdd);

        void incrementLectures(int toAdd);
        void incrementTut(int toAdd);
        void incrementStudy(int toAdd);

        /*
        Adds the provided resource type by the increment amount. Same
        functionality as other increment methods, except now under one common method
        */
        void incrementResource(ResourceType type, int toAdd);
        /*
        Get the number of resources the player has of the given resource type
        */
        int getNumResources(ResourceType resourceType);

        /*
        Return the number of victory points the player has
        */
        int getNumVictoryPoints();
        /*
        Get the current equipped Dice
        */
        Dice* getCurrentDice();
        /*
        Returns the color of the player 
        */
        PlayerColor getColor();
        /*
        Returns the type of player 
        (included for extensibility, so computer players can be added)
        */
        virtual  PlayerType getType() = 0;
        /*
        This method is called when it is the player's turn
        */
        virtual void onPlayerTurn() = 0;

        /*
        Adds a goal to the list of completions to the player.
        **This will be used by Objective when completing / achieving the objective
        */
        void addCompletedGoal(int goal);
        /*
        Adds a completed criterion to the player's list
        **This will be used by Objective when completing / achieving the objective
        */
        void addCompletedCriterion(int crit);
        /*
        Changes the active dice
        */
        void changeDice(Dice* newDice);
        /*
        Gets the completed criterions (the ones the player has completed)
        in the form of the criterion numbers
        */
       std::vector<int> getCompletedCriterions();
       /*
       Gets the completed goals (the ones the player has completed)
       in the form of the goal numbers
       */
       std::vector<int> getCompletedGoals();
}; 


Player::Player(PlayerColor color, Dice* equippedDice): numCaffiene{0}, numLectures{0}, numTut{0}, numStudy{0}, numLab{0}, victoryPoints{0}, color{color}, completedGoals{}, completedCriterions{}, equippedDice{equippedDice} {

}

Player::Player(PlayerColor color, std::vector<int> completedGoals, std::vector<int> completedCriterions, Dice* equipped): 
numCaffiene{0}, numLectures{0}, numTut{0}, numStudy{0}, numLab{0}, victoryPoints{0}, color{color}, 
completedGoals{completedGoals}, completedCriterions{completedCriterions}, equippedDice{equipped} {

}

Player::Player(int caf, int lec, int tut, int stud, int lab, int vp, PlayerColor color, std::vector<int> completedGoals, std::vector<int> completedCriterions, Dice* equipped) : 
numCaffiene{caf}, numLectures{lec}, numTut{tut}, numStudy{stud}, numLab{lab}, victoryPoints{vp}, color{color}, completedGoals{completedGoals}, completedCriterions{completedCriterions}, equippedDice{equipped} {

}

Player::~Player() {
    delete equippedDice;
}

std::vector<int> Player::getCompletedCriterions() {
    return completedCriterions;
}

std::vector<int> Player::getCompletedGoals() {
    return completedGoals;
}

void Player::incrementVP(int toAdd) {victoryPoints+=toAdd;}
void Player::incrementCaffiene(int toAdd) {numCaffiene+=toAdd;}
void Player::incrementLectures(int toAdd) {numLectures+=toAdd;}
void Player::incrementTut(int toAdd) {numTut+=toAdd;}
void Player::incrementStudy(int toAdd) {numStudy+=toAdd;}
void Player::incrementLab(int toAdd) {numLab+=toAdd;}

void Player::incrementResource(ResourceType type, int toAdd) {
    if (type == ResourceType::CAFFEINE) {
        this->incrementCaffiene(toAdd);
        return ;
    } else if (type == ResourceType::LECTURE) {
        this->incrementLectures(toAdd);
        return ;
    } else if (type == ResourceType::LAB) {
        this->incrementLab(toAdd);
    } else if (type == ResourceType::STUDY) {
        this->incrementStudy(toAdd);
    } else {
        // tutorial
        this->incrementTut(toAdd);
    }
}

int Player::getNumResources(ResourceType resourceType) {
    // return number of resources based on the requested type
    if (resourceType == ResourceType::CAFFEINE) {
        return numCaffiene;
    } else if (resourceType == ResourceType::LECTURE) {
        return numLectures;
    } else if (resourceType == ResourceType::LAB) {
        return numLab;
    } else if (resourceType == ResourceType::STUDY) {
        return numStudy;
    }
    return numTut;
}

int Player::getNumVictoryPoints() {
    return victoryPoints;
}

Dice* Player::getCurrentDice() {
    return equippedDice;
}

PlayerColor Player::getColor() {
    return color;
}

PlayerType Player::getType() {
    return PlayerType::REAL;
}

void Player::addCompletedGoal(int goal) {
    completedGoals.push_back(goal);
}

void Player::addCompletedCriterion(int crit) {
    completedCriterions.push_back(crit);
}

void Player::changeDice(Dice* newDice) {
    delete equippedDice;
    equippedDice = newDice;
}
