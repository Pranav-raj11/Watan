import Randomness;
import <iostream>;
import <sstream>;

int main(int argc, char** argv) {
    int seed;
    RandomGenerator myRandomGenerator;
    if (argc == 2) { // seed provided
        std::istringstream iss{argv[1]};
        iss >> seed;
        myRandomGenerator = RandomGenerator{seed};
    } else {
        myRandomGenerator = RandomGenerator{};
    }
    // now just print 100 random numbers between 0 and 19, inclusive
    for (int i = 0; i < 100; i++) {
        std::cout << myRandomGenerator.getRandom(20) << std::endl;
    }
}