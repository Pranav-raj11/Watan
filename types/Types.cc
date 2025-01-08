export module types;
import <vector>;
import <string>;
import <unordered_map>;

/* see https://stackoverflow.com/questions/70456868/vector-in-c-module-causes-useless-bad-file-data-gcc-output 
- If this namespace is not used, says "Bad file data" while importing this module
- A bug with g++
*/
namespace std _GLIBCXX_VISIBILITY(default){}

export enum PlayerColor {
    BLUE,
    RED,
    ORANGE,
    YELLOW,
    INVALID
};

export enum ResourceType {
    CAFFEINE,
    LAB,
    LECTURE,
    STUDY,
    TUTORIAL,
    NETFLIX
};
// number of each resources that should be present in the board at the start of the game
export const std::unordered_map<ResourceType, int> initialResourceCount{
    {ResourceType::CAFFEINE, 4},
    {ResourceType::LAB, 4},
    {ResourceType::LECTURE, 4},
    {ResourceType::STUDY, 3},
    {ResourceType::TUTORIAL, 3},
    {ResourceType::NETFLIX, 1}
};
// number of each tile value that should be present in the board at the start of the game
export const std::unordered_map<int, int> initialNumTileValues{
    {2, 1},
    {3, 2},
    {4, 2},
    {5, 2},
    {6, 2},
    {7, 1}, // this corresponds to netflix, though it is not used whatsoever. Required to have probabilities that add up to 1
    {8, 2},
    {9, 2},
    {10, 2},
    {11, 2},
    {12, 1}
};

export enum PlayerType {
    REAL,
    COMPUTER
};

export enum CriterionLevel {
    NONE,
    ASSIGNMENT,
    MIDTERM,
    EXAM
};

export enum TurnCommand {
    LOAD, // loaded dice
    FAIR, // fair dice
    ROLL, // roll the dice
    INVALID_CHOICE
};

export enum EndTurnCommand {
    BOARD,
    STATUS,
    CRITERIA,
    ACHIEVE,
    COMPLETE,
    IMPROVE,
    TRADE,
    NEXT,
    SAVE,
    HELP,
    INVALID_END_TURN
};

export enum DiceType {
    FAIRDICE,
    WEIGHTED
};

export const int NUM_PLAYERS = 4;
export const int NUM_TILES = 19;
export const int NUM_CRITERIONS = 54;
export const int NUM_GOALS = 72;
export const int NUM_INITIAL_ASSIGNMENTS = 2;
export const int REQUIRED_CRITERIONS_FOR_WIN = 10; // required number of (unique) criterions for a player to win
export const int GEESE_NUMBER = 7;
export const int RESOURCE_REQUIRED_ON_GEESE = 10; // min resources needed to lose resources after a geese is rolled

// stores number of criterions per rw in the board. Each row is any row that contains a number in the board
// 0's sandwiched in between since there are rows with only goals
export const std::vector<int> numCriterionsPerRow{ 
    2, 0,
    4, 0,
    6, 0,
    6, 0,
    6, 0,
    6, 0,
    6, 0,
    6, 0,
    6, 0,
    4, 0,
    2
};
// similar deinition as above
export const std::vector<int> numGoalsPerRow {
    1, 2, 2, 4, 3, 6, 2, 6, 3, 6, 2, 6, 3, 6, 2, 6, 3, 4, 2, 2, 1
};


export class ResourceRequirement {
    public:
        int numCaffeine;
        int numLab;
        int numLecture;
        int numStudy;
        int numTutorial;

        ResourceRequirement(int c, int l, int lec, int s, int t) : numCaffeine(c), numLab(l), numLecture(lec), numStudy(s), numTutorial(t) {}

        ResourceRequirement operator-() const;
};
/*
Returns the negated resource requirement
*/
ResourceRequirement ResourceRequirement::operator-() const {
    return ResourceRequirement{-numCaffeine, -numLab, -numLecture, -numStudy, -numTutorial};
}

export std::vector<EndTurnCommand> allEndTurnCommands() {
    return std::vector<EndTurnCommand>{EndTurnCommand::BOARD, EndTurnCommand::STATUS, EndTurnCommand::CRITERIA, EndTurnCommand::ACHIEVE, EndTurnCommand::COMPLETE, EndTurnCommand::IMPROVE, EndTurnCommand::TRADE, EndTurnCommand::NEXT, EndTurnCommand::SAVE, EndTurnCommand::HELP};
}

export ResourceRequirement getCriterionResourceRequirement(CriterionLevel lev) {
    switch (lev) {
        case CriterionLevel::ASSIGNMENT: return ResourceRequirement{1, 1, 1, 0, 1};
        case CriterionLevel::MIDTERM: return ResourceRequirement{0, 0, 2, 3, 0};
        case CriterionLevel::EXAM: return ResourceRequirement{3, 2, 2, 2, 1};
        case CriterionLevel::NONE: return ResourceRequirement{0, 0, 0, 0, 0};
    }
}

export ResourceRequirement getGoalResourceRequirement() {
    return ResourceRequirement{0, 0, 0, 1, 1};
}

