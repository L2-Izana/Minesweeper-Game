#include "Game.h"

int main()
{
    // Game starting
    std::string startingGamePrompt = "WELCOME TO OUR MINESWEEPER GAME!\n1. New game\n2. How to play\n3. Continue game\n4. Highscores\nYour choice: ";
    int startingChoice;
    while (true) {
        // Setting up new Game
        std::cout << startingGamePrompt;
        std::cin >> startingChoice;
        if (startingChoice == 1) {
            int modeDifficultyChoice;
            std::cout << "Please choose mode difficulty: \n" << "1. Hard: 16x16 grid with 40 bombs\n" << "2. Medium: 12x12 grid with 24 bombs\n" << "3. Easy: 8x8 grid with 12 bombs" << std::endl;
            std::cout << "Your choice: ";
            std::cin >> modeDifficultyChoice;
            int gridSize{}, numOfBombs{};
            switch (modeDifficultyChoice) {
            case 1:
                gridSize = 16;
                numOfBombs = 40;
                break;
            case 2:
                gridSize = 12;
                numOfBombs = 24;
                break;
            case 3:
                gridSize = 8;
                numOfBombs = 12;
                break;
            }

            // Setting up new Game
            Game game(gridSize, numOfBombs);
            std::cout << "Finished seting up! Happy playing." << std::endl;
            std::cout << "If suddenly busy! Please press esc key to save your game." << std::endl;

            // Game loop
            while (game.running()) {
                // Update
                game.update();

                // Render our game
                game.render();
            }
            int gameScore = game.getPoint();
            if (gameScore < 0) {
                std::cout << std::endl;
                std::cout << "Sorry! Your score is below 0." << std::endl;
            }
            if (gameScore > 0) {
                std::cout << std::endl;
                std::cout << "Your score: " << std::to_string(gameScore) << std::endl;
                std::string playerName;
                std::cout << "Please type in your name for recording highscore: ";
                std::cin >> playerName;
                std::ofstream file("highscore.txt", std::ios::app);
                if (file.is_open()) {
                    file << playerName << "|" << gameScore << std::endl;
                    file.close();
                    std::cout << "High score saved successfully.\n";
                }
                else {
                    std::cout << "Error in open file for recording" << std::endl;
                }
                std::cout << "Thank you for playing! Hope to see you again." << std::endl;
            }
            break;
        }
        else if (startingChoice == 2) {
            std::cout << "Objective: Uncover all non-mine squares without detonating mines.\n"
                << "Game Grid: Variable-sized rectangular grid with hidden mines.\n\n"
                << "Square States:\n"
                << "- Unclicked: Initially hidden.\n"
                << "- Clicked (Revealed): Left Click to reveal contents.\n"
                << "- Flagged: Right-click to flag suspected mines.\n\n"
                << "Gameplay:\n"
                << "- Start by clicking any square; the first click is always safe.\n"
                << "- Numbers show adjacent mines count.\n"
                << "- Empty squares reveal adjacent areas, potentially recursively.\n"
                << "- Uncovering a mine ends the game; all mines revealed.\n"
                << "- Flag suspected mine squares for strategy.\n\n"
                << "Winning and Losing:\n"
                << "- Win by revealing all non-mine squares.\n"
                << "- Lose by clicking a mine, revealing all mines.\n\n"
                << "Playing Tips:\n"
                << "- Use numbers to deduce mine locations.\n"
                << "- Logical deduction for safe moves.\n"
                << "- Sometimes, take calculated risks.\n\n"
                << "Minesweeper is a strategic and deductive challenge. Enjoy!\n";
        }
        else if (startingChoice == 3) { // Continue Game
            std::ifstream fileToLoadGame("gamesave.txt", std::ios::in);
            if (!fileToLoadGame.is_open()) {
                std::cout << "Cannot open previous game save" << std::endl;
            }
            int prevGridSize, prevNumOfBombs, prevTimer;
            std::vector<std::vector<int>> prevListOfBombIndices;
            std::vector<std::vector<std::string>> prevSolutionFrame, prevCells;
            
            fileToLoadGame >> prevGridSize;
            fileToLoadGame >> prevNumOfBombs;
            std::string line;
            std::getline(fileToLoadGame, line); // Skip the rest of the current line
            for (int i = 0; i < prevNumOfBombs; ++i) {
                std::getline(fileToLoadGame, line);
                std::stringstream ss(line);
                int x, y;
                char delim;
                ss >> x >> delim >> y;
                prevListOfBombIndices.push_back({ x, y });
            }
            for (int i = 0; i < prevGridSize; ++i) {
                std::getline(fileToLoadGame, line);
                std::stringstream ss(line);
                std::string cellValue;
                std::vector<std::string> row;
                while (ss >> cellValue) {
                    row.push_back(cellValue);
                }
                prevSolutionFrame.push_back(row);
            }
            for (int i = 0; i < prevGridSize; ++i) {
                std::getline(fileToLoadGame, line);
                std::stringstream ss(line);
                std::string cellValue;
                std::vector<std::string> row;
                while (ss >> cellValue) {
                    row.push_back(cellValue);
                }
                prevCells.push_back(row);
            }
            fileToLoadGame >> prevTimer;
            //std::cout << prevGridSize << "\n" << prevNumOfBombs << std::endl; // checked
            //for (std::vector<int> row : prevListOfBombIndices) {
            //    std::cout << row[0] << "," << row[1] << std::endl; //checked
            //}
            //for (std::vector<std::string> row : prevSolutionFrame) {
            //    for (std::string e : row) {
            //        std::cout << e << " ";
            //    }
            //    std::cout << std::endl;
            //}
            //for (std::vector<std::string> row : prevCells) {
            //    for (std::string e : row) {
            //        std::cout << e << " ";
            //    }
            //    std::cout << std::endl;
            //}
            fileToLoadGame.close();
            Game game(prevGridSize, prevNumOfBombs, prevTimer, prevListOfBombIndices, prevSolutionFrame, prevCells);
            std::cout << "Finished seting up! Happy playing." << std::endl;
            std::cout << "If suddenly busy! Please press esc key to save your game." << std::endl;

            // Game loop
            while (game.running()) {
                // Update
                game.update();

                // Render our game
                game.render();
            }
            int gameScore = game.getPoint();
            if (gameScore < 0) {
                std::cout << std::endl;
                std::cout << "Sorry! Your score is below 0." << std::endl;
            }
            if (gameScore > 0) {
                std::cout << std::endl;
                std::cout << "Your score: " << std::to_string(gameScore) << std::endl;
                std::string playerName;
                std::cout << "Please type in your name for recording highscore: ";
                std::cin >> playerName;
                std::ofstream file("highscore.txt", std::ios::app);
                if (file.is_open()) {
                    file << playerName << "|" << gameScore << std::endl;
                    file.close();
                    std::cout << "High score saved successfully.\n";
                }
                else {
                    std::cout << "Error in open file for recording" << std::endl;
                }
                std::cout << "Thank you for playing! Hope to see you again." << std::endl;
            }
            break;
        }
        else {
            // View Highscore
            std::vector<Highscore> highscores;
            std::ifstream file("highscore.txt");
            std::string line;

            // Check if the file is open
            if (!file.is_open()) {
                std::cerr << "Unable to open file highscore.txt" << std::endl;
                return 1;
            }

            // Read the file line by line
            while (getline(file, line)) {
                std::istringstream iss(line);
                std::string playerName;
                int gameScore;

                if (std::getline(iss, playerName, '|') && iss >> gameScore) {
                    highscores.emplace_back(playerName, gameScore);
                }
            }

            file.close();

            // Sort the highscores
            std::sort(highscores.begin(), highscores.end());

            // Print the top 10 high scores
            std::cout << "Top 10 High Scores:" << std::endl;
            int count = 0;
            for (const auto& hs : highscores) {
                std::cout << std::left << std::setw(23) << hs.playerName << " | " << hs.gameScore << std::endl;
                if (++count >= 10) break;
            }
        }
    }

    // End of application
    return 0;
}