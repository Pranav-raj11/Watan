import <iostream>;

import <memory>;

import <vector>;
import <string>;
import <unordered_map>;

import GameController;
import ModelFacade;
import Randomness;



int main(int argc, char* argv[]) {

    TestHarness harness{};
    std::string command;
    int number;
    while (std::cin >> command >> number) {
        if (command == "-cc") {
            harness.printAdjacentCriterionsToCriterion(number);
        } else if (command == "-gg") {
            harness.printAdjacentGoalsToGoal(number);
        } else if (command == "-gc") {
            harness.printAdjacentCriterionsToGoal(number);
        } else if (command == "-cg") {
            harness.printAdjacentGoalsToCriterion(number);
        }
    }
}
