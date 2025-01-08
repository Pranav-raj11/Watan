export module ResourceManager;

import <memory>;
import <unordered_map>;
import <vector>;

import types;
import GenericManager;
import ModelFacade;
import Randomness;
import Criterion;
import Goal;
import HexTile;
import Player;
import Board;


export class ResourceManager: public GenericManager {
    public:
        ResourceManager(std::shared_ptr<ModelFacade> data, std::shared_ptr<RandomGenerator> random);

        /*
        Awards players resources after a number is rolled. Returns a map from the resource type
        to the amount the player got.
        1. rollednumber: integer, MUST NOT BE 7.
        2. player: a pointer to the player that will get the reward
        */
        std::unordered_map<ResourceType, int> awardResourceOnRoll(int rolledNumber, Player* player);
        /*
        Returns a list of players who have more than (or equal to) n total resources
        */
        std::vector<Player*> getStudentsWithResources(int n);
        /*
        Deletes a random resource from the provided player. Returns the resource that was deleted
        from the player
        */
        ResourceType deleteRandomResource(Player* player);
    
        /*
        Counts the number of total resources the player has
        */
        int getTotalResources(Player* player);
        /*
        Attempts to trade resources with another player. Throws an error if not successful
        */
        void trade(Player* activePlayer, Player* offeringTo, ResourceType give, ResourceType receive);
        /*
        Completes the given criterion for the given player
        */
        void completeCriterion(Criterion* criterion, Player* activePlayer, std::vector<Criterion*> adjacentCriterions, std::vector<Goal*> adjacentGoals);
        /*
        Attempts to achieve the given goal for the given player
        */
        void achieveGoal(Goal* goal, Player* activePlayer, std::vector<Criterion*> adjacentCriterions, std::vector<Goal*> adjacentGoals);
        /*
        Attempts to improve the given criterion for the given player. Throws an error
        if not successful. The error message will indicate what went wrong.
        */
        void improveCriterion(Criterion* criterion, Player* activePlayer);
    
    private:
        /*
        Returns a booleani ndicating whetehr the player has enough resources
        */
        bool hasEnoughResources(Player* player, ResourceRequirement req);
        /*
        Increments the given player's resources by the given amounts
        */
        void incrementResources(Player* player, ResourceRequirement req);
};
