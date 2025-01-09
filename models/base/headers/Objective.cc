export module Objective;

import <stdexcept>;
import <string>;
import <vector>;

import GenericModel;
import Player;
import HexTile;

export class Objective: public GenericModel {
    protected:
        Player* owner; // the person who completed this objective. nullptr if no one completed
        int objectiveNumber; // eg the goal number of criterion number

        std::vector<HexTile*> parents; // list of HexTile this objective is on
    public:
        /*
        Construct the objective with no owner set. Used for
        starting a new game
        */
        Objective(int objectiveNumber);
        /*
        Construct the objective with owner set. Used for 
        loading an existing game
        */
        Objective(int objectiveNumber, Player* owner);

        virtual ~Objective();
        /*
        Returns the objective number
        */
        int getObjectiveNumber();
        /*
        Returns the Player* who completed this objective
        */
        Player* getOwner();
        /*
        Completes this objective for the given player. If already completed, throws an error (std::runtime_error with message "You cannot build here.")
        1. Also calls on the player's addCompletedGoal or addCompletedCriterion
        to update the player's list as well
        */
        void complete(Player* player);
        /*
        Gets the type of objective
        ---
        Returns
            1. "Goal" is this objective is a goal
            2. "Criterion" if this objective is a criterion
        */
        virtual std::string getType() = 0;

        /*
        Adds a hextile parent (i.e the intersection where this objective is located)
        in the hextile
        */
        void addHexTileParent(HexTile* tile);
        /*
        Gets the parents of the objective
        */
        std::vector<HexTile*> getHexTileParents();

    protected:
        /*
        Called when adding an owner to this objective.
        1. Should update the player's properties (such as completed goals / criterions)
        depending on the descendants' recipe
        */
       virtual void updatePlayer(int objectiveNumber, Player* player) = 0;
};


Objective::Objective(int objectiveNumber) : owner{nullptr}, objectiveNumber{objectiveNumber}, parents{} {}

Objective::Objective(int objectiveNumber, Player* owner) : owner{owner}, objectiveNumber{objectiveNumber}, parents{} {}


Objective::~Objective() {
    // not responsible for deleting player or hex tiles
}

int Objective::getObjectiveNumber() {
    return objectiveNumber;
}

std::vector<HexTile*> Objective::getHexTileParents() {
    return parents;
}

void Objective::addHexTileParent(HexTile* tile) {
    parents.push_back(tile);
}

Player* Objective::getOwner() {return owner;}

void Objective::complete(Player* player) {
    if (owner == nullptr) { // nobody already completed
        owner = player;
        this->updatePlayer(objectiveNumber, player);
    }
    else {
        throw std::runtime_error("You cannot build here.");
    }
}
