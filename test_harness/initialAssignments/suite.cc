import <iostream>;
import <sstream>;

import GameController;

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Please provide the filename of the board, and the random seed!" << std::endl;
        return 1;
    }
    std::string boardFilename = argv[1];
    std::istringstream iss{argv[2]};
    int seed;
    iss >> seed;
    GameController myController{seed};
    myController.setBoardFile(boardFilename);
    // also prints the board
    myController.startGame();
    
}