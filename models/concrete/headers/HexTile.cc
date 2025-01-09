export module HexTile;

import <vector>;

import types;

import GenericModel;

export class HexTile: public GenericModel {
    ResourceType tileResource; // the resource on the tile
    int tileNumber;
    int tileValue; // if this number rolled, players with criterions get rewards
    std::vector<int> criterionNumbers;
    std::vector<int> goalNumbers;

    public:
        /*
        Creates the tile with the given resource, tileNumber, tileValue.
        Also provide the list of Goal* and Criterion* that are on the tile
        */
        HexTile(ResourceType tileResource, int tileNumber, int tileValue
        );
        /*
        Adds the given criterion number to the list of criterion numbers
        */
        void addCriterionNumber(int criterionNumber);
        /*
        Adds the given goal number to the list of goal numbers
        */
        void addGoalNumber(int goalNumber);
        /*
        Returns the resource offered by the tile
        */
        ResourceType getTileResource();

        // /*
        // Awards players who have completed a criterion in this tile instance,
        // based on the level of completion.
        // ---
        // Eg: If completion is Assignment, 1 resource
        // If Midterm, 2 resources
        // If Exam, 3 resources
        // */
        // void awardCriterions();

        int getTileNumber();
        int getTileValue();
        /*
        Sorts the criterion and goal numbers in circular order
        eg: top left is 0, then bottom left is last in the list, so 
        we can encode the position of the criterion or goal in the tile
        */
        void sortCriterionAndGoalNumbers();
        /*
        Gets the adjacent criterions to the given criterion number
        */
        std::vector<int> getCriterionNumbers();
        /*
        Gets the adjacent goals to the given goal number
        */
        std::vector<int> getGoalNumbers();

    private:
        /*
        Determines the number of resource to reward a player
        based on the provided completion level of a criterion
        */
        int numResourcesByCompletion(CriterionLevel level);
};

HexTile::HexTile(ResourceType tileResource, int tileNumber, int tileValue): tileResource{tileResource}, tileNumber{tileNumber}, tileValue{tileValue} {

}

std::vector<int> HexTile::getCriterionNumbers() {
    return criterionNumbers;
}

std::vector<int> HexTile::getGoalNumbers() {
    return goalNumbers;
}

void HexTile::sortCriterionAndGoalNumbers() {
    // Reorder criterions in order: 0,1,3,5,4,2
    // this is so the ordering is circular in the tile
    std::vector<int> orderedCrits = {
        criterionNumbers[0],
        criterionNumbers[1], 
        criterionNumbers[3],
        criterionNumbers[5],
        criterionNumbers[4],
        criterionNumbers[2]
    };
    criterionNumbers = orderedCrits;

    // Reorder goals in order: 0,2,4,5,3,1
    // this is so the ordering is circular in the tile
    std::vector<int> orderedGoals = {
        goalNumbers[0],
        goalNumbers[2],
        goalNumbers[4], 
        goalNumbers[5],
        goalNumbers[3],
        goalNumbers[1]
    };
    goalNumbers = orderedGoals;
}

void HexTile::addCriterionNumber(int criterionNumber) {
    criterionNumbers.push_back(criterionNumber);
}

void HexTile::addGoalNumber(int goalNumber) {
    goalNumbers.push_back(goalNumber);
}

ResourceType HexTile::getTileResource() {
    return tileResource;
}


int HexTile::numResourcesByCompletion(CriterionLevel level) {
    switch (level) {
        case CriterionLevel::ASSIGNMENT:
            return 1;
        case CriterionLevel::MIDTERM:
            return 2;
        case CriterionLevel::EXAM:
            return 3;
        default:
            return 0;
    }
}

int HexTile::getTileNumber() {
    return tileNumber;
}

int HexTile::getTileValue() {
    return tileValue;
}
