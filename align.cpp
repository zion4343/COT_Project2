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

using namespace std;

#define MAX_ROW 100 //MAX ROW
#define MAX_COLUMN 100 //MAX COLUMN

/*
Class
*/
//The class used to store the result of MemoAlign
class Sentinel{
    int sentinel = 0; //if the value is not 0, the bool value is updated
    bool result = false; // The result of MemoAlign

    public:
        void updateResult(bool newValue){sentinel = 1; result = newValue;}
        int returnSentinel(){return sentinel;}
        bool returnResult(){return result;}
};

/*
Shared Variable
*/
//The variables that represent the limit of data container
int sequenceLength, nRow, nColumn;

//The data container to store the input sequence and matrix
vector<int> sequence;
vector<vector<int>> matrix;

//The variables for output
int minAlignScore, initRow, initColumn;
vector<char> optimumAlign;

//The data container that store the list of adjacent elements
vector<vector<vector<Sentinel>>> D;

//The variable to calcurate minAlign Score
int traversed = 0;
int maxScoreInMatrix = 0;


/*
Pre-defined Functions
*/
void WrapAlign();
bool MemoAlign(int row, int column, int current);

/*
Main Function
*/
int main(){
    //open input file
    ifstream inputFile;
    inputFile.open("input.txt");
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
    D.resize(nRow, vector<vector<Sentinel>>(nColumn, vector<Sentinel>(sequenceLength)));

    MemoAlign(0, 0, 0); //Start with 0

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
bool MemoAlign(int row, int column, int current){
    //Base case
    //Check the data structure
    if(D[row][column][current].returnSentinel() != 0){
        return D[row][column][current].returnResult();
    }
    //When the current index is the last index of sequence
    if(current == sequenceLength -1){
        minAlignScore = 0;
        return true;
    }
    //count traversed elements
    if(current == 0){
        traversed++;
    }
    //When corrent matrix element is not same with the current sequence element. (Used when current = 0)
    if(matrix[row][column] != sequence[current]){
        if(row-1 >= 0){
            if(MemoAlign(row-1, column, current)){return true;}
        }
        if(row+1 < nRow){
            if(MemoAlign(row+1, column, current)){return true;}
        }
        if(column-1 >= 0){
            if(MemoAlign(row, column-1, current)){return true;}
        }
        if(column+1 < nColumn){
            if(MemoAlign(row, column+1, current)){return true;}
        }
    }
    //When correct matrix element is same with the current sequence element
    else{
        //Check Up
        if(row-1 >= 0){
            if(matrix[row -1][column] == sequence[current + 1]){
                if(MemoAlign(row-1, column, current+1)){
                    if(current == 0){
                        initRow = row;
                        initColumn = column;
                    }
                    optimumAlign[current] = 'U';
                    D[row][column][current].updateResult(true);
                    return true;
                }
            }
        }
        //Check Down
        if(row+1 < nRow){
            if(matrix[row +1][column] == sequence[current + 1]){
                if(MemoAlign(row+1, column, current+1)){
                    if(current == 0){
                        initRow = row;
                        initColumn = column;
                    }
                    optimumAlign[current] = 'D';
                    D[row][column][current].updateResult(true);
                    return true;
                }
            }
        }
        //Check Left
        if(column-1 >= 0){
            if(matrix[row][column -1] == sequence[current + 1]){
                if(MemoAlign(row, column-1, current+1)){
                    if(current == 0){
                        initRow = row;
                        initColumn = column;
                    }
                    optimumAlign[current] = 'L';
                    D[row][column][current].updateResult(true);
                    return true;
                }
            }
        }
        //Check Right
        if(column+1 < nColumn){
            if(matrix[row][column+1] == sequence[current + 1]){
                if(MemoAlign(row, column+1, current+1)){
                    if(current == 0){
                        initRow = row;
                        initColumn = column;
                    }
                    optimumAlign[current] = 'R';
                    D[row][column][current].updateResult(true);
                    return true;
                }
            }
        }
        //If all direction is false
        D[row][column][current].updateResult(false);
        return false;
    }

    return false;
}