export TurnCommand stringToTurnCommand(std::string cmd) {
    if (cmd == "load") return TurnCommand::LOAD;
    if (cmd == "fair") return TurnCommand::FAIR;
    if (cmd == "roll") return TurnCommand::ROLL;
    return TurnCommand::INVALID_CHOICE;
}

export EndTurnCommand stringToEndTurnCommand(std::string cmd) {
    if (cmd == "board") return EndTurnCommand::BOARD;
    if (cmd == "status") return EndTurnCommand::STATUS;
    if (cmd == "criteria") return EndTurnCommand::CRITERIA;
    if (cmd == "achieve") return EndTurnCommand::ACHIEVE;
    if (cmd == "complete") return EndTurnCommand::COMPLETE;
    if (cmd == "improve") return EndTurnCommand::IMPROVE;
    if (cmd == "trade") return EndTurnCommand::TRADE;
    if (cmd == "next") return EndTurnCommand::NEXT;
    if (cmd == "save") return EndTurnCommand::SAVE;
    if (cmd == "help") return EndTurnCommand::HELP;
    return EndTurnCommand::INVALID_END_TURN;
}

export std::string endTurnCommandToString(EndTurnCommand cmd) {
    switch (cmd) {
        case EndTurnCommand::BOARD: return "board";
        case EndTurnCommand::STATUS: return "status";
        case EndTurnCommand::CRITERIA: return "criteria";
        case EndTurnCommand::ACHIEVE: return "achieve <goal>";
        case EndTurnCommand::COMPLETE: return "complete <criterion>";
        case EndTurnCommand::IMPROVE: return "improve <criterion>";
        case EndTurnCommand::TRADE: return "trade <color> <give> <take>";
        case EndTurnCommand::NEXT: return "next";
        case EndTurnCommand::SAVE: return "save <file>";
        case EndTurnCommand::HELP: return "help";
        case EndTurnCommand::INVALID_END_TURN: return "Invalid command.";
    }
}

export int getResourceAward(ResourceType type, CriterionLevel lev) {
    switch (lev) {
        case CriterionLevel::NONE: return 0;
        case CriterionLevel::ASSIGNMENT: return 1;
        case CriterionLevel::MIDTERM: return 2;
        case CriterionLevel::EXAM: return 3;
    }
}

export std::vector<PlayerColor> allPlayerColors() {
    return std::vector<PlayerColor>{PlayerColor::BLUE, PlayerColor::RED, PlayerColor::ORANGE, PlayerColor::YELLOW};
}

export std::vector<ResourceType> allResources() {
    return std::vector<ResourceType>{ResourceType::CAFFEINE,ResourceType::LAB,
        ResourceType::LECTURE, ResourceType::STUDY, ResourceType::TUTORIAL
    };
}

export int numResourceTypes() {
    return 6;
}

export ResourceType stringToResourceType(std::string res) {
    if (res == "caffeine") return ResourceType::CAFFEINE;
    if (res == "lab") return ResourceType::LAB;
    if (res == "lecture") return ResourceType::LECTURE;
    if (res == "study") return ResourceType::STUDY;
    if (res == "tutorial") return ResourceType::TUTORIAL;
    return ResourceType::NETFLIX;
}

export std::string resourceToString(ResourceType res) {
    switch (res) {
        case ResourceType::CAFFEINE: return "CAFFEINE";
        case ResourceType::LAB: return "LAB";
        case ResourceType::LECTURE: return "LECTURE";
        case ResourceType::STUDY: return "STUDY";
        case ResourceType::TUTORIAL: return "TUTORIAL";
        case ResourceType::NETFLIX: return "NETFLIX";
    }
}

export char completionLevelToChar(CriterionLevel lev) {
    switch (lev) {
        case CriterionLevel::ASSIGNMENT: return 'A';
        case CriterionLevel::MIDTERM: return 'M';
        case CriterionLevel::EXAM: return 'E';
        case CriterionLevel::NONE: return 'N';
    }
}

/*
Returns the next completion level. Requires that the level is not the
 maximum level (i.e exam)
*/
export CriterionLevel nextCompletionLevel(CriterionLevel lev) {
    return static_cast<CriterionLevel>(static_cast<int>(lev) + 1);
}

export PlayerColor stringToPlayerColor(std::string col) {
    if (col == "Blue") {
        return PlayerColor::BLUE;
    } else if (col == "Red") {
        return PlayerColor::RED;
    } else if (col == "Orange") {
        return PlayerColor::ORANGE;
    } else if (col == "Yellow") {
        return PlayerColor::YELLOW;
    } else {
        return PlayerColor::INVALID;
    }
}


export std::string playerColorToString(PlayerColor col) {
    switch (col) {
        case PlayerColor::BLUE: return "BLUE";
        case PlayerColor::RED: return "RED";
        case PlayerColor::ORANGE: return "ORANGE";
        case PlayerColor::YELLOW: return "YELLOW";
        default: return "BLUE";
    }
}

export char playerColorToChar(PlayerColor col) {
    switch (col) {
        case PlayerColor::BLUE: return 'B';
        case PlayerColor::RED: return 'R';
        case PlayerColor::ORANGE: return 'O';
        case PlayerColor::YELLOW: return 'Y';
        default: return 'B';
    }
}
