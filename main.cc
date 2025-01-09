import <iostream>;
import GameController;

const std::string SAVE_FILENAME = "backup.sv";

int main(int argc, char** argv) {
    // TODO: remove default seed
    int SEED = -1000;
    std::string boardFile = "";
    std::string loadFile = "";
    GameController controller{};

    // parses command line arguments (-seed, -board, -load)
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-seed" && i + 1 < argc) {
            SEED = std::stoi(argv[++i]);
            // set the seed
            controller.setSeed(SEED);
        }
        else if (arg == "-board" && i + 1 < argc) {
            boardFile = argv[++i];
        }
        else if (arg == "-load" && i + 1 < argc) {
            loadFile = argv[++i];
        }
    }

    if (!boardFile.empty()) {
        controller.setBoardFile(boardFile);
    }
    if (!loadFile.empty()) {
        controller.setGameFile(loadFile);
    }

    try {
        controller.beginGameSeries();
    } catch (...) {
        // save. typically after EOF
        controller.saveGame(SAVE_FILENAME);
    }
}
