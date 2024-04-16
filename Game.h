#pragma once

#include<iostream>
#include<vector>
#include<string>
#include<iomanip>
#include<random>
#include<algorithm>
#include<queue>
#include<unordered_set>
#include<thread>
#include<chrono>
#include<sstream>
#include<fstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

// Highscore struct
struct Highscore {
	std::string playerName;
	int gameScore;

	// Constructor for convenience
	Highscore(const std::string& name, int score) : playerName(name), gameScore(score) {}

	// Comparison operator for sorting
	bool operator<(const Highscore& other) const {
		return gameScore > other.gameScore; // Descending order
	}
};

/** Cell struct
Attributes:
	- rowIndex, colIndex: indices to access and retrieve from gameFrame
	- isLeftCliked, isRightClicked: determine wheither it is clicked or flagged
Method:
	- hasImage to check its sprite
	- Constructor to set up its properties and position relative to the window frame.
*/
struct Cell {
	sf::RectangleShape shape;
	int rowIndex, colIndex;
	bool isLeftClicked = false;
	bool isRightClicked = false;
	bool hasImage = false;
	sf::Sprite sprite;

	Cell(int x, int y, int row, int col, float cellSize) : rowIndex(row), colIndex(col) {
		shape.setPosition(x, y);
		shape.setSize(sf::Vector2f(cellSize, cellSize));
		shape.setFillColor(sf::Color::Cyan);
		shape.setOutlineThickness(3.f);
		shape.setOutlineColor(sf::Color::Magenta);
	}
	void setImage(const sf::Texture& texture) {
		sprite.setTexture(texture);
		sprite.setPosition(shape.getPosition());

		// Calculate scale factors
		float scaleX = shape.getSize().x / texture.getSize().x;
		float scaleY = shape.getSize().y / texture.getSize().y;
		sprite.setScale(scaleX, scaleY);

		hasImage = true;
	}
};

/** SolutionFrame Struct
Attributes:
	- Initialized with a frame size and a number of bombs
	- solutionFrame and listOfBombIndices for later retrieving
Methods:
	- createRandomBombs() to create a set number of bombs
	- setUpSolutionFrame() to fill with number of surrounding bombs with a helper method of surroundingPixel
*/
struct SolutionFrame {
	int numOfBombs;
	int frameSize;
	std::vector<std::vector<int>> listOfBombIndices;
	std::vector<std::vector<std::string>> solution;

