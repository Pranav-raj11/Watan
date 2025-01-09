module Randomness;


RandomGenerator::RandomGenerator(int seedNum) : seed{seedNum}, rng(seedNum) {}

RandomGenerator::RandomGenerator(): seed{-1}, rng(std::random_device{}()) {

}

int RandomGenerator::getRandom(int min, int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(rng);
}

void RandomGenerator::setSeed(int seedNum) {
    this->seed = seedNum;
    rng = std::mt19937(seedNum); // Create a new generator with the seed instead of just seeding
}

bool RandomGenerator::getRandomProbability(int numerator, int denominator) { 
    std::uniform_int_distribution<int> distribution(1, denominator); 
    return distribution(rng) <= numerator;
}


int RandomGenerator::getRandomIndex(const std::vector<int>& numerators, int denominator) { 
    int cumulativeSum = 0; 
    std::vector<int> cumulativeSums; 
    for (int num : numerators) { 
        cumulativeSum += num; 
        
        cumulativeSums.push_back(cumulativeSum); 
    } 
    std::uniform_int_distribution<int> distribution(1, denominator); 
    
    int randomValue = distribution(rng); 
    
    for (size_t i = 0; i < cumulativeSums.size(); ++i) { 
        if (randomValue <= cumulativeSums[i]) { return i; } 
    } 
    
    return -1; // shouldnt reach here
}