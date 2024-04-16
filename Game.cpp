#include "Game.h"

// Private functions
// Initialize variables
void Game::initVariables() {
    // Init window
    this->window = nullptr;

    // Init game logic
    gameActive = true;
    hasWinYet = false;

    // Load images 
    this->bombImage.loadFromFile("Image\\bomb.png");
    this->number0Image.loadFromFile("Image\\number0.png");
    this->number1Image.loadFromFile("Image\\number1.png");
    this->number2Image.loadFromFile("Image\\number2.png");
    this->number3Image.loadFromFile("Image\\number3.png");
    this->number4Image.loadFromFile("Image\\number4.png");
    this->number5Image.loadFromFile("Image\\number5.png");
    this->number6Image.loadFromFile("Image\\number6.png");
    this->number7Image.loadFromFile("Image\\number7.png");
    this->number8Image.loadFromFile("Image\\number8.png");
    this->flagImage.loadFromFile("Image\\flag.png");
}
void Game::initWindow() {
    this->videoMode.width = 1000;
    this->videoMode.height = 800;
    this->window = new sf::RenderWindow(videoMode, "Minesweeper Game", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(144); // Limit the number of display frame per second (not really necessary)
}
void Game::initCells() {
    cellSize = static_cast<float>(this->videoMode.height) / this->gridSize; // size of each cell in the grid

    // Initialize each cell in the grid
    for (int i = 0; i < gridSize; ++i) {
        std::vector<Cell> row;
        for (int j = 0; j < gridSize; ++j) {
            row.emplace_back(j * cellSize, i * cellSize, i, j, cellSize);
        }
        cells.push_back(row);
    }
}
void Game::initCells(std::vector<std::vector<std::string>> prevCells) {
    cellSize = static_cast<float>(this->videoMode.height) / this->gridSize; // size of each cell in the grid

    // Initialize each cell in the grid
    for (int i = 0; i < gridSize; ++i) {
        std::vector<Cell> row;
        for (int j = 0; j < gridSize; ++j) {
            row.emplace_back(j * cellSize, i * cellSize, i, j, cellSize);
        }
        cells.push_back(row);
    }
    for (int row = 0; row < gridSize; row++) {
        for (int col = 0; col < gridSize; col++) {
            if (prevCells[row][col] == "l") {
                if (this->solutionFrame.solution[row][col] == "0") {
                    this->performCase0(row, col);
                }
                else {
                    this->revealCell(row, col);
                }
                
            }
            else if (prevCells[row][col] == "r") {
                this->cells[row][col].setImage(flagImage);
                this->cells[row][col].hasImage = true;
                this->cells[row][col].isRightClicked = true;
            }
        }
    }
}


void Game::initTimer()
{
    if (!this->font.loadFromFile("fonts\\times.ttf")) { // Replace with the path to your font file
        std::cerr << "Could not load font, exiting." << std::endl;
        return;
    }
    this->clockText.setFont(font);
    this->clockText.setCharacterSize(30); // in pixels
    this->clockText.setFillColor(sf::Color::Yellow);
    this->clockText.setPosition(820, 50); // Position of the text in the window
    startingTimer = 0;
}

void Game::initTimer(int prevTimer) {
    if (!this->font.loadFromFile("fonts\\times.ttf")) { // Replace with the path to your font file
        std::cerr << "Could not load font, exiting." << std::endl;
        return;
    }
    this->clockText.setFont(font);
    this->clockText.setCharacterSize(30); // in pixels
    this->clockText.setFillColor(sf::Color::Yellow);
    this->clockText.setPosition(820, 50); // Position of the text in the window
    startingTimer = prevTimer;
}

void Game::revealCell(int row, int col)
{
    std::vector<int> index;
    index.push_back(row);
    index.push_back(col);
    this->cells[row][col].isLeftClicked = true;
    int clickedValue = std::stoi(this->solutionFrame.solution[row][col]);
    switch (clickedValue) {
    case -1:
        this->performCaseBomb();
        break;
    case 0:
        this->cells[row][col].setImage(number0Image);
        break;
    case 1:
        this->cells[row][col].setImage(number1Image);
        break;
    case 2:
        this->cells[row][col].setImage(number2Image);
        break;
    case 3:
        this->cells[row][col].setImage(number3Image);
        break;
    case 4:
        this->cells[row][col].setImage(number4Image);
        break;
    case 5:
        this->cells[row][col].setImage(number5Image);
        break;
    case 6:
        this->cells[row][col].setImage(number6Image);
        break;
    case 7:
        this->cells[row][col].setImage(number7Image);
        break;
    case 8:
        this->cells[row][col].setImage(number8Image);
        break;
    }
}

void Game::performCase0(int row, int col) {
    std::vector<int> index = { row, col };
    std::queue<std::vector<int>> queue;
    queue.push(index);

    while (!queue.empty()) {
        std::vector<int> currentIndex = queue.front();
        queue.pop();
        int currentRow = currentIndex[0];
        int currentCol = currentIndex[1];
        this->revealCell(currentRow, currentCol);
        std::vector<std::vector<int>> currentListOfSurroundingIdx = solutionFrame.surroundingPixel(currentIndex);
        for (std::vector<int> currentSurroundingIdx : currentListOfSurroundingIdx) {
            int currSurrRow = currentSurroundingIdx[0];
            int currSurrCol = currentSurroundingIdx[1];
            if (this->solutionFrame.solution[currSurrRow][currSurrCol] == "0") {
                if (!this->cells[currSurrRow][currSurrCol].hasImage) {
                    queue.push(std::vector<int> {currSurrRow, currSurrCol});                   
                }
            }
            if (!this->cells[currSurrRow][currSurrCol].hasImage) {
                this->revealCell(currSurrRow, currSurrCol);
            }
        }
    }
}

void Game::performCaseBomb()
{
    for (std::vector<int> bombIndices : solutionFrame.listOfBombIndices) {
        this->cells[bombIndices[0]][bombIndices[1]].setImage(bombImage);
        this->cells[bombIndices[0]][bombIndices[1]].isLeftClicked = true;
    }
    gameActive = false;
    std::cout << "Oops! You lost.\n" << "Your received score: 0.\n"; //Need Adjustments
}

bool Game::checkWincase()
{
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            if (this->solutionFrame.solution[i][j] != "-1") {
                if (!this->cells[i][j].hasImage) {
                    return false;
                }
            }
        }
    }
    return true;
}