	SolutionFrame() : frameSize(0), numOfBombs(0) {}
	SolutionFrame(int framesize, int numofbombs) : frameSize(framesize), numOfBombs(numofbombs) {
		listOfBombIndices = randomBombs();
		solution = setUpSolutionFrame(listOfBombIndices);
		 printoutframe(solution); // For testingk
	}
	SolutionFrame(int framesize, int numofbombs, std::vector<std::vector<int>> listOfbombindices, std::vector<std::vector<std::string>> prevsolution) {
		numOfBombs = numofbombs;
		frameSize = framesize;
		listOfBombIndices = listOfbombindices;
		solution = prevsolution;
	}
	void printoutframe(std::vector<std::vector<std::string>> frame) {
		for (int rowidx = 0; rowidx < frameSize; rowidx++) {
			for (int colidx = 0; colidx < frameSize; colidx++) {
				std::cout << std::setw(2) << std::setfill(' ') << frame[rowidx][colidx];
			}
			std::cout << std::endl;
		}
	}
	std::vector<std::vector<std::string>> setUpSolutionFrame(std::vector<std::vector<int>> listofbombindices) {
		std::vector<std::vector<std::string>> returnFrame(frameSize, std::vector<std::string>(frameSize));
		for (const std::vector<int>& randomGeneratedBombIndices : listofbombindices) {
			int randomGeneratedRowIdx = randomGeneratedBombIndices[0];
			int randomGeneratedColIdx = randomGeneratedBombIndices[1];
			returnFrame[randomGeneratedRowIdx][randomGeneratedColIdx] = "-1";
		}
		for (int rowIdx = 0; rowIdx < frameSize; rowIdx++) {
			for (int colIdx = 0; colIdx < frameSize; colIdx++) {
				if (returnFrame[rowIdx][colIdx] != "-1") {
					std::vector<std::vector<int>> boxSurroundingIndices = surroundingPixel(std::vector<int>{rowIdx, colIdx});
					int count = 0;
					for (const std::vector<int>& currSurroundingIdx : boxSurroundingIndices) {
						if (returnFrame[currSurroundingIdx[0]][currSurroundingIdx[1]] == "-1") {
							count += 1;
						}
					}
					returnFrame[rowIdx][colIdx] = std::to_string(count);
				}
			}
		}
		return returnFrame;
	}
	std::vector<std::vector<int>> randomBombs() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::vector<std::vector<int>> listOfRandomIndices;
		while (listOfRandomIndices.size() < static_cast<size_t>(numOfBombs)) {
			int newRowIdx = gen() % frameSize;
			int newColIdx = gen() % frameSize;
			std::vector<int> newIndices = { newRowIdx, newColIdx };
			if (find(listOfRandomIndices.begin(), listOfRandomIndices.end(), newIndices) == listOfRandomIndices.end()) {
				listOfRandomIndices.push_back(newIndices);
			}
		}
		return listOfRandomIndices;
	}
	std::vector<std::vector<int>> surroundingPixel(const std::vector<int>& index) {
		int selectedRowIdx = index[0];
		int selectedColIdx = index[1];
		int lowerRowIdx, upperRowIdx, lowerColIdx, upperColIdx;
		std::vector<int> lowupRowColIdx;

		if (selectedRowIdx == selectedColIdx && selectedRowIdx == 0) {
			lowupRowColIdx = { 0, 1, 0, 1 };
		}
		else if (selectedRowIdx == frameSize - 1 && selectedColIdx == 0) {
			lowupRowColIdx = { frameSize - 2, frameSize - 1, 0, 1 };
		}
		else if (selectedRowIdx == 0 && selectedColIdx == frameSize - 1) {
			lowupRowColIdx = { 0, 1, frameSize - 2, frameSize - 1 };
		}
		else if (selectedRowIdx == frameSize - 1 && selectedColIdx == frameSize - 1) {
			lowupRowColIdx = { frameSize - 2, frameSize - 1, frameSize - 2, frameSize - 1 };
		}
		else if (selectedRowIdx == 0) {
			lowupRowColIdx = { 0, 1, selectedColIdx - 1, selectedColIdx + 1 };
		}
		else if (selectedColIdx == 0) {
			lowupRowColIdx = { selectedRowIdx - 1, selectedRowIdx + 1, 0, 1 };
		}
		else if (selectedRowIdx == frameSize - 1) {
			lowupRowColIdx = { frameSize - 2, frameSize - 1, selectedColIdx - 1, selectedColIdx + 1 };
		}
		else if (selectedColIdx == frameSize - 1) {
			lowupRowColIdx = { selectedRowIdx - 1, selectedRowIdx + 1, frameSize - 2, frameSize - 1 };
		}
		else {
			lowupRowColIdx = { selectedRowIdx - 1, selectedRowIdx + 1, selectedColIdx - 1, selectedColIdx + 1 };
		}

		lowerRowIdx = lowupRowColIdx[0];
		upperRowIdx = lowupRowColIdx[1];
		lowerColIdx = lowupRowColIdx[2];
		upperColIdx = lowupRowColIdx[3];

		std::vector<std::vector<int>> boxSurroundingIndices;
		for (int rowIdx = lowerRowIdx; rowIdx <= upperRowIdx; rowIdx++) {
			for (int colIdx = lowerColIdx; colIdx <= upperColIdx; colIdx++) {
				//if (rowIdx != selectedRowIdx || colIdx != selectedRowIdx) {
				//	boxSurroundingIndices.push_back({ rowIdx, colIdx });
				//}
				boxSurroundingIndices.push_back({ rowIdx, colIdx });
			}
		}
		return boxSurroundingIndices;
	}
};

class Game
{
private:
	// Variables
	// Window
	sf::RenderWindow* window;
	sf::Event ev;
	sf::VideoMode videoMode; // Window size
	
	// Mouse position relative to the game window
	sf::Vector2i mousePosWin;

	// Game logic
	bool gameActive;
	bool hasWinYet;

	/** GAME OBJECTS
	* - Cell grid
	* - Cell image
	* - Solution frame
	* - Timer: text + clock
	*/ 
	// Cell grid
	int gridSize;
	int numOfBombs;
	int startingTimer;
	float cellSize;
	std::vector<std::vector<Cell>> cells;
	// Number images when clicked
	sf::Texture bombImage;
	sf::Texture number0Image;
	sf::Texture number1Image;
	sf::Texture number2Image;
	sf::Texture number3Image;
	sf::Texture number4Image;
	sf::Texture number5Image;
	sf::Texture number6Image;
	sf::Texture number7Image;
	sf::Texture number8Image;
	sf::Texture flagImage;
	// Solution frame for retrieving
	SolutionFrame solutionFrame = SolutionFrame();
	// Timer
	sf::Font font;
	sf::Text clockText;
	sf::Clock clock;

	// Private functions
	void initVariables(); 
	void initWindow();
	void initCells();
	void initTimer();
	void initCells(std::vector<std::vector<std::string>> prevCells); // when continue game 
	void initTimer(int prevTimer); // when continue game
	// Helper functions when clicking on special cell
	void revealCell(int row, int col);
	void performCase0(int row, int col);
	void performCaseBomb();

	// Check if player has win game
	bool checkWincase();
public:
	// Constructor / Destructor
	Game(int inputGridsize, int inputNumOfBombs);
	Game(int prevGridSize, int prevNumOfBombs, int prevTimer, std::vector<std::vector<int>> prevListOfBombIndices, std::vector<std::vector<std::string>> prevSolutionFrame, std::vector<std::vector<std::string>> prevCells);
	virtual ~Game();

	// Assessors
	const bool running() const;
	int getPoint();

	// Public Functions
	void pollEvents();
	void updateMousePosition();
	void updateWinStatus();
	void updateTimer();
	void update();
	void renderCells();
	void render();
};

