#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>

using namespace std;

class SolutionAndPlayerFrame {
private:
    int numOfBombs;
    int frameSize;
    vector<vector<string>> solutionframe;
    vector<vector<string>> playerframe;
    vector<vector<int>> listofbombindices;

public:
    // Constructor
    SolutionAndPlayerFrame(int size, int bombs) : numOfBombs(bombs), frameSize(size) {
        listofbombindices = randomBombs();
        solutionframe = setUpSolutionFrame();
        playerframe = setUpFrame();
        /*
        printoutframe(solutionframe);
        for (vector<int> pairofindices : listofbombindices){
            cout << "Row index: " << pairofindices[0] << " Col index: " << pairofindices[1] << endl;
        }
        */
    }

    // Define helper methods for setting up the frames
private:
    vector<vector<string>> setUpFrame() {
        vector<vector<string>> returnFrame(frameSize, vector<string>(frameSize, "-"));
        return returnFrame;
    }

    vector<vector<string>> setUpSolutionFrame() {
        vector<vector<string>> returnFrame = setUpFrame();
        for (const vector<int>& randomGeneratedBombIndices : listofbombindices) {
            int randomGeneratedRowIdx = randomGeneratedBombIndices[0];
            int randomGeneratedColIdx = randomGeneratedBombIndices[1];
            returnFrame[randomGeneratedRowIdx][randomGeneratedColIdx] = "-1";
        }
        for (int rowIdx = 0; rowIdx < frameSize; rowIdx++) {
            for (int colIdx = 0; colIdx < frameSize; colIdx++) {
                if (returnFrame[rowIdx][colIdx] != "-1") {
                    vector<vector<int>> boxSurroundingIndices = surroundingPixel(vector<int>{rowIdx, colIdx});
                    int count = 0;
                    for (const vector<int>& currSurroundingIdx : boxSurroundingIndices) {
                        if (returnFrame[currSurroundingIdx[0]][currSurroundingIdx[1]] == "-1") {
                            count += 1;
                        }
                    }
                    returnFrame[rowIdx][colIdx] = to_string(count);
                }
            }
        }
        return returnFrame;
    }

    vector<vector<int>> randomBombs() {
        random_device rd;
        mt19937 gen(rd());
        vector<vector<int>> listOfRandomIndices;
        while (listOfRandomIndices.size() < static_cast<size_t>(numOfBombs)) {
            int newRowIdx = gen() % frameSize;
            int newColIdx = gen() % frameSize;
            vector<int> newIndices = { newRowIdx, newColIdx };
            if (find(listOfRandomIndices.begin(), listOfRandomIndices.end(), newIndices) == listOfRandomIndices.end()) {
                listOfRandomIndices.push_back(newIndices);
            }
        }
        return listOfRandomIndices;
    }

public:
    void printoutframe(vector<vector<string>> frame) {
        for (int rowidx = 0; rowidx < frameSize; rowidx++) {
            for (int colidx = 0; colidx < frameSize; colidx++) {
                cout << setw(2) << setfill(' ') << frame[rowidx][colidx];
            }
            cout << endl;
        }
    }
    vector<vector<int>> surroundingPixel(const vector<int>& index) {
        int selectedRowIdx = index[0];
        int selectedColIdx = index[1];
        int lowerRowIdx, upperRowIdx, lowerColIdx, upperColIdx;
        vector<int> lowupRowColIdx;

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

        vector<vector<int>> boxSurroundingIndices;
        for (int rowIdx = lowerRowIdx; rowIdx <= upperRowIdx; rowIdx++) {
            for (int colIdx = lowerColIdx; colIdx <= upperColIdx; colIdx++) {
                boxSurroundingIndices.push_back({ rowIdx, colIdx });
            }
        }
        return boxSurroundingIndices;
    }


    // Define accessor methods
    vector<vector<string>> getSolutionFrame() {
        return solutionframe;
    }
    vector<vector<string>> getPlayerFrame() {
        return playerframe;
    }
    int getSize() {
        return frameSize;
    }
    vector<vector<int>> getListOfBombIndices() {
        return listofbombindices;
    }
};