/*PUBLIC*/
// Constructor / Destructor
Game::Game(int size, int bombs) : gridSize(size), numOfBombs(bombs), solutionFrame(size, bombs) {
    // Calling functions for initailizations
    this->initVariables();
    this->initWindow();
    this->initCells();
    this->initTimer();
}

Game::Game(int prevGridSize, int prevNumOfBombs, int prevTimer, std::vector<std::vector<int>> prevListOfBombs, std::vector<std::vector<std::string>> prevSolutionFrame, std::vector<std::vector<std::string>> prevCells)
{
    this->gridSize = prevGridSize;
    this->numOfBombs = prevNumOfBombs;
    this->solutionFrame = SolutionFrame(prevGridSize, prevNumOfBombs, prevListOfBombs, prevSolutionFrame);
    this->initVariables();
    this->initWindow();
    this->initCells(prevCells);
    this->initTimer(prevTimer);
}

Game::~Game() {
    delete this->window;
}

// Accessors
const bool Game::running() const {
    return this->window->isOpen();
}

int Game::getPoint()
{
    if (this->hasWinYet) {
        sf::Time elapsed = clock.getElapsedTime();
        int totalSeconds = static_cast<int>(elapsed.asSeconds());
        int score = 0;
        switch (this->gridSize) {
        case 16: // hard
            score = 3000;
            break;
        case 12: //medium
            score = 2000;
            break;
        case 8: //easy
            score = 1000;
            break;
        }
        return (score - totalSeconds * 10);
    }
    else {
        return 0;
    }
}

