/*
COT Project 2
The program includes the code of the solution to the program to find the optimal sequence alignment.
The solution is solved by the iterative or memoized dynamic programming algorithm.

Input : input.txt : the file that includes the matrix and the sequence to find from the matrix
Output: output.txt : the file that include the minumim alignment score, the initial row and column, and the optimum alignment
*/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

using namespace std;

#define MAX_ROW 100 //MAX ROW
#define MAX_COLUMN 100 //MAX COLUMN

#define MAX_VALUE 10000 //MAX VALUE

/*
Shared Variable
*/
//The variables that represent the limit of data container
int sequenceLength, nRow, nColumn;

//The data container to store the input sequence and matrix
vector<int> sequence;
vector<vector<int>> matrix;

//The variables for output
int minAlignScore = MAX_VALUE;
int initRow, initColumn;
vector<char> optimumAlign;

//The data container that store the list of adjacent elements
vector<vector<vector<int>>> D;

//The variable to calcurate minAlign Score
vector<int> total;


/*
Pre-defined Functions
*/
void WrapAlign();
int MemoAlign(int row, int column, int current);

/*
Main Function
*/
int main(){
    //open input file
    ifstream inputFile;
    inputFile.open("input2.txt");
    if(!inputFile.is_open()){
        cout << "ERROR ON OPENING FILE";
        return 1;
    }

    //read the first sentence of the input file and get sequece of Length, the number of Rows and Columns
    inputFile >> sequenceLength >> nRow >> nColumn;

    if(nRow > MAX_ROW || nColumn > MAX_COLUMN){
        cout << "THE NUMBER OF ROW OR COLUMN EXCEEDS THE LIMIT";
        inputFile.close();
        return 1;
    }

    //Make the array to represent the sequence
    sequence.resize(sequenceLength);
    for(int i = 0; i < sequenceLength; i++){
        inputFile >> sequence[i];
    }

    //Make the 2D array to represent the matrix
    matrix.resize(nRow, vector<int>(nColumn));
    for(int i = 0; i < nRow; i++){
        for (int j = 0; j < nColumn; j++){
            inputFile >> matrix[i][j];
        }
    }

    //close the inputFile
    inputFile.close();

    //pass the sequence and matrix as the arguments into WrapAlign()
    WrapAlign();
    
    return 0;
}

/*
Functions
*/
//the wrapper function to initialize data structure and write output into file.
void WrapAlign(){
    //The 1-D array to store the direction to proceed
    optimumAlign.resize(sequenceLength-1);
    //3-D array to store the result of MemoAlign
    D.resize(nRow, vector<vector<int>>(nColumn, vector<int>(sequenceLength, -1)));

    //Start with 0
    MemoAlign(0, 0, 0);

    //write the result into file
    ofstream outFile;
    outFile.open("output.txt");
    outFile << minAlignScore << '\n'; // First line consist of the minumim alignment score
    outFile << initRow + 1 << ' ' << initColumn + 1 << '\n'; // Second line contain the inital row and column for the first element of the sequence (Base is 1).
    //Third line indicating how the optimum alignment should proceed
    for (int i = 0; i < optimumAlign.size(); i++){
        if (i == optimumAlign.size() - 1){ //if the element is last element
            outFile << optimumAlign[i];
        }
        else{
            outFile << optimumAlign[i] << ' ';
        }
    }
    outFile.close();
}

//the function to find output elements.
//the function to find output elements and return them.
int MemoAlign(int row, int column, int current){
    //Base case
    //Check the data structure
    if(D[row][column][current] != -1){
        return D[row][column][current];
    }
    //if the current index is the last index of sequence
    else if(current == sequenceLength -1){
        D[row][column][current] = abs(matrix[row][column] - sequence[current]);
        return D[row][column][current];
    }
    else{
        //Look for the minimim difference between adjacent elements and next sequence element
        vector<int> directions(4, MAX_VALUE);
        //Up
        if(row-1 >= 0){
            directions[0] = abs(matrix[row -1][column] - sequence[current+1]);
        }
        //Down
        if(row+1 < nRow){
            directions[1] = abs(matrix[row+1][column] - sequence[current+1]);
        }
        //Left
        if(column-1 >= 0){
            directions[2] = abs(matrix[row][column-1] - sequence[current+1]);
        }
        //Right
        if(column+1 < nColumn){
            directions[3] = abs(matrix[row][column+1] - sequence[current+1]);
        }

        //Compare with minimum difference
        auto min = std::min_element(directions.begin(), directions.end());
        vector<int> directionsTotal(4, MAX_VALUE);
        if(directions[0] == *min){
            directionsTotal[0] = MemoAlign(row-1, column, current+1);
        }
        if(directions[1] == *min){
            directionsTotal[1] = MemoAlign(row+1, column, current+1);
        }
        if(directions[2] == *min){
            directionsTotal[2] = MemoAlign(row, column-1, current+1);
        }
        if(directions[3] == *min){
            directionsTotal[3] = MemoAlign(row, column+1, current+1);
        }

        //Set the proceed direction
        auto minTotal = min_element(directionsTotal.begin(), directionsTotal.end());
        if(directionsTotal[0] == *minTotal){
            optimumAlign[current] = 'U';
        }
        else if(directionsTotal[1] == *minTotal){
            optimumAlign[current] = 'D';
        }
        else if(directionsTotal[2] == *minTotal){
            optimumAlign[current] = 'L';
        }
        else if(directionsTotal[3] == *minTotal){
            optimumAlign[current] = 'R';
        }

        //Set the value in 3D vector
        D[row][column][current] = *minTotal + abs(matrix[row][column] - sequence[current]);
        if(current == 0){
            if(minAlignScore > *minTotal){
                minAlignScore = D[row][column][current];
                initRow = row;
                initColumn = column;
            }
            if(minAlignScore != 0){
                //Up
                if(row-1 >= 0){
                    if(MemoAlign(row-1, column, 0) == 0){
                        return minAlignScore;
                    }
                }
                //Down
                if(row+1 < nRow){
                    if(MemoAlign(row+1, column, 0) == 0){
                        return minAlignScore;
                    }
                }
                //Left
                if(column-1 >= 0){
                    if(MemoAlign(row, column-1, 0) == 0){
                        return minAlignScore;
                    }
                }
                //Right
                if(column+1 < nColumn){
                    if(MemoAlign(row, column+1, 0) == 0){
                        return minAlignScore;
                    }
                }
            }
        }
        return D[row][column][current];
    }
}