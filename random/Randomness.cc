export module Randomness;

import <random>;
import <unordered_map>;
import <vector>;

export class RandomGenerator {
    int seed;
    std::mt19937 rng;
    public:
        RandomGenerator(int seed);
        RandomGenerator();
        /*
        Returns a random integer between min and max, inclusive
        */
        int getRandom(int min=0, int max=1);
        void setSeed(int seed);
        /*
        Returns a bool indicating if the probability was successfully hit.
        The probability is numerator / denominator
        */
       bool getRandomProbability(int numerator, int denominator);
       /*
       Given a set of probabilities for each event, returns an integer indicating 
       which event occured (0-indexed)
       */
      int getRandomIndex(const std::vector<int> & numerators, int denom);
      /*
      Given a set of probabilities for each event, returns the event that occured
      */
      template<typename T>
      T getRandomKey(const std::unordered_map<T, int>& numerators, int denominator);
};

/*
Same file due to compilation issues
*/
template<typename T>
T RandomGenerator::getRandomKey(const std::unordered_map<T, int>& numerators, int denominator) {
    int cumulativeSum = 0;
    std::vector<std::pair<T, int>> cumulativeSums;
    for (const auto& [key, num] : numerators) {
        cumulativeSum += num;
        cumulativeSums.push_back({key, cumulativeSum});
    }
    std::uniform_int_distribution<int> distribution(1, denominator);
    
    int randomValue = distribution(rng);
    
    for (const auto& [key, sum] : cumulativeSums) {
        if (randomValue <= sum) {
            return key;
        }
    }
    
    return T{}; // shouldn't reach here
}
