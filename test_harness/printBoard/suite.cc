import <vector>;

import BoardView;
import HexTile;
import Board;
import types;
import Criterion;
import Goal;
import Randomness;
import Dice;
import FairDice;
import Player;
import Student;

int main() {
    const int NUM_GOALS = 72;
    const int NUM_CRITERIONS = 54;
    const int NUM_HEXTILES = 19;

    const int SEED = 1000;

    RandomGenerator randomizer{SEED};

    // Create a list of goals
    std::vector<Goal*> goals;
    for (int i = 0; i < NUM_GOALS; ++i) {
        goals.push_back(new Goal{i});
    }

    // Create a list of criterions
    std::vector<Criterion*> criterions;
    for (int i = 0; i < NUM_CRITERIONS; ++i) {
        criterions.push_back(new Criterion{i});
    }

    // Randomly initialize hex tiles
    std::vector<HexTile*> hexes;
    for (int i = 0; i < NUM_HEXTILES; ++i) {
        // Get a random ResourceType value (assuming there are 6 ResourceTypes, indexed from 0 to 5)
        ResourceType tileResource = static_cast<ResourceType>(randomizer.getRandom(0, numResourceTypes() - 1));
        int tileValue = randomizer.getRandom(1, 6)+randomizer.getRandom(1, 6);  // Assuming tile values range between 2 and 12
        hexes.push_back(new HexTile{tileResource, i, tileValue});
    }

    Dice* fairDice = new FairDice{&randomizer};

    // create 3 students with different colors
    std::vector<int> empty{};
    Player* student1 = new Student{PlayerColor::RED, empty, empty, fairDice};
    Player* student2 = new Student{PlayerColor::BLUE, empty, empty, fairDice};
    Player* student3 = new Student{PlayerColor::YELLOW, empty, empty, fairDice};

    std::vector<Player*> students{student1, student2, student3};

    // randomly (not really) assign some goals and criterions to students
    for (int i = 0; i < NUM_GOALS / 10; ++i) {
        int goalIndex = 3 * i;
        int studentIndex = randomizer.getRandom(0, 2); // Random student (0, 1, or 2)
        goals[goalIndex]->complete(students[studentIndex]);
    }
    // randomly (not really) sign students to criterions
    for (int i = 0; i < NUM_CRITERIONS / 10; ++i) {
        int criterionIndex = 5* i;
        int studentIndex = randomizer.getRandom(0, 2); // Random student (0, 1, or 2)
        criterions[criterionIndex]->complete(students[studentIndex]);
    }

    Board* board = new Board{5, hexes, criterions, goals};
    BoardView* view = new BoardView{};
    view->printBoard(board);
}