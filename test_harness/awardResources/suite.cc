import <iostream>;
import GameController;

const std::string SAVE_FILENAME = "backup.sv";

int main(int argc, char** argv) {
    int SEED = 1000;
    GameController controller{SEED};
    if (argc > 1) {
        std::string argOne = argv[1];
        if (argOne == "-board") {
            std::string filename = argv[2];
            controller.setBoardFile(filename);
        } else {
            // load game
            std::string filename = argv[2];
            controller.setGameFile(filename);
        }
    }
    try {
        controller.startGame();
    } catch (...) {
        controller.saveGame(SAVE_FILENAME);
    }
}