// Public functions
void Game::pollEvents() {
    while (this->window->pollEvent(this->ev)) {
        switch (this->ev.type) {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape) {
                /** Save game process
                    - gridSize
                    - numOfBombs
                    - SolutionFrame.listOfBombIndices
                    - SolutionFrame.solution
                    - cells matrix: l for isLeftClicked, r for isRightClicked, m for nothing
                    - timer
                    2.
                */
                std::ofstream fileToSaveGame("gamesave.txt", std::ios::out);
                fileToSaveGame << this->gridSize << std::endl;
                fileToSaveGame << this->numOfBombs << std::endl;
                for (std::vector<int> bombIndices : this->solutionFrame.listOfBombIndices) {
                    fileToSaveGame << bombIndices[0] << "," << bombIndices[1] << std::endl;
                }
                for (std::vector<std::string> row : this->solutionFrame.solution) {
                    for (std::string e : row) {
                        fileToSaveGame << e << " ";
                    }
                    fileToSaveGame << std::endl;
                }
                for (std::vector<Cell> row : this->cells) {
                    for (Cell e : row) {
                        if (e.isLeftClicked) {
                            fileToSaveGame << "l" << " ";
                        }
                        else if (e.isRightClicked) {
                            fileToSaveGame << "r" << " ";
                        }
                        else {
                            fileToSaveGame << "m" << " ";
                        }
                    }
                    fileToSaveGame << std::endl;
                }
                fileToSaveGame << std::to_string(static_cast<int>(this->clock.getElapsedTime().asSeconds()));
                fileToSaveGame.close();
                std::cout << "Your game saved!";
                this->window->close();
            }
            break;

            // Click on the cells
        case sf::Event::MouseButtonPressed:
            int row = this->mousePosWin.y / this->cellSize;
            int col = this->mousePosWin.x / this->cellSize;

            if (row >= 0 && row < gridSize && col >= 0 && col < gridSize && gameActive && (!hasWinYet)) {
                /**
                    Leftclick case:
                    - View the corresponding images.
                    Rightclick case:
                    - Click to flag. Reclick to unflag
                */
                if (ev.mouseButton.button == sf::Mouse::Left) {
                    if (this->solutionFrame.solution[row][col] == "-1") {
                        this->performCaseBomb();
                    }
                    else if (this->solutionFrame.solution[row][col] == "0") {
                        this->performCase0(row, col);
                    }
                    else {
                        this->revealCell(row, col);
                    }
                    //this->revealCell(row, col);
                    ;
                }
                else if (ev.mouseButton.button == sf::Mouse::Right) {
                    if (!this->cells[row][col].hasImage) {
                        this->cells[row][col].setImage(flagImage);
                        this->cells[row][col].isRightClicked = true;
                    }
                    else {
                        this->cells[row][col].hasImage = false;
                    }
                }
            }
        }
    }
}

void Game::updateMousePosition() {
    this->mousePosWin = sf::Mouse::getPosition(*this->window);
}

void Game::updateWinStatus() {
    this->hasWinYet = this->checkWincase();
    if (this->hasWinYet) {
        gameActive = false;
    }
}

void Game::updateTimer() {
    sf::Time elapsed = clock.getElapsedTime();

    int totalSeconds = static_cast<int>(elapsed.asSeconds()) + startingTimer;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    // Format the string as "Time: MM:SS"
    std::ostringstream timeStream;
    timeStream << "Time: "
        << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;
    this->clockText.setString(timeStream.str());
}

void Game::update() {
    this->pollEvents();

    // Update mouse postion
    this->updateMousePosition();

    // Update wincase
    this->updateWinStatus();

    // Update Timer
    this->updateTimer();
}

void Game::renderCells() {
    for (int i = 0; i < this->gridSize; i++) {
        for (int j = 0; j < this->gridSize; j++) {
            if (!this->cells[i][j].hasImage) {
                this->window->draw(this->cells[i][j].shape);
            }
            else {
                this->window->draw(this->cells[i][j].sprite);
            }
        }
    }
}

void Game::render() {
    this->window->clear(sf::Color::Black); // Clear old frame

    // Draw game objects
    this->renderCells();
    this->window->draw(this->clockText);

    this->window->display(); // Display frame in window
    if (this->hasWinYet) {
        std::cout << "Congratulations! You won the game.";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        this->window->close();
    }
    if (!this->gameActive) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        this->window->close();
    }
}